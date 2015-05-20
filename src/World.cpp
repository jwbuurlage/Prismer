#include "World.h"
#include "Entity.h"
#include "Terrain.h"

namespace Arya
{
    World::World()
    {
        terrain = new Terrain;
        //skybox = new Skybox;
    }

    World::~World()
    {
        //delete skybox;
        delete terrain;
        // shared_ptr takes care of entity deletion
    }

    shared_ptr<Entity> World::createEntity()
    {
        shared_ptr<Entity> ent = make_shared<Entity>();
        entities.push_back(ent);
        return ent;
    }

    void World::removeEntity(shared_ptr<Entity> ent)
    {
        // Simply remove ent from the linked list
        // The shared_ptr will make sure the object
        // is deleted if needed
        for (EntityList::iterator e = entities.begin(); e != entities.end(); e++)
        {
            if (*e == ent)
            {
                entities.erase(e);
                break;
            }
        }
    }

    void World::update(float elapsedTime)
    {
        for (auto ent : entities) ent->update(elapsedTime);
    }
}
