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

        virtual ~UnitEntity() { }

         // unit state changed; update appearance
        virtual void updateState();
        virtual void update(float dt, float t);

    protected:
        shared_ptr<Unit> _unit;
        shared_ptr<GridEntity> _grid_entity;
        shared_ptr<Arya::Entity> _entity;

        vec3 _current_position;
        float _current_yaw;
};

} // namespace Prismer
