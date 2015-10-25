#include <Arya.h>
#include <memory>
#include <sstream>
#include <algorithm>

#include "Faction.h"
#include "FactionInput.h"
#include "GameSessionClient.h"
#include "GameLogger.h"
#include "GameCamera.h"
#include "GameInterface.h"
#include "GameSessionInput.h"
#include "Grid.h"
#include "GridGraphics.h"
#include "GridInput.h"
#include "Unit.h"
#include "Shapes/TriangleGraphics.h"

namespace Prismer {

using std::make_shared;
using std::stringstream;

GameSessionClient::GameSessionClient() : GameSession()
{
}

GameSessionClient::~GameSessionClient()
{
    GameLogInfo << "Game session ended" << endLog;
}

bool GameSessionClient::init()
{
    GameSession::init();

    _input = make_unique<GameSessionInput>(
            std::dynamic_pointer_cast<GameSessionClient>(
                shared_from_this()));
    _input->init();

    _grid_entity = make_shared<GridEntity>(_grid);
    _grid_entity->init();

    _grid_input = make_shared<GridInput>(_grid);

    _grid->setInput(_grid_input);
    _grid->setEntity(_grid_entity);

    _camera = make_shared<GameCamera>();

    for (auto& fac : _factions) {
        fac->setInput(make_shared<FactionInput>(fac));
    }

    _interface = make_unique<GameInterface>();
    toggleFPS();

    return true;
}

void GameSessionClient::update(float elapsedTime)
{
    total_time += elapsedTime;

    if(_interface)
        _interface->update(elapsedTime);

    if (_input)
        _input->update(elapsedTime);

    _camera->update(elapsedTime);

    for(auto unitIter : getUnitMap())
    {
        auto unit = unitIter.second;
        unit->update(elapsedTime, total_time);
    }
}

void GameSessionClient::updateGameLogic(int elapsedTime)
{
    //FIXME what to do with this function
    return;
}

shared_ptr<Unit> GameSessionClient::createUnit(int x, int y)
{
    auto unit = GameSession::createUnit(x, y);

    if (!unit)
        return unit;

    GameLogInfo << "GameSessionClient::createUnit()" << endLog;
    // also create a unit entity
    auto unitEntity = make_shared<TriangleEntity>(unit, _grid_entity);
    unit->setEntity(unitEntity);

    unitEntity->setTintColor((*_currentFactionIter)->getColor());

    return unit;
}

void GameSessionClient::toggleFPS()
{
    _interface->toggleFPS();
}

} // namespace Prismer
