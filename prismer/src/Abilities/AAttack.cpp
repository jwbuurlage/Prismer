#include <Abilities/AGather.h>

namespace Prismer {

AGather::perform(shared_ptr<TileInfo> tile,
                shared_ptr<Unit> actor)
{
    // check if tile has resource
    tile->unit.health -= actor.attack_damage;
};

bool AGather::isValid(shared_ptr<TileInfo> tile)
{
    if (tile->unit) // TileInfo shared_ptr<Unit> 
        return true;
    return false;
}

} // namespace Prismer
