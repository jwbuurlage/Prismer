#include "GameSessionClient.h"
#include "Arya.h"
#include "GameLogger.h"
#include "GameSessionInput.h"
#include "Faction.h"
#include "Unit.h"
#include "UnitInfo.h"
#include "Cells.h"

#include <queue>
#include <algorithm>
using std::priority_queue;

GameSessionClient::GameSessionClient() : GameSession()
{
    input = 0;
    localFaction = 0;
    unitCells = 0;
    entityCreated = false;
}

GameSessionClient::~GameSessionClient()
{
    //if(unitCells) delete unitCells;

    if (input) delete input;

    GameLogInfo << "Game session ended" << endLog;
}

bool GameSessionClient::init()
{
    Arya::Camera* cam = Arya::Locator::getRoot().getGraphics()->getCamera();
    cam->setPosition(vec3(0.0f, 0.0f, 10.0f));
    cam->setPitch(-glm::radians(60.0f));
    cam->setZoom(100.0f);

    unitCells = new CellList(64, 1024); //map->getSize());
    input = new GameSessionInput(this);
    input->init();

    totalSessionTime = 0.0f;

    return true;
}

void GameSessionClient::rebuildCellList()
{
    unitCells->clear();
    for(auto unitIter : getUnitMap())
        unitIter.second->setCellFromList(unitCells);
}

//Temporarily
const int animCount = 19;
const char* animNames[animCount] = {"stand", "run", "attack", "pain_a", "pain_b", "pain_c", "jump", "flip", "salute", "fallback", "wave", "point", "crouch_stand", "crouch_walk", "crouch_death", "death_fallback", "death_fallforward", "death_fallbackslow", "boom"};

void GameSessionClient::update(float elapsedTime)
{
    if(input) input->update(elapsedTime);

    //We only create the entity after 0.5 seconds so that
    //the game first shows the window with black screen
    //instead of waiting to load all files
    //Step one towards a loading screen ;)

    totalSessionTime += elapsedTime;
    if (!entityCreated && totalSessionTime > 0.5f) {
        entityCreated = true;

        UnitInfo* info = new UnitInfo(1);
        (void)info;

        int idCounter = 1;

        using Arya::Entity;
        using Arya::ImageView;
        using Arya::Label;
        using Arya::Model;
        using Arya::Material;
        using Arya::ShaderProgram;

        Unit* unit;
        shared_ptr<Entity> ent, ent2;
        auto model = Model::create("ogros.aryamodel");
        auto hexagon = Model::create("hexagon");
        auto triangle = Model::create("triangle.aryamodel");
        auto circle = Model::create("circle");
        auto mat = Material::create("grass.tga");
        auto mat2 = Material::create(vec4(0.0f, 1.0f, 0.0f, 0.8f));
        auto mat3 = Material::create(vec4(1.0f, 0.0f, 0.0f, 0.8f));
        auto mat4 = Material::create(vec4(0.9f, 0.9f, 0.9f, 0.8f));

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
            myShader->enableUniform(Arya::UNIFORM_MOVEMATRIX | Arya::UNIFORM_VPMATRIX | Arya::UNIFORM_TEXTURE | Arya::UNIFORM_LIGHTMATRIX | Arya::UNIFORM_SHADOWTEXTURE);
            myShader->addUniform4fv("customUniform", [this](Arya::ShaderUniformBase* b){
                    Entity* e = static_cast<Entity*>(b);
                    if (e->getPosition().x > 50.0f && e->getPosition().x < 150.0f
                            && e->getPosition().y > 20.0f && e->getPosition().y < 80.0f)
                    return vec4(
                            0.7f + 0.3f*sin(0.06f * totalSessionTime * e->getPosition().x),
                            0.7f + 0.3f*sin(0.09f * totalSessionTime * e->getPosition().y),
                            1.0f,
                            1.0f);
                    else
                    return vec4(1.0f,1.0f,1.0f,1.0f);
                    });
            hexagon->setShaderProgram(myShader);
            hexagon2->setShaderProgram(myShader);
        }

        triangle->setMaterial(mat3);
        debugEntity = Entity::create();
        debugEntity->setPosition(vec3(0.0f, 0.0f, -0.02f)); //hidden
        debugEntity->setGraphics(circle);
        debugEntity->getGraphics()->setScale(2.0f);
        //debugEntity->setUserData(something);

        // bar at top of screen. 50px high, 10px from all sides
        auto image = ImageView::create();
        image->setMaterial(mat4);
        image->setPosition(vec2(0.5f, -1.0f), vec2(0.0f, 0.5f*80.0f + 10.0f)); //middle-top + (0,0)
        image->setSize(vec2(0.5f, 0.0f), vec2(-20.0f, 80.0f)); //fullwidth + (-20px, +40px)

        auto image2 = ImageView::create();
        image2->setMaterial(mat2);
        image2->setPosition(vec2(-1.0f, 0.0f), vec2(0.5f*100.0f + 10.0f, 0.0f));
        image2->setSize(vec2(0.0f, 1.0f), vec2(100.0f , -20.0f));

        auto label = Label::create();
        label->setPosition(vec2(0.0f, 0.0f), vec2(0.0f));
        label->setSize(vec2(1.0f), vec2(0.0f));
        label->setText("This is a font test.\nNewline test\nKerning test AVAVAV ijijAABB...M'N\nUTF-8 é â np");

        image->addToRootView();
        //image->add(image2);
        image->add(label);

        int counter = 0;
        const float a = 0.5f * sqrt(3.0f);
        for(int x = 0; x < 12; ++x) {
            for(int y = 0; y < 8; ++y) {
                vec3 pos(a*x, y + ((x&1)?0.5f : 0.0f),0.0f);
                pos *= 30.0f*a;

                ent = Entity::create();
                ent->setPosition(pos);
                ent->setGraphics((((x+y)%2) == 0 ? hexagon : hexagon2));
                ent->getGraphics()->setScale(14.8f);
                entities.push_back(ent);

                if ( (x + y)%3 == 0 )
                {
                    ent = Entity::create();
                    ent->setPosition(pos);
                    ent->setPitch(0.5f*M_PI);
                    ent->setGraphics(model);
                    ent->getGraphics()->setAnimation( animNames[counter%animCount] );
                    ++counter;

                    unit = createUnit(idCounter++, 1);
                    unit->setEntity(ent);

                    ent2 = ent;

                    ent = Entity::create();
                    ent->setPosition(vec3(0.0f, 15.0f, 0.0f));
                    ent->setParent(ent2);
                    ent->setGraphics(mat2);
                    ent->getGraphics()->setScreenSize(vec2(0.03f, 0.005f));
                    ent->getGraphics()->setScreenOffset(vec2(-0.02f, 0.0f));
                    entities.push_back(ent);

                    ent = Entity::create();
                    ent->setPosition(vec3(0.0f, 15.0f, 0.0f));
                    ent->setParent(ent2);
                    ent->setGraphics(mat3);
                    ent->getGraphics()->setScreenSize(vec2(0.05f, 0.005f));
                    ent->getGraphics()->setScreenOffset(vec2(0.0f, 0.0f));
                    entities.push_back(ent);
                }
            }
        }

    }
}

void GameSessionClient::updateGameLogic(int elapsedTime)
{
    if(!localFaction) return;
    gameTimer += elapsedTime;

    //First purge units that have to be deleted
    for(auto fIter : getFactionMap())
    {
        Faction* faction = fIter.second;
        for(auto it = faction->getUnits().begin(); it != faction->getUnits().end(); )
        {
            if ((*it)->readyToDelete()) {
                delete *it;
                it = faction->getUnits().erase(it);
            } else {
                ++it;
            }
        }
    }

    //Update all units
    //mat4 vpMatrix = Arya::Locator::getRoot().getGraphics()->getCamera()->getVPMatrix();
    for(auto unitIter : getUnitMap())
    {
        Unit* unit = unitIter.second;
        //vec4 onScreen(unit->getEntity()->getPosition(), 1.0);
        //onScreen = vpMatrix * onScreen;
        //onScreen.x /= onScreen.w;
        //onScreen.y /= onScreen.w;
        //unit->setScreenPosition(vec2(onScreen.x, onScreen.y));
        unit->update(gameTimer);
    }
    for(auto unit : localFaction->getUnits())
        unit->checkForEnemies();
}

