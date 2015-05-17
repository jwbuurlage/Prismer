#include "Entity.h"
#include "Models.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arya
{
    Entity::Entity()
    {
        position = vec3(0);
        pitch = 0;
        yaw = 0;
        updateMatrix = true;

        graphicsComponent = 0;
    }

    Entity::~Entity()
    {
        if (graphicsComponent) delete graphicsComponent;
    }

    const mat4& Entity::getMoveMatrix()
    {
        if (updateMatrix) {
            updateMatrix = false;
            mMatrix = glm::translate(mat4(1.0f), position);
            mMatrix = glm::rotate(mMatrix, yaw,   vec3(0.0, 0.0, 1.0)); //z-axis is up
            mMatrix = glm::rotate(mMatrix, pitch, vec3(1.0, 0.0, 0.0));
        }
        return mMatrix;
    }

    void Entity::update(float elapsedTime)
    {
        if (graphicsComponent) graphicsComponent->updateAnimation(elapsedTime);
    }

    void Entity::setModel(Model* model)
    {
        if (graphicsComponent) delete graphicsComponent;
        ModelGraphicsComponent* comp = new ModelGraphicsComponent;
        comp->setModel(model);
        graphicsComponent = comp;
    }

    //TODO: Separate GraphicsComponent file
    ModelGraphicsComponent::ModelGraphicsComponent()
    {
        model = 0;
        animState = 0;
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

    void ModelGraphicsComponent::setModel(Model* newModel)
    {
        if (model) model->release();
        if (animState) delete animState;
        animState = 0;

        //Set new model and get a new animation state object
        //(subclass of AnimationState)
        model = newModel;
        if (model) {
            animState = model->createAnimationState();
            model->addRef();
        }
    }
}
