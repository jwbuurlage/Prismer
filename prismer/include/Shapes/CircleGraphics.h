#include "ShapeGraphics.h"

namespace Prismer {

using std::shared_ptr;

class CircleEntity
    : public ShapeEntity
{
    public:
        CircleEntity(shared_ptr<Unit> unit,
                shared_ptr<GridEntity> grid_entity);

        virtual ~CircleEntity() { }

         // unit state changed; update appearance
        virtual void updateState() override;
        virtual void update(float dt, float t) override;
};

} // namespace Prismer
