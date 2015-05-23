#include "common/Logger.h"
#include "AnimationVertex.h"
#include "Camera.h"
#include "Entity.h"
#include "Geometry.h"
#include "Graphics.h"
#include "Materials.h"
#include "Models.h"
#include "ModelGraphicsComponent.h"
#include "BillboardGraphicsComponent.h"
#include "Renderer.h"
#include "Shaders.h"
#include "Textures.h"
#include "World.h"
#include "Interface.h"
#include "Text.h"
#include "Locator.h"
#include <typeinfo>

namespace Arya
{
    Graphics::Graphics()
    {
        renderer = new Renderer;
        camera = new Camera;
    }

    Graphics::~Graphics()
    {
        delete camera;
        delete renderer;
    }

    bool Graphics::init(int width, int height)
    {
        if (!renderer->init()) return false;
        renderer->checkErrors();

        resize(width, height);

        billboardShader = make_shared<ShaderProgram>(
                "../shaders/billboard.vert",
                "../shaders/billboard.frag");
        if (!billboardShader->isValid()) {
            billboardShader = nullptr;
            LogError << "Could not load billboard shader." << endLog;
            return false;
        }

        billboardShader->enableUniform(UNIFORM_MOVEMATRIX | UNIFORM_VPMATRIX | UNIFORM_TEXTURE);

        billboardShader->addUniform2fv("screenOffset",
                [](ShaderUniformBase* b) {
                Entity* e = dynamic_cast<Entity*>(b);
                return e->getGraphics()->getScreenOffset();
                } );
        billboardShader->addUniform2fv("screenSize",
                [](ShaderUniformBase* b) {
                Entity* e = dynamic_cast<Entity*>(b);
                return e->getGraphics()->getScreenSize();
                } );

        viewShader = make_shared<ShaderProgram>(
                "../shaders/view.vert",
                "../shaders/view.frag");
        if (!viewShader->isValid()) {
            viewShader = nullptr;
            LogError << "Could not load view shader." << endLog;
            return false;
        }

        viewShader->enableUniform(UNIFORM_TEXTURE);
        viewShader->addUniform2fv("screenOffset",
                [this](ShaderUniformBase* b) {
                View* v = dynamic_cast<View*>(b);
                return v->getScreenOffset(inverseWindowSize);
                } );
        viewShader->addUniform2fv("screenSize",
                [this](ShaderUniformBase* b) {
                View* v = dynamic_cast<View*>(b);
                return v->getScreenSize(inverseWindowSize);
                } );
        viewShader->addUniform1i("isFont",
                [this](ShaderUniformBase* b) {
                Label* lbl = dynamic_cast<Label*>(b);
                if (lbl) return 1;
                else return 0;
                } );

        renderer->checkErrors();
        return true;
    }

    void Graphics::resize(int width, int height)
    {
        if(height > 0)
            camera->setProjectionMatrix(45.0f, ((float)width)/((float)height), 0.1f, 2000.0f);
        windowWidth = width;
        windowHeight = height;
        inverseWindowSize.x = 1.0f / float(windowWidth);
        inverseWindowSize.y = 1.0f / float(windowHeight);
    }

    void Graphics::clear(int width, int height)
    {
        renderer->checkErrors();
        renderer->clear(width, height);
    }

    void Graphics::render(World* world)
    {
        shared_ptr<Entity> ent;
        auto entities = world->getEntities();
        for(auto weakEnt : entities) {
            ent = weakEnt.lock();
            if (!ent) continue;

            GraphicsComponent* gr = ent->getGraphics();
            if (!gr) continue;

            RenderType type = gr->getRenderType();
            switch(type) {
                case TYPE_MODEL:
                    renderModel((ModelGraphicsComponent*)gr, ent.get());
                    break;
                case TYPE_TERRAIN:
                    break;
                case TYPE_BILLBOARD:
                    renderBillboard((BillboardGraphicsComponent*)gr, ent.get());
                    break;
                default:
                    break;
            }
        }
        return;
    }

    void Graphics::render(Interface* interface)
    {
        // Get the quad if we do not have it yet
        if (!quad2dGeometry)
        {
            shared_ptr<Model> a = Locator::getModelManager().getModel("quad2d");
            if (a->getMeshes().empty()) return;
            quad2dGeometry = a->getMeshes().front()->geometry;
            if (!quad2dGeometry) return;
        }
        viewShader->use();
        renderer->enableBlending(true);
        renderer->enableDepthTest(false);
        renderer->enableDepthWrite(false);
        renderView(interface->getRootView().get());
        renderer->enableBlending(false);
        renderer->enableDepthTest(true);
        renderer->enableDepthWrite(true);
    }

    void Graphics::renderView(View* view)
    {
        if (typeid(*view) == typeid(Image) )
        {
            Image* image = dynamic_cast<Image*>(view);

            Material* mat = image->material.get();
            if (mat)
            {
                viewShader->doUniforms(view);
                renderer->renderGeometry(quad2dGeometry.get(), mat, viewShader.get());
            }
        }
        else if (typeid(*view) == typeid(Label) )
        {
            Label* lbl = dynamic_cast<Label*>(view);

            auto geom = lbl->getGeometry();
            if (geom)
            {
                viewShader->doUniforms(view);
                renderer->renderGeometry(geom, lbl->getFont()->getFontMaterial().get(), viewShader.get());
            }
        }

        auto children = view->getChildren();
        for (auto v : children)
            renderView(v.get());
        return;
    }

    void Graphics::update(float elapsed)
    {
        camera->update(elapsed);
    }

    vec2 Graphics::normalizeMouseCoordinates(int x, int y)
    {
        return vec2(
                -1.0f + 2.0f*float(x)/float(windowWidth),
                -1.0f + 2.0f*float(y)/float(windowHeight) );
    }

    void Graphics::renderModel(ModelGraphicsComponent* gr, Entity* e)
    {
        Model* model = gr->getModel();
        if(!model) return;
        ShaderProgram* shader = model->getShaderProgram().get();
        if(!shader) return;

        shader->use();
        shader->setMoveMatrix(gr->getMoveMatrix());
        shader->setViewMatrix(camera->getVMatrix());
        shader->setViewProjectionMatrix(camera->getVPMatrix());
        shader->doUniforms(e);

        //TODO: Investigate the bounding box and also check onScreen.z ?
        //mat4 totalMatrix = camera->getVPMatrix() * ent->getMoveMatrix();
        //bool flag = false;
        //for(int j = 0; j < 8; j++)
        //{
        //    vec4 onScreen(model->getBoundingBoxVertex(j), 1.0);
        //    onScreen = totalMatrix * onScreen;
        //    onScreen /= onScreen.w;
        //    //LogDebug << "Onscreen = " << onScreen << endLog;
        //    if(!(onScreen.x < -1.0 || onScreen.x > 1.0 || onScreen.y < -1.0 || onScreen.y > 1.0))
        //    {
        //        flag = true;
        //        break;
        //    }
        //}
        //if(flag == false) continue;

        int frame = 0;
        if (shader->isEnabled(UNIFORM_ANIM_INTERPOL))
        {
            float interpolation = 0.0f;
            if (auto animState = gr->getAnimationState())
            {
                frame = animState->getCurFrame();
                interpolation = animState->getInterpolation();
            }
            shader->setAnimInterpolation(interpolation);
        }

        for(auto mesh : model->getMeshes())
            renderer->renderMesh(mesh, shader, frame);
    }

    void Graphics::renderBillboard(BillboardGraphicsComponent* gr, Entity* e)
    {
        // Get the quad if we do not have it yet
        if (!quad2dGeometry)
        {
            shared_ptr<Model> a = Locator::getModelManager().getModel("quad2d");
            if (a->getMeshes().empty()) return;
            quad2dGeometry = a->getMeshes().front()->geometry;
            if (!quad2dGeometry) return;
        }

        Material* mat = gr->getMaterial();
        if(!mat) return;

        billboardShader->use();

        billboardShader->setMoveMatrix(gr->getMoveMatrix());
        billboardShader->setViewMatrix(camera->getVMatrix());
        billboardShader->setViewProjectionMatrix(camera->getVPMatrix());
        billboardShader->doUniforms(e);

        renderer->enableBlending(true);
        renderer->renderGeometry(quad2dGeometry.get(), mat, billboardShader.get());
        renderer->enableBlending(false);

        return;
    }
}
