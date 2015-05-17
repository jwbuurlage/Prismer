#include "common/Logger.h"
#include "Renderer.h"
#include "Models.h"
#include "Geometry.h"
#include "Materials.h"
#include "Shaders.h"
#include "Textures.h"

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

        return true;
    }

    void Renderer::clear(int width, int height)
    {
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, width, height);
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::renderMesh(Mesh* mesh, int frame, ShaderProgram* shader)
    {
        if (!mesh->getGeometry()) return;
        if (mesh->getGeometry()->frameCount <= frame) return;

        glActiveTexture(GL_TEXTURE0);
        shader->setUniform1i("tex", 0);

        Material* mat = mesh->getMaterial();
        if (mat && mat->texture) {
            glBindTexture(GL_TEXTURE_2D, mat->texture->handle);
            shader->setUniform4fv("parameters", vec4(mat->specAmp,mat->specPow,mat->ambient,mat->diffuse));
        }
        mesh->getGeometry()->draw(frame);
    }

}
