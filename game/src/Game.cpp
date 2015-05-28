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

    auto quitFunc = [this](bool down, const Arya::MousePos&) { if (down) root->stopGameLoop(); return down; };
    Arya::InputSystem* input = root->getInputSystem();
    bindQ = input->bind("CTRL+Q", quitFunc, Arya::CHAIN_LAST);
    bindW = input->bind("CTRL+W", quitFunc, Arya::CHAIN_LAST);
    bindEscape = input->bind("escape", quitFunc, Arya::CHAIN_LAST);

    if (session) delete session;
    session = new GameSessionClient;

    if (!session->init()) return false;

    return true;
}

float fpstimer;
int fpscounter;
void Game::run()
{
    fpstimer = 0.0f;
    fpscounter = 0;
    root->gameLoop( std::bind(&Game::update, this, std::placeholders::_1) );
}

void Game::update(float dt)
{
    totalTime += dt;
    session->update(dt);

    fpstimer += dt;
    fpscounter++;

    if (fpstimer >= 5.0f)
    {
        LogInfo << "FPS: " << float(fpscounter) / fpstimer << endLog;
        fpstimer = 0.0f;
        fpscounter = 0;
    }
}

