#include <memory>

#include <Arya.h>

namespace Prismer {

using std::shared_ptr;

class Unit;
class GridEntity;

class UnitEntity
{
    public:
        UnitEntity() { };

        UnitEntity(shared_ptr<Unit> unit,
                shared_ptr<GridEntity> grid_entity);

         // unit state changed; update appearance
        void updateState();
        void update(float dt, float t);

    private:
        shared_ptr<Unit> _unit;
        shared_ptr<GridEntity> _grid_entity;
        shared_ptr<Arya::Entity> _entity;

        vec3 _current_position;
};

} // namespace Prismer
