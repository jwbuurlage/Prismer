#include "GraphicsComponent.h"
#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arya
{
    const mat4& GraphicsComponent::getMoveMatrix()
    {
        if (updateMatrix && ent) {
            updateMatrix = false;
            mMatrix = glm::translate(mat4(1.0f), ent->getPosition());
            mMatrix = glm::rotate(mMatrix, ent->getYaw(),   vec3(0.0, 0.0, 1.0)); //z-axis is up
            mMatrix = glm::rotate(mMatrix, ent->getPitch(), vec3(1.0, 0.0, 0.0));
            mMatrix = glm::scale(mMatrix, vec3(getScale()));
        }
        return mMatrix;
    }

}
