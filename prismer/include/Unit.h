#include <Arya.h>

#include "Shapes/Shape.h"
#include "GameLogger.h"

#include <memory>

namespace Prismer {

using std::shared_ptr;

class GameSession;
class UnitEntity;
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

        void update(int gameTimer) const;

        void addColor(ColorID color);

        const Shape& getShape() const {
            return _info.shape;
        };

        void cast(int abilityId, shared_ptr<TileInfo> tile) {
            auto shape = getShape();
            auto ability = shape.getAbilities().front();
            if (!ability) {
                GameLogInfo << "No castable ability" << endLog;
            }
            else if (!ability->isValid(tile)) {
                GameLogInfo << "Ability is invalid" << endLog;
            } else {
                GameLogInfo << "Performing ability" << endLog;
                ability->perform(tile, shared_from_this());
            }
        }

        void setEntity(shared_ptr<UnitEntity> entity) {
            _entity = entity;
        }

        int getX() const {
            return _x;
        }

        int getY() const {
            return _y;
        }

        void setX(int x) {
            _x = x;
        }

        void setY(int y) {
            _y = y;
        }

    private:
        int _id;
        int _x = 0;
        int _y = 0;

        UnitInfo _info;
        shared_ptr<GameSession> _session;
        shared_ptr<UnitEntity> _entity;
};

} // namespace Prismer
