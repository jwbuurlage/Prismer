#pragma once
#include <memory>

using std::shared_ptr;
using std::make_shared;

typedef unsigned int GLuint;

namespace Arya {

class Mesh;
class Geometry;
class Material;
class ShaderProgram;

class RenderTarget
{
    public:
        RenderTarget();
        ~RenderTarget();

        // Only to be used by Renderer
        int width, height;
        GLuint frameBuffer;
        GLuint texture; //0 if no color
        GLuint depthBuffer; //0 if no depth
};

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        //! Initialize GLEW
        bool init();

        void checkErrors();

        //! Clear the screen
        void clear(int width, int height);

        void enableBlending(bool enable = true);

        //! Note that it IS possible to write to the depth buffer without testing
        //! and also to do the depth test without writing (updating)
        void enableDepthWrite(bool enable = true);
        void enableDepthTest(bool enable = true);

        //! Create a render target:
        //! framebuffer and possible texture and depth buffer
        shared_ptr<RenderTarget> createRenderTarget(int width, int height, bool color, bool depth);

        //! Set the render target
        //! Zero means screen
        //! If target is screen, then MUST call Renderer::setViewport or Renderer::clear afterwards
        //! for the viewport size to be correct
        void setRenderTarget(RenderTarget* target = 0);

        //! Set viewport after rendertarget has been set to screen
        void setViewport(int width, int height);

        //! Render a piece of geometry with texture
        //! Wrapper for lower-level renderGeometry
        //! Assumes mesh, shader are valid pointers
        void renderMesh(Mesh* mesh, ShaderProgram* shader, int frame = 0);

        //! Assumes that geom, mat, shader are valid pointers
        //! and mat has a valid texture handle
        void renderGeometry(Geometry* geom, Material* mat, ShaderProgram* shader, int frame = 0);
};
}
