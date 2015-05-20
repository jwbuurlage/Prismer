#include <Arya.h>

#include "GameSessionClient.h"
#include "GridGraphics.h"
#include "GameLogger.h"
#include "GameSessionInput.h"
#include "Unit.h"
#include "GameCamera.h"

namespace Prismer {

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

    auto& root = Arya::Locator::getRoot();


    _grid_entity = make_unique<GridEntity>(_grid);

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

shared_ptr<Unit> GameSessionClient::createUnit(UnitInfo info)
{
    auto unit = GameSession::createUnit(info);
    GameLogInfo << "GameSessionClient::createUnit()" << endLog;
    // also create a unit entity
    return unit;
}

} // namespace Prismer
