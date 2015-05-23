#include "Abilities/AGather.h"
#include "Tile.h"
#include "Unit.h"

#include "GameLogger.h"

namespace Prismer {

void AGather::perform(shared_ptr<Tile> tile,
                shared_ptr<Unit> actor)
{
    // check if tile has resource
    auto color = tile->getInfo()->popResource(); // ColorID::red;
    GameLogInfo << "Popped: " << color << endLog;
    actor->addColor(color);
};

bool AGather::isValid(shared_ptr<Tile> tile,
                shared_ptr<Unit>)
{
    // FIXME: unit next to it?
    return tile->getInfo()->hasResource();
}

void AGather::activate()
{

}

void AGather::deactivate()
{

}

} // namespace Prismer
