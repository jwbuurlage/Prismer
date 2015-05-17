#include "GameSessionClient.h"
#include "Arya.h"
#include "GameLogger.h"
#include "GameSessionInput.h"
#include "Unit.h"

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
    Arya::Camera* cam = Arya::Locator::getRoot().getGraphics()->getCamera();
    cam->setPosition(vec3(0.0f, 0.0f, 10.0f));
    cam->setPitch(-glm::radians(60.0f));
    cam->setZoom(100.0f);

    input = make_unique<GameSessionInput>();
    input->init();

    return true;
}

void GameSessionClient::update(float elapsedTime)
{
    if(input) input->update(elapsedTime);
}

void GameSessionClient::updateGameLogic(int elapsedTime)
{
    gameTimer += elapsedTime;

    //Update all units
    //mat4 vpMatrix = Arya::Locator::getRoot().getGraphics()->getCamera()->getVPMatrix();
    for(auto unitIter : getUnitMap())
    {
        Unit* unit = unitIter.second;
        unit->update(gameTimer);
    }
}

} // namespace Prismer
