#include "GameSession.h"
#include "GameLogger.h"
#include "Unit.h"

GameSession::GameSession()
{
    gameTimer = 0;
}

GameSession::~GameSession()
{
    //After deleting the factions, there should be no more units left
    if(!unitMap.empty())
        GameLogError << "List of units is not empty at deconstruction of GameSession. Possible memory leak" << endLog;
}

Unit* GameSession::createUnit(UnitInfo info)
{
    auto id = generateId();
    auto unit = new Unit(id, info, this);
    unitMap.insert(std::pair<int, Unit*>(unit->getId(),unit));
    return unit;
}

void GameSession::destroyUnit(int id)
{
    auto iter = unitMap.find(id);
    if(iter == unitMap.end())
    {
        GameLogWarning << "Trying to destroy unexisting unit id" << endLog;
        return;
    }
    unitMap.erase(iter);
}

Unit* GameSession::getUnitById(int id)
{
    auto iter = unitMap.find(id);
    if(iter == unitMap.end()) return 0;
    return iter->second;
}
