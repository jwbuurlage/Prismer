#include <Arya.h>

#include <memory>

namespace Prismer {

using std::shared_ptr;

class GameSession;

struct UnitInfo
{
    UnitInfo(int type_id_) {
        type_id = type_id_;
    }

    int type_id;
};

class Unit
{
    public:
        Unit(int id,
            UnitInfo info,
            shared_ptr<GameSession> session) :
            _id(id),
            _info(info),
            _session(session)
        {}

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
