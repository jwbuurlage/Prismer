#include "GameSession.h"
#include "GameLogger.h"
#include "Unit.h"
#include "Grid.h"

#include <memory>

namespace Prismer {

using std::make_shared;
using std::make_unique;
using std::shared_ptr;

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

void GameSession::init()
{
    _grid = make_shared<Grid>(10, 10);
    _grid->init();
}

shared_ptr<Unit> GameSession::createUnit(UnitInfo info, int x, int y)
{
    if (_grid->getTile(x, y)->getInfo()->hasUnit())
        return nullptr;

    auto id = generateId();
    auto unit = make_shared<Unit>(id, info, shared_from_this());
    unit->setTile(_grid->getTile(x, y));
    unitMap.insert(std::pair<int, shared_ptr<Unit>>(unit->getId(), unit));
    return unit;
}

void GameSession::destroyUnit(int id)
{
    auto iter = unitMap.find(id);
    if(iter == unitMap.end())
    {
        GameLogWarning << "Trying to destroy non-existing unit id" << endLog;
        return;
    }
    unitMap.erase(iter);
}

shared_ptr<Unit> GameSession::getUnitById(int id)
{
    auto iter = unitMap.find(id);
    if(iter == unitMap.end()) return 0;
    return iter->second;
}

void GameSession::_listUnits() const
{
    GameLogInfo << "Units" << endLog;
    for (auto& iter : unitMap) {
        GameLogInfo << "Unit id: " << iter.second->getId() << endLog;
    }
}

} // namespace Prismer
