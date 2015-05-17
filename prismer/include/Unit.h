#include <Arya.h>

#include <memory>

namespace Prismer {

using std::shared_ptr;

struct UnitInfo
{
    int type_id;
    // ...
};

class GameSession;

class Unit
{
    public:
        Unit(int id,
            UnitInfo info,
            shared_ptr<GameSession> session) {
            _id = id;
            _info = info;
            _session = session;
        }
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
