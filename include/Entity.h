#pragma once

#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Arya
{
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;
    using glm::vec2;
    using glm::vec3;
    using glm::mat4;

    class Model;
    class Material;
    class Primitive;
    class GraphicsComponent;

    //! The user can subclass EntityUserData
    class EntityUserData {};

    class Entity
    {
        private:
            // For details on this, see
            // http://stackoverflow.com/questions/8147027/how-do-i-call-stdmake-shared-on-a-class-with-only-protected-or-private-const
            struct this_is_private {};
            Entity(const Entity&) = delete;
            const Entity& operator =(const Entity&) = delete;

        public:
            explicit Entity(const this_is_private&);
            ~Entity();

            //! Create an entity and add a weak reference to it in World
            static shared_ptr<Entity> create();

            inline const vec3& getPosition() const { return position; }
            inline vec2 getPosition2() const { return vec2(position.x, position.y); }
            inline float getPitch() const { return pitch; }
            inline float getYaw() const { return yaw; }

            inline void setPosition(const vec3& pos) { position = pos; updateMatrix(); }
            inline void setPitch(float p) { pitch = p; updateMatrix(); }
            inline void setYaw(float y) { yaw = y; updateMatrix(); }

            inline void setParent(shared_ptr<Entity> ent) { parent = ent; }
            inline shared_ptr<Entity> getParent() const { return parent.lock(); }

            //! Updates all components
            void update(float elapsedTime);

            //! Get the graphics component (can be zero)
            GraphicsComponent* getGraphics() const { return graphicsComponent.get(); }

            //! Set the graphics component (can be zero)
            //! If another graphics component was set, it will be deleted
            void setGraphics(unique_ptr<GraphicsComponent> gr);

            //! Creates a ModelGraphicsComponent with the specified model
            void setGraphics(shared_ptr<Model> model);

            //! Creates a BillboardGraphicsComponent with the specified material
            void setGraphics(shared_ptr<Material> material);

            void setUserData(EntityUserData* data) { userData = data; }
            EntityUserData* getUserData() const { return userData; }

        private:
            //cached movematrix version of position, pitch, yaw, scale is in GraphicsComponent
            vec3 position;
            float pitch;
            float yaw;

            EntityUserData* userData;
            weak_ptr<Entity> parent;

            void updateMatrix();

            //Components
            unique_ptr<GraphicsComponent> graphicsComponent;
    };
}
