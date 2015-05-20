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

} // namespace Prismer
