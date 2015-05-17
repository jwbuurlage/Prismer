#include "World.h"
#include "Entity.h"
#include "EntitySystem.h"
#include "Terrain.h"

namespace Arya
{
    World::World()
    {
        entitySystem = new EntitySystem;
        terrain = new Terrain;
        //skybox = new Skybox;
    }

    World::~World()
    {
        //delete skybox;
        delete terrain;
        delete entitySystem;
    }

    void World::update(float elapsedTime)
    {
        const list<Entity*> ents = entitySystem->getEntities();
        for(auto ent : ents) ent->update(elapsedTime);
    }
}
