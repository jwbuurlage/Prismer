#pragma once

#include <vector>
#include <memory>

namespace Prismer {

class Unit;

using std::vector;
using std::shared_ptr;

class Faction
{
    public:
        Faction() { }
        ~Faction() = default;

        // turn logic
        void beginTurn();
        void endTurn() const;

    private:
        vector<shared_ptr<Unit>> _units;
};

} // namespace Prismer
