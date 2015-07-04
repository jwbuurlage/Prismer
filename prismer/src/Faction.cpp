#include <Arya.h>

#include "Faction.h"
#include "FactionInput.h"
#include "GameLogger.h"
#include "GameSession.h"
#include "Unit.h"

namespace Prismer {

Faction::Faction(int id, weak_ptr<GameSession> session)
    : _id(id), _session(session)
{
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
}

void Faction::endTurn()
{
    GameLogInfo << "End turn of faction: " << _id << endLog;

    if (_input)
        _input->deactivate();

    if (auto session = _session.lock())
        session->nextFaction();
}

} //namespace Prismer
