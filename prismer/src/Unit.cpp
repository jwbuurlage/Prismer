#include "Tile.h"
#include "Unit.h"
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

    GameLogInfo << "setting to tile" << endLog;
    tile->getInfo()->setUnit(shared_from_this());
}

void Unit::activate()
{
    GameLogInfo << "Unit::activate()" << endLog;
    for (auto& ability : _info.shape.getAbilities()) {
        ability->activate();
    }
}

void Unit::deactivate()
{
    GameLogInfo << "Unit::deactivate()" << endLog;
    for (auto& ability : _info.shape.getAbilities()) {
        ability->deactivate();
    }
}

} // namespace Prismer
