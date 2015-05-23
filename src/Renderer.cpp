#include "common/Logger.h"
#include "Renderer.h"
#include "Models.h"
#include "Geometry.h"
#include "Materials.h"
#include "Shaders.h"
#include "Textures.h"
#include "Locator.h"

#include <GL/glew.h>

namespace Arya
{
    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    bool Renderer::init()
    {
        glewExperimental = GL_TRUE; 
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            LogError << "Failed to initialize Glew. Errormessage: " << glewGetErrorString(err) << endLog;
            return false;
        }

        if (!GLEW_VERSION_3_1)
            LogWarning << "No OpenGL 3.1 support! Continuing" << endLog;

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void Renderer::checkErrors()
    {
        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            LogError << "OpenGL error: ";
            switch (err)
            {
                case GL_INVALID_ENUM:
                    LogError << "invalid enum";
                    break;
                case GL_INVALID_VALUE:
                    LogError << "invalid value";
                    break;
                case GL_INVALID_OPERATION:
                    LogError << "invalid operation";
                    break;
                default:
                    LogError << "unknown error";
                    break;
            }
            LogError << endLog;
        }
    }

    void Renderer::clear(int width, int height)
    {
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::enableBlending(bool enable)
    {
        if (enable)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    void Renderer::enableDepthWrite(bool enable)
    {
        glDepthMask(enable ? GL_TRUE : GL_FALSE);
    }

    void Renderer::enableDepthTest(bool enable)
    {
        glDepthFunc(enable ? GL_LESS : GL_ALWAYS);
    }

    void Renderer::renderMesh(Mesh* mesh, ShaderProgram* shader, int frame)
    {
        if (!mesh->geometry) return;

        shared_ptr<Material> mat = mesh->material;
        if (!mat || !mat->texture)
            mat = Locator::getMaterialManager().getMaterial("default");

        renderGeometry(mesh->geometry.get(), mat.get(), shader, frame);
    }

    void Renderer::renderGeometry(Geometry* geom, Material* mat, ShaderProgram* shader, int frame)
    {
        if (frame > geom->frameCount) return;
        glActiveTexture(GL_TEXTURE0);
        shader->setTexture(0);
        glBindTexture(GL_TEXTURE_2D, mat->texture->handle);
        shader->setMaterialParams(vec4(mat->specAmp,mat->specPow,mat->ambient,mat->diffuse));

        geom->draw(frame);
    }
}
