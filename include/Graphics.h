#pragma once

namespace Arya
{
    class World;
    class Renderer;
    class Camera;
    class ShaderProgram;
    class GraphicsComponent;

    class Graphics
    {
        public:
            Graphics();
            ~Graphics();

            //! Initialize GLEW, a default shader and
            //! set the camera projection matrix
            bool init(int width, int height);

            //! Recompute the camera projection matrix
            void resize(int width, int height);

            //! Clear the screen
            void clear(int width, int height);

            //TODO: Should this be world* or some list of RenderSpec's
            //The last case seems appropriate so that Graphics does not
            //know about World, only a list of renderable things
            void render(World* world);

            //! Update camera
            void update(float elapsed);

            Renderer*       getRenderer() const { return renderer; }
            Camera*         getCamera() const { return camera; }

        private:
            Renderer*       renderer;
            Camera*         camera;

            //TODO: This should be replaced
            //Shaders should be in renderspec
            ShaderProgram*  defaultShader;

            void renderModel(GraphicsComponent* gr);
    };
}
