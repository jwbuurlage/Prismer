#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

namespace Arya
{
    using glm::vec2;
    using glm::mat4;

    class AnimationState;
    class Entity;

    enum RenderType
    {
        TYPE_NONE = 0,
        TYPE_MODEL,
        TYPE_TERRAIN,
        TYPE_BILLBOARD
    };

    class GraphicsComponent
    {
        public:
            GraphicsComponent() { updateMatrix = true; }
            virtual ~GraphicsComponent() {}
            // RenderType determines the subclass of GraphicsComponent
            virtual RenderType getRenderType() const { return TYPE_NONE; }

            // Get the move matrix based on Entity position, orientation and graphics scale
            const mat4& getMoveMatrix();
            // Little hack for dirty matrix optimization
            void setEntity(Entity* e) { ent = e; }
            inline void matrixDirty() { updateMatrix = true; }

            // Subclasses can choose which of these they actually implement

            virtual void setScale(float /* scale */) { return; }
            virtual float getScale() const { return 1.0; }

            virtual void setScreenOffset(const vec2& /* offset */) { return; }
            virtual vec2 getScreenOffset() const { return vec2(0,0); }

            //! Set the animation
            virtual void setAnimation(const char* /* name */) { return; }

            //! Call this every frame to let the animation run
            virtual void updateAnimation(float /* elapsedTime */) { return; }

            //! Set the animation time for the currently set animation
            //! Meaning it will speedup or slowdown the animation so that
            //! it finishes in the specified time
            //! Usefull for making attack animations depend on attack speed
            virtual void setAnimationTime(float /* time */) { return; }

        private:
            Entity* ent;
            mat4 mMatrix; //cached version of position, pitch, yaw, graphics.Scale
            bool updateMatrix;
    };

}
