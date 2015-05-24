#include <Arya.h>

#include "Shapes/Shape.h"
#include "GameLogger.h"

#include <memory>

namespace Prismer {

using std::shared_ptr;
using std::weak_ptr;

class GameSession;
class UnitEntity;
class Tile;
enum ColorID;

// unit info is stuff that (in general) does not change turn-to-turn.
// specific to a unit at creation
struct UnitInfo
{
    UnitInfo(Shape shape_)
        : shape(shape_)
    { }

    Shape shape;
};

class Unit
    : public std::enable_shared_from_this<Unit>
{
    public:
        Unit(int id,
            UnitInfo info,
            shared_ptr<GameSession> session);
        ~Unit() { }

        int getId() const {
            return _id;
        }

        void update(float dt, float t);

        void addColor(ColorID color);

        const Shape& getShape() const {
            return _info.shape;
        };

        void setEntity(shared_ptr<UnitEntity> entity) {
            _entity = entity;
        }

        int getX() const {
            return _x;
        }

        int getY() const {
            return _y;
        }

        int getMovePoints() const {
            return _mp;
        }

        float getSpeed() const {
            return (float)_mp;
        }

        void activate(shared_ptr<GridInput> grid_input);
        void deactivate();

        void setTile(shared_ptr<Tile> tile);

        weak_ptr<Tile> getTile() {
            return _tile;
        }

    private:
        int _id;
        int _x = 0;
        int _y = 0;
        int _mp = 2;

        UnitInfo _info;
        shared_ptr<GameSession> _session;
        shared_ptr<UnitEntity> _entity;
        weak_ptr<Tile> _tile;
};

} // namespace Prismer
