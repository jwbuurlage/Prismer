#include "GraphicsComponent.h"
#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Arya
{
    bool GraphicsComponent::isDirty() const
    {
        if (updateMatrix) return true;
        // This object is not dirty but maybe its parent is
        if (ent)
            if (auto parent = ent->getParent())
                if (auto gr = parent->getGraphics())
                    return gr->isDirty();
        return false;
    }

    const mat4& GraphicsComponent::getMoveMatrix()
    {
        if (isDirty() && ent) {
            updateMatrix = false;
            mMatrix = glm::translate(mat4(1.0f), ent->getPosition());
            mMatrix = glm::rotate(mMatrix, ent->getYaw(),   vec3(0.0, 0.0, 1.0)); //z-axis is up
            mMatrix = glm::rotate(mMatrix, ent->getPitch(), vec3(1.0, 0.0, 0.0));
            mMatrix = glm::scale(mMatrix, vec3(getScale()));

            if (auto parent = ent->getParent())
            {
                if (auto gr = parent->getGraphics())
                {
                    mMatrix = gr->getMoveMatrix() * mMatrix;
                }
            }
        }
        return mMatrix;
    }

}
