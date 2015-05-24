#include "Abilities/AGather.h"
#include "Tile.h"
#include "GridInput.h"
#include "Unit.h"

#include "GameLogger.h"

namespace Prismer {

void AGather::perform()
{
    auto hover = _grid_input->getHovered();
    if (!hover)
        return;

    //actor->setTile(_hover);
};

bool AGather::isValid()
{
    auto hover = _grid_input->getHovered();
    if (!hover)
        return false;

    // FIXME: unit next to it?
    return hover->getInfo()->hasResource();
}

void AGather::activate(shared_ptr<Unit> actor,
                shared_ptr<GridInput> grid_input)
{
    Ability::activate(actor, grid_input);
}

void AGather::deactivate()
{

}

} // namespace Prismer
