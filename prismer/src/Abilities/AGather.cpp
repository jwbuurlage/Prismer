#include "Abilities/AGather.h"
#include "Tile.h"
#include "Unit.h"

#include "GameLogger.h"

namespace Prismer {

void AGather::perform(shared_ptr<TileInfo> tile,
                shared_ptr<Unit> actor)
{
    // check if tile has resource
    auto color = tile->popResource(); // ColorID::red;
    GameLogInfo << "Popped: " << color << endLog;
    actor->addColor(color);
};

bool AGather::isValid(shared_ptr<TileInfo> tile)
{
    return tile->hasResource();
}


} // namespace Prismer
