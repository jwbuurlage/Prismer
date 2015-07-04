#include <Arya.h>

#include "Faction.h"
#include "FactionInput.h"
#include "GameLogger.h"
#include "GameSession.h"
#include "Unit.h"
#include "UnitGraphics.h"

namespace Prismer {

Faction::Faction(int id, weak_ptr<GameSession> session)
    : _id(id), _session(session)
{
    static vec3 colors[4] = {
        vec3(1.0, 0.0, 0.0),
        vec3(1.0, 1.0, 0.0),
        vec3(0.0, 1.0, 0.0),
        vec3(0.0, 1.0, 1.0)
    };

    _color = colors[_id];
}

Faction::~Faction()
{
}

void Faction::beginTurn()
{
    GameLogInfo << "Begin turn of faction: " << _id << endLog;

    if (_input)
        _input->activate();

    for (auto& unit : _units) {
        unit->resetPoints();
    }

    _active = true;
}

void Faction::endTurn()
{
    GameLogInfo << "End turn of faction: " << _id << endLog;

    if (_input)
        _input->deactivate();

    if (auto session = _session.lock())
        session->nextFaction();

    _active = false;
}

void Faction::addUnit(shared_ptr<Unit>& unit)
{
    GameLogInfo << "Add unit: " << _id << endLog;

   _units.push_back(unit);

}

} //namespace Prismer
