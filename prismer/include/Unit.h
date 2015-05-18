#include <Arya.h>

#include <memory>

namespace Prismer {

using std::shared_ptr;

class GameSession;

// unit info is stuff that (in general) does not change turn-to-turn.
// specific to a unit at creation
struct UnitInfo
{
    UnitInfo(int type_id_, Shape shape_) {
        type_id = type_id_;
        shape = shape_;
    }

    int type_id;
    Shape shape;

    // Abilities depend on Shape
    // strenth of abilities depend on colors..

    // these also depend on colors, how exactly depends on shape
//    int health_points;
//    int movement_points;

    // Colors colors;
    // Stats stats;
    //
    // First stats:
    // - movement info
    // - vision == movement?
};

class Unit
{
    public:
        Unit(int id,
            UnitInfo info,
            shared_ptr<GameSession> session) :
            _id(id),
            _info(info),
            _session(session);

        ~Unit() { }

        int getId() const {
            return _id;
        }

        void update(int gameTimer) const;

    private:
        int _id;
        UnitInfo _info;
        shared_ptr<GameSession> _session;
        shared_ptr<Arya::Entity> _entity;
};

} // namespace Prismer
