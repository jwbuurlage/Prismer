#include <cmath>

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

    Arya::InputSystem* input = root->getInputSystem();

    keyBinding = input->bind("shift+q", [this](bool down, const MousePos&) {
        if (down)
            root->stopGameLoop();
        return down;
        }, Arya::CHAIN_LAST);
    Arya::Locator::getCommandHandler().bind("quit",[this](const string&)
            {
            root->stopGameLoop();
            });
    Arya::Locator::getCommandHandler().bind("exit",[this](const string&)
            {
            root->stopGameLoop();
            });


    session = make_shared<GameSessionClient>();

    if (!session->init())
        return false;

    return true;
}

void Game::run()
{
    session->startMatch();

    root->gameLoop([this] (float f) { this->update(f); });
}

void Game::update(float dt)
{
    totalTime += dt;

    session->update(dt);
}

} // namespace Prismer
