#include "ModelGraphicsComponent.h"
#include "Models.h"

namespace Arya
{
    ModelGraphicsComponent::ModelGraphicsComponent()
    {
        model = 0;
        scale = 1.0f;
    }

    ModelGraphicsComponent::~ModelGraphicsComponent()
    {
        //Release model and animation state
        setModel(0);
    }

    void ModelGraphicsComponent::setAnimation(const char* name)
    {
        if (animState) animState->setAnimation(name);
    }

    void ModelGraphicsComponent::updateAnimation(float elapsedTime)
    {
        if (animState) animState->updateAnimation(elapsedTime);
    }

    void ModelGraphicsComponent::setAnimationTime(float time)
    {
        if (animState) animState->setAnimationTime(time);
    }

    void ModelGraphicsComponent::setModel(shared_ptr<Model> newModel)
    {
        //Set new model and get a new animation state object
        //(subclass of AnimationState)
        model = newModel;

        animState.reset();
        if (model)
            animState = model->createAnimationState();
    }
}
