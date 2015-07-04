#pragma once

#include <vector>
#include <memory>

namespace Prismer {

class FactionInput;
class GameSession;
class Unit;

using std::vector;
using std::weak_ptr;
using std::shared_ptr;

class Faction
{
    public:
        Faction(int id, weak_ptr<GameSession> session);
        ~Faction();

        void setInput(shared_ptr<FactionInput> input) {
            _input = input;
        }

        // turn logic
        void beginTurn();
        void endTurn();

        void addUnit(shared_ptr<Unit>& unit);

        vec3 getColor() const {
            return _color;
        }

    private:
        int _id;

        vec3 _color;

        vector<shared_ptr<Unit>> _units;
        shared_ptr<FactionInput> _input;
        weak_ptr<GameSession> _session;
};

} // namespace Prismer
