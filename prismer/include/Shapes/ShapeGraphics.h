#include "UnitGraphics.h"

#include <Arya.h>

namespace Prismer {

using std::shared_ptr;

class ShapeEntity
    : public UnitEntity
{
    public:
        ShapeEntity() : UnitEntity()
        { }

        ShapeEntity(shared_ptr<Unit> unit,
                shared_ptr<GridEntity> grid_entity)
            : UnitEntity(unit, grid_entity)
        { }

        virtual ~ShapeEntity() { }

    protected:

};

} // namespace Prismer
