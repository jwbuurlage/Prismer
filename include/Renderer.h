#pragma once

namespace Arya
{
    class Mesh;
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

            //! Render a piece of geometry with texture
            void renderMesh(Mesh* mesh, int frame, ShaderProgram* shader);
    };
}
