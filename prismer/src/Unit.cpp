#include "Colors.h"
#include "Faction.h"
#include "Tile.h"
#include "Unit.h"
#include "UnitGraphics.h"

namespace Prismer {

Unit::Unit(int id,
            weak_ptr<Faction> faction)
    : _id(id), _faction(faction)
{
}

void Unit::update(float dt, float t)
{
    if(_entity)
        _entity->update(dt, t);
}

void Unit::setTile(shared_ptr<Tile> tile)
{
    _x = tile->getX();
    _y = tile->getY();

    if (auto l_tile = _tile.lock())
        l_tile->getInfo()->setUnit(nullptr);

    _tile = tile;

    tile->getInfo()->setUnit(shared_from_this());

    if (_entity) 
        _entity->updateState();
}

void Unit::activate(shared_ptr<GridInput> grid_input)
{
}

void Unit::deactivate()
{
}

} // namespace Prismer
