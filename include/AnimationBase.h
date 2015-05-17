//See top of Models.h for more explanation
#pragma once
#include <string>
using std::string;

namespace Arya
{
    //
    // Base class for an animation state. Every Entity that has
    // a model that is animatable will have a (copy) of a specific
    // AnimationState subclass.
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
    //This is stored inside the model, not in the entity
    class AnimationData
    {
        public:
            AnimationData(){}
            virtual ~AnimationData(){}
    };
}
