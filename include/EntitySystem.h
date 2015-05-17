#pragma once

#include <list>
using std::list;

namespace Arya
{
    class Entity;

    class EntitySystem
    {
        public:
            EntitySystem();
            ~EntitySystem();

            Entity* createEntity();

            const list<Entity*>& getEntities() const { return entities; }
        private:

            list<Entity*> entities;
    };
}
