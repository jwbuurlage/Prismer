#include "GameSession.h"
#include "GameLogger.h"
#include "Faction.h"
#include "Unit.h"
//#include "Map.h"

GameSession::GameSession()
{
    gameTimer = 0;
    map = 0;
}

GameSession::~GameSession()
{
    //The Faction deconstructor will unregister itself
    //so the map will become empty like this
    while(!factionMap.empty())
        delete factionMap.begin()->second;

    //After deleting the factions, there should be no more units left
    if(!unitMap.empty())
        GameLogError << "List of units is not empty at deconstruction of GameSession. Possible memory leak" << endLog;

    //if (map) delete map;
}

Unit* GameSession::createUnit(int id, int type)
{
    Unit* unit = getUnitById(id);
    if(unit)
    {
        GameLogWarning << "Trying to create unit with duplicate id (" << id << ")" << endLog;
        return unit;
    }
    unit = new Unit(type, id, this);
    unitMap.insert(std::pair<int,Unit*>(unit->getId(),unit));
    return unit;
}

void GameSession::destroyUnit(int id)
{
    auto iter = unitMap.find(id);
    if(iter == unitMap.end())
    {
        GameLogWarning << "Trying to destory unexisting unit id" << endLog;
        return;
    }
    unitMap.erase(iter);
    return;
}

Unit* GameSession::getUnitById(int id)
{
    auto iter = unitMap.find(id);
    if(iter == unitMap.end()) return 0;
    return iter->second;
}

Faction* GameSession::createFaction(int id)
{
    Faction* faction = getFactionById(id);
    if(faction)
    {
        GameLogWarning << "Trying to create faction with duplicate id (" << id << ")" << endLog;
        return faction;
    }
    faction = new Faction(id, this);
    factionMap.insert(std::pair<int,Faction*>(faction->getId(),faction));
    return faction;
}

void GameSession::destroyFaction(int id)
{
    auto iter = factionMap.find(id);
    if(iter == factionMap.end())
    {
        GameLogWarning << "Trying to destory unexisting faction id" << endLog;
        return;
    }
    factionMap.erase(iter);
    return;
}

Faction* GameSession::getFactionById(int id)
{
    auto iter = factionMap.find(id);
    if(iter == factionMap.end()) return 0;
    return iter->second;
}
