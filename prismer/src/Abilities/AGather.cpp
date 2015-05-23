#include "Abilities/AGather.h"
#include "Tile.h"
#include "Unit.h"

#include "GameLogger.h"

namespace Prismer {

void AGather::perform(shared_ptr<Unit> actor)
{
    if (!_hover)
        return;

    //actor->setTile(_hover);
};

bool AGather::isValid(shared_ptr<Unit>)
{
    if (!_hover)
        return false;

    // FIXME: unit next to it?
    return _hover->getInfo()->hasResource();
}

void AGather::activate(shared_ptr<Unit> actor)
{

}

void AGather::deactivate()
{

}

} // namespace Prismer
