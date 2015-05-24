#pragma once

#include <memory>

namespace Prismer {

class Unit;
class GridInput;

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
        
        virtual void perform() = 0;
        virtual bool isValid() = 0;

        virtual void activate(shared_ptr<Unit> actor,
                shared_ptr<GridInput> grid_input) {
            _actor = actor;    
            _grid_input = grid_input;
        };

        virtual void deactivate() = 0;
        
    protected:
        shared_ptr<GridInput> _grid_input;
        shared_ptr<Unit> _actor;
};

} // namespace Prismer
