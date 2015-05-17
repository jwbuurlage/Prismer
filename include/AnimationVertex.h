//Read Models.h for explanation of AnimationState vs AnimationData
#pragma once
#include "AnimationBase.h"
#include <vector>
#include <map>

using std::vector;
using std::map;

namespace Arya
{
    struct VertexAnim
    {
        int startFrame; //inclusive
        int endFrame; //inclusive
        vector<float> frameTimes; //size = end - start + 1
    };

    class VertexAnimationData : public AnimationData
    {
        public:
            map<string, VertexAnim> animations;
    };

    class VertexAnimationState : public AnimationState
    {
        public:
            VertexAnimationState(VertexAnimationData* data);
            ~VertexAnimationState();

            //Base class overloads
            void setAnimation(string name);

            void updateAnimation(float elapsedTime);

            int getCurFrame(){ return startFrame + curFrame; }
            float getInterpolation(){ return interpolation; }

            float getAnimationTime();
            void setAnimationTime(float newTime);

        private:
            VertexAnimationData* animData;

            VertexAnim* curAnim; //one of the animations in animData

            int startFrame;
            int curFrame; //current frame RELATIVE TO STARTFRAME
            float timer;
            float interpolation; //in range [0,1]
            float speedFactor;
    };

}
