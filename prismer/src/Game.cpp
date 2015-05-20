#include "Game.h"
#include "GameLogger.h"
#include "GameSessionClient.h"

using namespace Arya;

namespace Prismer {

Game::Game()
{
    totalTime = 0.0f;
}

Game::~Game()
{
}

bool Game::init()
{
    root = new Arya::Root();

    if (!root->init("Prismer", 1024, 768, false)) {
        return false;
    }

    auto quitFunc = [this](bool down) {
        if (down)
            root->stopGameLoop();
    };

    Arya::InputSystem* input = root->getInputSystem();
    input->bind("shift+q", quitFunc);
    input->bind("shift+space", [](bool down) {
            if(down)
                GameLogDebug << "You pressed shift+space" << endLog;
        });

    session = make_shared<GameSessionClient>();

    auto triangle = root->getModelManager()->getModel("triangle");

    auto ent = root->getWorld()->createEntity();
    ent->setPosition(vec3(x, y, 0));
    ent->setPitch(0.5f*M_PI);
    ent->setGraphics(triangle);
    ent->getGraphics()->setScale(10.0f);

    if (!session->init())
        return false;

    return true;
}

void Game::run()
{
    root->gameLoop([this] (float f) { this->update(f); });
}

void Game::update(float dt)
{
    totalTime += dt;

    session->update(dt);
}

} // namespace Prismer
