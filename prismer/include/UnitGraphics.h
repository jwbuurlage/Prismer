#include <memory>

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
        void update() = delete;

    private:
        shared_ptr<Unit> _unit;
        shared_ptr<GridEntity> _grid_entity;
};

} // namespace Prismer