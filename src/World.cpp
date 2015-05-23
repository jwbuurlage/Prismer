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
    }

    void World::update(float elapsedTime)
    {
        // Remove expired entities from the list while updating
        shared_ptr<Entity> ent;
        for (auto e = entities.begin(); e != entities.end();)
        {
            ent = e->lock();
            if (!ent)
            {
                e = entities.erase(e);
            }
            else
            {
                ent->update(elapsedTime);
                e++;
            }
        }
    }

    void World::addEntity(weak_ptr<Entity> e)
    {
        entities.push_back(e);
    }
}
