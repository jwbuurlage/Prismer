#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::mat4;

namespace Arya
{
    class Model;
    class AnimationState;

    enum RenderType
    {
        TYPE_NONE = 0,
        TYPE_MODEL,
        TYPE_TERRAIN
    };

    class GraphicsComponent
    {
        public:
            GraphicsComponent() {}
            virtual ~GraphicsComponent() {}
            virtual RenderType getRenderType() const { return TYPE_NONE; }

            virtual Model* getModel() const { return 0; }
            virtual AnimationState* getAnimationState() const { return 0; }

            //! Set the animation
            virtual void setAnimation(const char* /* name */) { return; }

            //! Call this every frame to let the animation run
            virtual void updateAnimation(float /* elapsedTime */) { return; }

            //! Set the animation time for the currently set animation
            //! Meaning it will speedup or slowdown the animation so that
            //! it finishes in the specified time
            //! Usefull for making attack animations depend on attack speed
            virtual void setAnimationTime(float /* time */) { return; }
    };

    class ModelGraphicsComponent : public GraphicsComponent
    {
        public:
            ModelGraphicsComponent();
            ~ModelGraphicsComponent();
            RenderType getRenderType() const override { return TYPE_MODEL; }

            Model* getModel() const override { return model; }
            AnimationState* getAnimationState() const override { return animState; }
            void setAnimation(const char* name) override;
            void updateAnimation(float elapsedTime) override;
            void setAnimationTime(float time) override;

            //! setModel releases the old model and animationstate.
            //! If the new model is nonzero,
            //! it creates a new AnimationState object
            void setModel(Model* model);

        private:
            Model* model;
            AnimationState* animState;
    };

    class Entity
    {
        private:
            //TODO: is this ok?
            //Only EntitySystem can create entities
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

            //! Creates a ModelGraphicsComponent with the specified model
            void setModel(Model* model);

            //! Get the graphics component (can be zero)
            GraphicsComponent* getGraphics() const { return graphicsComponent; }

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
