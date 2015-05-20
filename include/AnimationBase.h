//See top of Models.h for more explanation
#pragma once
#include <string>
#include <memory>

namespace Arya
{
    using std::string;
    using std::unique_ptr;

    //
    // Base class for an animation state.
    // Entity holds a ModelGraphicsComponent
    // with a unique_ptr to a subclass of AnimationState
    // 
    // Subclasses of this state (in the cpp file) could be a float specifying
    // the percentage of the animation that is completed
    // The actual keyframe data (like timings or bone positions)
    // are in Model->animationData
    //
    class AnimationState
    {
        public:
            AnimationState(){}
            virtual ~AnimationState(){}

            virtual void setAnimation(string name) = 0;
            virtual void updateAnimation(float elapsedTime) = 0;

            //TODO:
            //the frame thing is actually only for vertex based animations
            //so we could move it to a subclass. But then Graphics
            //can not access it :/
            virtual int getCurFrame() = 0;
            virtual float getInterpolation() = 0;
            //For currently set animation:
            virtual float getAnimationTime() = 0;
            //Change speed of current animation
            virtual void setAnimationTime(float newTime) = 0;
    };

    //Base class for animation data
    //Model holds a unique_ptr to a subclass of this
    class AnimationData
    {
        public:
            AnimationData(){}
            virtual ~AnimationData(){}

            //! Create an AnimationState instance
            virtual unique_ptr<AnimationState> createAnimationState() const { return 0; }
    };
}
