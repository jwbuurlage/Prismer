#include "Entity.h"
#include "GraphicsComponent.h"
#include "ModelGraphicsComponent.h"
#include "BillboardGraphicsComponent.h"

namespace Arya
{
    Entity::Entity()
    {
        position = vec3(0);
        pitch = 0;
        yaw = 0;
    }

    Entity::~Entity()
    {
    }

    void Entity::update(float elapsedTime)
    {
        if (graphicsComponent) graphicsComponent->updateAnimation(elapsedTime);
    }

    void Entity::setGraphics(unique_ptr<GraphicsComponent> gr)
    {
        graphicsComponent = std::move(gr);
        graphicsComponent->setEntity(this);
    }

    void Entity::setGraphics(shared_ptr<Model> model)
    {
        //can not be done with make_unique because it is casted to its base class
        ModelGraphicsComponent* comp = new ModelGraphicsComponent;
        comp->setModel(model);
        comp->setEntity(this);
        graphicsComponent.reset(comp);
    }

    void Entity::setGraphics(shared_ptr<Material> material)
    {
        //can not be done with make_unique because it is casted to its base class
        BillboardGraphicsComponent* comp = new BillboardGraphicsComponent;
        comp->setMaterial(material);
        comp->setEntity(this);
        graphicsComponent.reset(comp);
    }

    void Entity::updateMatrix()
    {
        if (graphicsComponent)
            graphicsComponent->matrixDirty();
    }
}
