#include <Arya.h>
#include <memory>

#include "GameSessionClient.h"
#include "Grid.h"
#include "GridGraphics.h"
#include "GridInput.h"
#include "GameLogger.h"
#include "GameSessionInput.h"
#include "GameCamera.h"
#include "Unit.h"
#include "UnitGraphics.h"

namespace Prismer {

using std::make_shared;

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

    input = make_unique<GameSessionInput>(shared_from_this());
    input->init();

    _grid_entity = make_shared<GridEntity>(_grid);
    _grid_entity->init();

    _grid_input = make_shared<GridInput>(_grid);

    _grid->setInput(_grid_input);
    _grid->setEntity(_grid_entity);

    _camera = make_shared<GameCamera>();

    return true;
}

void GameSessionClient::update(float elapsedTime)
{
    if (input)
        input->update(elapsedTime);

    _camera->update(elapsedTime);
}

void GameSessionClient::updateGameLogic(int elapsedTime)
{
    gameTimer += elapsedTime;

    //Update all units
    //mat4 vpMatrix = Arya::Locator::getRoot().getGraphics()->getCamera()->getVPMatrix();
    for(auto unitIter : getUnitMap())
    {
        auto unit = unitIter.second;
        unit->update(gameTimer);
    }
}

shared_ptr<Unit> GameSessionClient::createUnit(UnitInfo info, int x, int y)
{
    auto unit = GameSession::createUnit(info, x, y);

    if (!unit)
        return unit;

    GameLogInfo << "GameSessionClient::createUnit()" << endLog;
    // also create a unit entity
    auto unitEntity = make_shared<UnitEntity>(unit, _grid_entity);
    unit->setEntity(unitEntity);
    return unit;
}

} // namespace Prismer
