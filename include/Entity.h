#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace Arya
{
    class Model;
    class Primitive;
    class GraphicsComponent;

    class Entity
    {
        private:
            //Private constructor so that only EntitySystem can create entities
            friend class EntitySystem;
            Entity();
            ~Entity();

        public:
            inline const vec3& getPosition() const { return position; }
            inline vec2 getPosition2() const { return vec2(position.x, position.y); }
            inline float getPitch() const { return pitch; }
            inline float getYaw() const { return yaw; }

            inline void setPosition(const vec3& pos) { position = pos; updateMatrix = true; }
            inline void setPitch(float p) { pitch = p; updateMatrix = true; }
            inline void setYaw(float y) { yaw = y; updateMatrix = true; }

            const mat4& getMoveMatrix();

            //! Updates all components
            void update(float elapsedTime);

            //! Get the graphics component (can be zero)
            GraphicsComponent* getGraphics() const { return graphicsComponent; }

            //! Set the graphics component (can be zero)
            //! If another graphics component was set, it will be deleted
            void setGraphics(GraphicsComponent* gr);

            //! Creates a ModelGraphicsComponent with the specified model
            void setGraphics(Model* model);

        private:
            vec3 position;
            float pitch;
            float yaw;

            mat4 mMatrix; //cached version of position,pitch,yaw
            bool updateMatrix;

            //Components
            GraphicsComponent* graphicsComponent;
    };
}
