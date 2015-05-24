#include "Tile.h"
#include "Unit.h"
#include "UnitGraphics.h"
#include "Colors.h"

namespace Prismer {

Unit::Unit(int id,
            UnitInfo info,
            shared_ptr<GameSession> session)
    : _id(id), _info(info), _session(session)
{
    // FIXME: create entity from unitInfo
}

void Unit::update(float dt, float t)
{
    _entity->update(dt, t);
}

void Unit::addColor(ColorID color)
{
    // shape...
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
    for (auto& ability : _info.shape.getAbilities()) {
        ability->activate(shared_from_this(), grid_input);
    }
}

void Unit::deactivate()
{
    for (auto& ability : _info.shape.getAbilities()) {
        ability->deactivate();
    }
}

} // namespace Prismer
