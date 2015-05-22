#include "Game.h"
#include "GameLogger.h"
#include "GameSessionClient.h"

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

        using Arya::Entity;
        using Arya::ShaderProgram;

        shared_ptr<Entity> ent, ent2;
        auto model = root->getModelManager()->getModel("ogros.aryamodel");
        auto hexagon = root->getModelManager()->getModel("hexagon");
        auto triangle = root->getModelManager()->getModel("triangle");
        auto mat = root->getMaterialManager()->getMaterial("grass.tga");
        auto mat2 = root->getMaterialManager()->createMaterial(vec4(0.0f, 1.0f, 0.0f, 0.8f));
        auto mat3 = root->getMaterialManager()->createMaterial(vec4(1.0f, 0.0f, 0.0f, 0.8f));

        auto hexagon2 = hexagon->clone();
        hexagon2->setMaterial(mat);

        auto myShader = make_shared<ShaderProgram>(
                "../shaders/custom.vert",
                "../shaders/custom.frag");
        if (!myShader->isValid()) {
            myShader = nullptr;
            LogError << "Could not load custom shader." << Arya::endLog;
        }
        else
        {
            myShader->enableUniform(Arya::UNIFORM_MOVEMATRIX | Arya::UNIFORM_VPMATRIX | Arya::UNIFORM_TEXTURE);
            myShader->addUniform4fv("customUniform", [this](Entity* e){
                    if (e->getPosition().x > 50.0f && e->getPosition().x < 150.0f
                            && e->getPosition().y > 20.0f && e->getPosition().y < 80.0f)
                    return vec4(
                            0.7f + 0.3f*sin(0.06f * totalTime * e->getPosition().x),
                            0.7f + 0.3f*sin(0.09f * totalTime * e->getPosition().y),
                            1.0f,
                            1.0f);
                    else
                    return vec4(1.0f,1.0f,1.0f,1.0f);
                    });
            hexagon->setShaderProgram(myShader);
            hexagon2->setShaderProgram(myShader);
        }

        int counter = 0;
        const float a = 0.5f * sqrt(3.0f);
        for(int x = 0; x < 12; ++x) {
            for(int y = 0; y < 8; ++y) {
                vec3 pos(a*x, y + ((x&1)?0.5f : 0.0f),0.0f);
                pos *= 30.0f*a;

                ent = root->getWorld()->createEntity();
                ent->setPosition(pos);
                ent->setGraphics((((x+y)%2) == 0 ? hexagon : hexagon2));
                ent->getGraphics()->setScale(14.8f);

                if ( (x + y)%3 == 0 )
                {
                    ent = root->getWorld()->createEntity();
                    ent->setPosition(pos);
                    ent->setPitch(0.5f*M_PI);
                    ent->setGraphics(model);
                    ent->getGraphics()->setAnimation( animNames[counter%animCount] );
                    ++counter;

                    ent2 = ent;

                    ent = root->getWorld()->createEntity();
                    ent->setPosition(vec3(0.0f, 15.0f, 0.0f));
                    ent->setParent(ent2);
                    ent->setGraphics(mat2);
                    ent->getGraphics()->setScreenSize(vec2(0.03f, 0.005f));
                    ent->getGraphics()->setScreenOffset(vec2(-0.02f, 0.0f));

                    ent = root->getWorld()->createEntity();
                    ent->setPosition(vec3(0.0f, 15.0f, 0.0f));
                    ent->setParent(ent2);
                    ent->setGraphics(mat3);
                    ent->getGraphics()->setScreenSize(vec2(0.05f, 0.005f));
                    ent->getGraphics()->setScreenOffset(vec2(0.0f, 0.0f));
                }
            }
        }

    }
}

