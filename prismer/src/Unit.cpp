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

void Unit::update(int gameTimer) const
{
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

    GameLogInfo << "setting to tile" << endLog;
    tile->getInfo()->setUnit(shared_from_this());

    if (_entity) 
        _entity->update();
}

void Unit::activate()
{
    GameLogInfo << "Unit::activate()" << endLog;
    for (auto& ability : _info.shape.getAbilities()) {
        ability->activate(shared_from_this());
    }
}

void Unit::deactivate()
{
    GameLogInfo << "Unit::deactivate()" << endLog;
    for (auto& ability : _info.shape.getAbilities()) {
        ability->deactivate();
    }
}

void Unit::hover(shared_ptr<Tile> tile)
{
    for (auto& ability : _info.shape.getAbilities()) {
        ability->hover(tile);
    }
}

} // namespace Prismer
