#include <Arya.h>

namespace Prismer {

class Unit;
class Tile;

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
    public:
        virtual void perform(shared_ptr<Tile> tile, shared_ptr<Unit> actor);

    private:
        // Animation? / Graphics?
};

} // namespace Prismer
