#pragma once

#include <list>
#include <memory>

namespace Arya
{
    using std::list;
    using std::shared_ptr;
    using std::weak_ptr;
    using std::make_shared;

    class Entity;
    class Terrain;
    class Skybox;

    class World
    {
        public:
            World();
            ~World();

            // World holds a list of weak pointers to entities
            // The user is responsible for maintaining the shared_ptr objects
            // The user can delete an entity by simply resetting the shared_ptr
            typedef list< weak_ptr<Entity> > EntityList;

            const EntityList& getEntities() const { return entities; }
            Terrain*      getTerrain() const { return terrain; }
            Skybox*       getSkybox() const { return skybox; }

            void update(float elapsedTime);
        private:
            EntityList  entities;
            Terrain*    terrain;
            Skybox*     skybox;

            friend class Entity;
            void addEntity(weak_ptr<Entity> e);
    };
}
