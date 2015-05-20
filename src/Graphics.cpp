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
#include "Locator.h"

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

        resize(width, height);

        billboardShader = make_shared<ShaderProgram>(
                "../shaders/billboard.vert",
                "../shaders/billboard.frag");
        if (!billboardShader->isValid()) {
            billboardShader = nullptr;
            LogError << "Could not load billboard shader." << endLog;
            return false;
        }

        return true;
    }

    void Graphics::resize(int width, int height)
    {
        if(height > 0)
            camera->setProjectionMatrix(45.0f, ((float)width)/((float)height), 0.1f, 2000.0f);
    }

    void Graphics::clear(int width, int height)
    {
        renderer->clear(width, height);
    }

    void Graphics::render(World* world)
    {
        auto entities = world->getEntities();
        for(auto ent : entities) {
            GraphicsComponent* gr = ent->getGraphics();
            if (!gr) continue;

            RenderType type = gr->getRenderType();
            switch(type) {
                case TYPE_MODEL:
                    renderModel((ModelGraphicsComponent*)gr);
                    break;
                case TYPE_TERRAIN:
                    break;
                case TYPE_BILLBOARD:
                    renderBillboard((BillboardGraphicsComponent*)gr);
                    break;
                default:
                    break;
            }
        }
        return;
    }

    void Graphics::update(float elapsed)
    {
        camera->update(elapsed);
    }

    void Graphics::renderModel(ModelGraphicsComponent* gr)
    {
        Model* model = gr->getModel();
        if(!model) return;
        ShaderProgram* shader = model->getShaderProgram();
        if(!shader) return;

        shader->use();
        shader->setUniformMatrix4fv("vpMatrix", camera->getVPMatrix());
        shader->setUniformMatrix4fv("viewMatrix", camera->getVMatrix());
        shader->setUniformMatrix4fv("mMatrix", gr->getMoveMatrix());
        shader->setUniform3fv("tintColor", vec3(0.5, 1.0, 0.5));

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
        float interpolation = 0.0f;
        AnimationState* animState = gr->getAnimationState();
        if(animState)
        {
            frame = animState->getCurFrame();
            interpolation = animState->getInterpolation();
        }
        shader->setUniform1f("interpolation", interpolation);

        for(auto mesh : model->getMeshes())
            renderer->renderMesh(mesh, shader, frame);
    }

    void Graphics::renderBillboard(BillboardGraphicsComponent* gr)
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
        billboardShader->setUniformMatrix4fv("vpMatrix", camera->getVPMatrix());
        billboardShader->setUniformMatrix4fv("viewMatrix", camera->getVMatrix());
        billboardShader->setUniformMatrix4fv("mMatrix", gr->getMoveMatrix());
        billboardShader->setUniform2fv("screenOffset", gr->getScreenOffset());
        billboardShader->setUniform2fv("screenSize", gr->getScreenSize());

        renderer->enableBlending(true);
        renderer->renderGeometry(quad2dGeometry.get(), mat, billboardShader.get());
        renderer->enableBlending(false);

        return;
    }
}
