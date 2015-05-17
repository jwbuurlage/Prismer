#include "AnimationVertex.h"
#include "common/Logger.h"

namespace Arya
{
    VertexAnimationState::VertexAnimationState(VertexAnimationData* data)
    {
        animData = data;
        curAnim = 0;
        startFrame = 0;
        curFrame = 0;
        timer = 0.0f;
        interpolation = 0.0f;
        speedFactor = 1.0f;
    }

    VertexAnimationState::~VertexAnimationState()
    {
    }

    //Base class overloads
    void VertexAnimationState::setAnimation(string name)
    {
        if(!animData) return;
        auto anim = animData->animations.find(name);
        if(anim == animData->animations.end())
        {
            LogWarning << "Animation not found: " << name << endLog;
            return;
        }
        if(curAnim == &anim->second) return;
        curAnim = &anim->second;
        startFrame = curAnim->startFrame;
        curFrame = 0;
        timer = 0.0f;
        interpolation = 0.0f;
        speedFactor = 1.0f;
    }

    void VertexAnimationState::updateAnimation(float elapsedTime)
    {
        if(!curAnim) return;

        timer += elapsedTime * speedFactor;

        //Advance frames
        //It is a loop because it is possible to skip more
        //frames if frametimes are short, or the update is
        //over a large time
        while(timer > curAnim->frameTimes[curFrame])
        {
            timer -= curAnim->frameTimes[curFrame];
            curFrame++;
            if(curFrame > (curAnim->endFrame - curAnim->startFrame)) curFrame = 0;
        }
        interpolation = timer / curAnim->frameTimes[curFrame];
    }


    float VertexAnimationState::getAnimationTime()
    {
        if(!curAnim) return 0.0f;

        float time = 0.0f;
        for(int i = 0; i <= (curAnim->endFrame - curAnim->startFrame); ++i)
        {
            time += curAnim->frameTimes[i];
        }
        return time;
    }

    void VertexAnimationState::setAnimationTime(float newTime)
    {
        if(newTime > 0.0001)
            speedFactor = getAnimationTime() / newTime;
    }
};


