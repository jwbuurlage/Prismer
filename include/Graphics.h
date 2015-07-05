#pragma once

#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Arya {

using std::shared_ptr;
using std::make_shared;
using glm::vec2;
using glm::mat4;

class View;
class World;
class Entity;
class Camera;
class Geometry;
class Renderer;
class Interface;
class RenderTarget;
class ShaderProgram;
class GraphicsComponent;
class ModelGraphicsComponent;
class BillboardGraphicsComponent;

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
        void render(Interface* interface);

        //! Update camera
        void update(float elapsed);

        //! Convert from x,y pixel coordinates with origin top left
        //! to [-1,1] coordinates with (-1,-1) bottom left
        vec2 normalizeMouseCoordinates(int x, int y);

        Renderer*       getRenderer() const { return renderer; }
        Camera*         getCamera() const { return camera; }

    private:
        Renderer*       renderer;
        Camera*         camera;
        int windowWidth;
        int windowHeight;
        vec2 inverseWindowSize;

        // directional light. NOT point source
        vec3 lightDirection; //normalized, points along the lightbeams in light direction, from light towards objects
        mat4 lightMatrix; //for rendering the scene as seen from the light. includes orthographic projection

        void makeLightMatrix();

        shared_ptr<RenderTarget> shadowRenderTarget;
        shared_ptr<ShaderProgram> billboardShader;
        shared_ptr<ShaderProgram> viewShader;
        shared_ptr<Geometry> quad2dGeometry;

        // The Entity is temporary untill shader-uniform-setting has
        // been moved into render()
        void renderView(View* view);
        void renderModel(ModelGraphicsComponent* gr, Entity* e, bool shadowPass);
        void renderBillboard(BillboardGraphicsComponent* gr, Entity* e);
};

} // namespace Arya
