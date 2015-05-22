#include "Game.h"
#include "GameLogger.h"
#include "GameSessionClient.h"

Game::Game()
{
    totalTime = 0;
    session = 0;
}

Game::~Game()
{
    if (session) delete session;
}

bool Game::init()
{
    root = new Arya::Root();

    if(!root->init("Minimal Example", 1024, 768, false)) {
        return false;
    }

    auto quitFunc = [this](bool down) { if (down) root->stopGameLoop(); };
    Arya::InputSystem* input = root->getInputSystem();
    input->bind("CTRL+Q", quitFunc);
    input->bind("escape", quitFunc);
    input->bind("ctrl+shift+f", [](bool down) { if(down) GameLogDebug << "ctrl+shift+f" << endLog; });
    input->bind("shift+space", [](bool down) { if(down) GameLogDebug << "You pressed shift+space" << endLog; });

    if (session) delete session;
    session = new GameSessionClient;

    if (!session->init()) return false;

    return true;
}

void Game::run()
{
    root->gameLoop( std::bind(&Game::update, this, std::placeholders::_1) );
}

void Game::update(float dt)
{
    totalTime += dt;
    session->update(dt);
}

