#include "GameSessionClient.h"
#include "Arya.h"
#include "GameLogger.h"
#include "GameSessionInput.h"
#include "Faction.h"
#include "Unit.h"
#include "Cells.h"

#include <queue>
#include <algorithm>
using std::priority_queue;

GameSessionClient::GameSessionClient() : GameSession()
{
    input = 0;
    localFaction = 0;
    unitCells = 0;
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
    input = new GameSessionInput;
    input->init();

    return true;
}

void GameSessionClient::rebuildCellList()
{
    unitCells->clear();
    for(auto unitIter : getUnitMap())
        unitIter.second->setCellFromList(unitCells);
}

void GameSessionClient::update(float elapsedTime)
{
    if(input) input->update(elapsedTime);
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

