#include "Entity.h"
#include "Locator.h"
#include "World.h"
#include "GraphicsComponent.h"
#include "ModelGraphicsComponent.h"
#include "BillboardGraphicsComponent.h"

namespace Arya
{
    Entity::Entity(const Entity::this_is_private&)
    {
        position = vec3(0);
        pitch = 0;
        yaw = 0;
        userData = 0;
    }

    Entity::~Entity()
    {
    }

    shared_ptr<Entity> Entity::create()
    {
        shared_ptr<Entity> e = make_shared<Entity>(this_is_private{});
        Locator::getWorld().addEntity(e);
        return e;
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
            graphicsComponent->setDirty();
    }
}
