#pragma once

#include <memory>

namespace Prismer {

class Unit;
class TileInfo;

using std::shared_ptr;

// an ability is something that affects a certain tile.
// - gather resource
// - attack unit
// - build something on a tile
// - boost unit on a tile
// - ...
//
// do we want to allocate abilities for each unit?
// only necessary if they are stateful. can also 
// add argument to perform().
class Ability
{
    // TODO: think more
    // static GatherAbility = AGather();
    // static AttackAbility = AAttack();

    public:
        Ability() { } 
        

        virtual void perform(shared_ptr<TileInfo> tile,
                shared_ptr<Unit> actor) = 0;

        virtual bool isValid(shared_ptr<TileInfo> tile) = 0;
    private:
        // Animation? / Graphics?
};

} // namespace Prismer
