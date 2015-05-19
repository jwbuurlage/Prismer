#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using glm::mat4;

namespace Arya
{
    class World;
    class Camera;
    class Renderer;
    class ShaderProgram;
    class GraphicsComponent;
    class ModelGraphicsComponent;

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

            void renderModel(ModelGraphicsComponent* gr, const mat4& moveMatrix);
    };
}
