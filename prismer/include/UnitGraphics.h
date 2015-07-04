#include <memory>

#include <Arya.h>

namespace Prismer {

using std::shared_ptr;

class Unit;
class GridEntity;

class UnitEntity
{
    public:
        UnitEntity() { }

        UnitEntity(shared_ptr<Unit> unit,
            shared_ptr<GridEntity> grid_entity)
            : _unit(unit), _grid_entity(grid_entity) { }

        virtual ~UnitEntity() { }

         // unit state changed; update appearance
        virtual void updateState() = 0;
        virtual void update(float dt, float t) = 0;

        virtual void setTintColor(vec3 tintColor) {
            _tintColor = tintColor;
        }

    protected:
        shared_ptr<Unit> _unit;
        shared_ptr<GridEntity> _grid_entity;
        shared_ptr<Arya::Entity> _entity;

        vec3 _current_position;
        vec3 _tintColor;
        float _current_yaw;
};

} // namespace Prismer
