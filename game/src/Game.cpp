#include "Game.h"
#include "GameLogger.h"
#include "GameSessionClient.h"
using namespace Arya;

Game::Game()
{
    totalTime = 0;
    entityCreated = false;
    session = 0;
}

Game::~Game()
{
    if (session) delete session;
}

//Temporarily
const int animCount = 19;
const char* animNames[animCount] = {"stand", "run", "attack", "pain_a", "pain_b", "pain_c", "jump", "flip", "salute", "fallback", "wave", "point", "crouch_stand", "crouch_walk", "crouch_death", "death_fallback", "death_fallforward", "death_fallbackslow", "boom"};

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

    //We only create the entity after 0.5 seconds so that
    //the game first shows the window with black screen
    //instead of waiting to load all files
    //Step one towards a loading screen ;)
    if (!entityCreated && totalTime > 0.5f) {
        entityCreated = true;

        Arya::Model* model = root->getModelManager()->getModel("ogros.aryamodel");

        int counter = 0;
        for(int x = 0; x < 10; ++x) {
            for(int y = 0; y < 10; ++y) {
                Entity* ent = root->getWorld()->getEntitySystem()->createEntity();
                ent->setPosition(vec3(30.0f*x,30.0f*y,0));
                ent->setModel(model);
                ent->getGraphics()->setAnimation( animNames[counter%animCount] );
                ++counter;
            }
        }
    }
}

