#include "common/Logger.h"
#include "Renderer.h"
#include "Models.h"
#include "Geometry.h"
#include "Materials.h"
#include "Shaders.h"
#include "Textures.h"
#include "Locator.h"

#include <GL/glew.h>

namespace Arya {

RenderTarget::RenderTarget()
{
    width = height = 0;
    frameBuffer = 0;
    texture = 0;
    depthBuffer = 0;
}

RenderTarget::~RenderTarget()
{
    if (depthBuffer)
        glDeleteTextures(1, &depthBuffer);
    if (texture)
        glDeleteTextures(1, &texture);
    if (frameBuffer)
        glDeleteFramebuffers(1, &frameBuffer);
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::init()
{
    checkErrors();

    glewExperimental = GL_TRUE; 
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LogError << "Failed to initialize Glew. Errormessage: " << glewGetErrorString(err) << endLog;
        return false;
    }
    // A known bug of GLEW / OpenGL is that glewInit causes an GL_INVALID_ENUM
    // even though everything is fine so we reset that error message here
    glGetError();

    if (!GLEW_VERSION_3_2)
        LogWarning << "No OpenGL 3.2 support! Continuing" << endLog;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

shared_ptr<RenderTarget> Renderer::createRenderTarget(int width, int height, bool color, bool depth)
{
    if (width <= 0 || height <= 0) return nullptr;
    if (!color && !depth) return nullptr;

    shared_ptr<RenderTarget> target = make_shared<RenderTarget>();
    target->width = width;
    target->height = height;

    glGenFramebuffers(1, &target->frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, target->frameBuffer);

    if (color)
    {
        glGenTextures(1, &target->texture);
        glBindTexture(GL_TEXTURE_2D, target->texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target->texture, 0);
    }

    if (depth)
    {
        // this can also be a render buffer instead of texture (code below)
        glGenTextures(1, &target->depthBuffer);
        glBindTexture(GL_TEXTURE_2D, target->depthBuffer);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target->depthBuffer, 0);

        //glGenRenderbuffers(1, &target->depthBuffer);
        //glBindRenderbuffer(GL_RENDERBUFFER, target->depthBuffer);
        //glRenderBufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        //glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, target->depthBuffer);
    }

    if (color)
    {
        // Set the list of draw buffers.
        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, drawBuffers);
    }
    else
    {
        glDrawBuffer(GL_NONE);
    }

    switch(glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        case GL_FRAMEBUFFER_COMPLETE:
            return target;
            break;

        case GL_FRAMEBUFFER_UNSUPPORTED:
            LogError << "Framebuffer is unsupported" << endLog;
            break;

        case GL_FRAMEBUFFER_UNDEFINED:
            LogError << "Framebuffer is undefined" << endLog;
            break;

        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LogError << "Framebuffer has incomplete attachment" << endLog;
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LogError << "Framebuffer is missing attachment" << endLog;
            break;

        case 0:
        default:
            LogError << "Unknown framebuffer error" << endLog;
            break;
    }
    // RenderBuffer object will automatically be deleted due to shared_ptr semantics
    return nullptr;
}

void Renderer::setRenderTarget(RenderTarget* target)
{
    if (target)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, target->frameBuffer);
        glViewport(0, 0, target->width, target->height);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void Renderer::setViewport(int width, int height)
{
    glViewport(0, 0, width, height);
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

} // namespace Arya
