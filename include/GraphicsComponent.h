#pragma once

namespace Arya
{
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
            // RenderType determines the subclass of GraphicsComponent
            virtual RenderType getRenderType() const { return TYPE_NONE; }

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

}
