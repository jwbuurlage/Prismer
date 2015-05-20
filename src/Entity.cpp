#include "Entity.h"
#include "GraphicsComponent.h"
#include "ModelGraphicsComponent.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arya
{
    Entity::Entity()
    {
        position = vec3(0);
        pitch = 0;
        yaw = 0;
        updateMatrix = true;
    }

    Entity::~Entity()
    {
    }

    const mat4& Entity::getMoveMatrix()
    {
        if (updateMatrix) {
            updateMatrix = false;
            mMatrix = glm::translate(mat4(1.0f), position);
            mMatrix = glm::rotate(mMatrix, yaw,   vec3(0.0, 0.0, 1.0)); //z-axis is up
            mMatrix = glm::rotate(mMatrix, pitch, vec3(1.0, 0.0, 0.0));
            float scale = (graphicsComponent ? graphicsComponent->getScale() : 1.0f);
            mMatrix = glm::scale(mMatrix, vec3(scale));
        }
        return mMatrix;
    }

    void Entity::update(float elapsedTime)
    {
        if (graphicsComponent) graphicsComponent->updateAnimation(elapsedTime);
    }

    void Entity::setGraphics(unique_ptr<GraphicsComponent> gr)
    {
        graphicsComponent = std::move(gr);
    }

    void Entity::setGraphics(shared_ptr<Model> model)
    {
        ModelGraphicsComponent* comp = new ModelGraphicsComponent;
        comp->setModel(model);
        graphicsComponent.reset(comp);
    }

}
