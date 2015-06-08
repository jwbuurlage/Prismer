#include "ShapeGraphics.h"

namespace Prismer {

using std::shared_ptr;

class TriangleEntity
    : public ShapeEntity
{
    public:
        TriangleEntity(shared_ptr<Unit> unit,
                shared_ptr<GridEntity> grid_entity);

        virtual ~TriangleEntity() { }

         // unit state changed; update appearance
        virtual void updateState() override;
        virtual void update(float dt, float t) override;
};

} // namespace Prismer
