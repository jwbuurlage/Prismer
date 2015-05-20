#pragma once

namespace Arya
{
    class Mesh;
    class Geometry;
    class Material;
    class ShaderProgram;

    class Renderer
    {
        public:
            Renderer();
            ~Renderer();

            //! Initialize GLEW
            bool init();

            //! Clear the screen
            void clear(int width, int height);

            void enableBlending(bool enable = true);

            //! Render a piece of geometry with texture
            //! Wrapper for lower-level renderGeometry
            //! Assumes mesh, shader are valid pointers
            void renderMesh(Mesh* mesh, ShaderProgram* shader, int frame = 0);

            //! Assumes that geom, mat, shader are valid pointers
            //! and mat has a valid texture handle
            void renderGeometry(Geometry* geom, Material* mat, ShaderProgram* shader, int frame = 0);
    };
}
