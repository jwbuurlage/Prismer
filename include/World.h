#pragma once

#include <list>
#include <memory>

namespace Arya
{
    using std::list;
    using std::shared_ptr;
    using std::make_shared;

    class Entity;
    class Terrain;
    class Skybox;

    class World
    {
        public:
            World();
            ~World();

            typedef list< shared_ptr<Entity> > EntityList;

            //! Creates an entity and stores a reference to it
            //! in the entity list
            shared_ptr<Entity> createEntity();

            //! Removes the reference that World has to entity
            //! Entity object gets deleted only after all other
            //! references are gone
            void removeEntity(shared_ptr<Entity> ent);

            const EntityList& getEntities() const { return entities; }
            Terrain*      getTerrain() const { return terrain; }
            Skybox*       getSkybox() const { return skybox; }

            void update(float elapsedTime);
        private:
            EntityList  entities;
            Terrain*    terrain;
            Skybox*     skybox;
    };
}
