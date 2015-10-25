#include <memory>
#include <vector>

#include "GameSession.h"
#include "GameLogger.h"
#include "Shapes/Triangle.h"
#include "Colors.h"
#include "Grid.h"
#include "Faction.h"

namespace Prismer {

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::vector;

GameSession::GameSession()
{
    gameTimer = 0;
}

GameSession::~GameSession()
{
}

void GameSession::init()
{
    _grid = make_shared<Grid>(10, 10);
    _grid->init();

    // make factions
    int numFactions = 2;
    for (int i = 0; i < numFactions; ++i) {
        _factions.push_back(make_shared<Faction>(i,
                    weak_ptr<GameSession>(shared_from_this())));
    }
    
    _currentFactionIter = _factions.end();
}

int GameSession::generateId() const
{
    static int id = 1;
    return id++;
}

shared_ptr<Unit> GameSession::createUnit(int x, int y)
{
    if (_grid->getTile(x, y)->getInfo()->hasUnit())
        return nullptr;

    auto id = generateId();
    vector<ColorID> colors;
    shared_ptr<Unit> unit = make_shared<Triangle>(id,
            weak_ptr<Faction>(*_currentFactionIter),
            colors);
    unit->setTile(_grid->getTile(x, y));
    unitMap.insert(std::pair<int, shared_ptr<Unit>>(unit->getId(), unit));

    (*_currentFactionIter)->addUnit(unit);

    return unit;
}

void GameSession::startMatch()
{
    _turn = 1;

    _currentFactionIter = _factions.begin();
    (*_currentFactionIter)->beginTurn();
}

void GameSession::nextFaction()
{
    _currentFactionIter++;

    if (_currentFactionIter == _factions.end()) {
        _turn++;
        _currentFactionIter = _factions.begin();
    }

    (*_currentFactionIter)->beginTurn();
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
