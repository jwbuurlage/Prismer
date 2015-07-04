#include <Arya.h>
#include <memory>
#include <sstream>
#include <algorithm>

#include "Faction.h"
#include "FactionInput.h"
#include "GameSessionClient.h"
#include "Grid.h"
#include "GridGraphics.h"
#include "GridInput.h"
#include "GameLogger.h"
#include "GameSessionInput.h"
#include "GameCamera.h"
#include "Unit.h"
#include "Shapes/TriangleGraphics.h"

namespace Prismer {

using std::make_shared;
using std::stringstream;

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

    _grid_entity = make_shared<GridEntity>(_grid);
    _grid_entity->init();

    _grid_input = make_shared<GridInput>(_grid);

    _grid->setInput(_grid_input);
    _grid->setEntity(_grid_entity);

    _camera = make_shared<GameCamera>();

    for (auto& fac : _factions) {
        fac->setInput(make_shared<FactionInput>(fac));
    }

    /* FPS COUNTER */
    using Arya::Material;
    auto matGray = Material::create(vec4(0.8f, 0.8f, 0.8f, 0.8f));

    using Arya::Font;
    auto font = make_shared<Font>();
    font->loadFromFile("DejaVuSansMono.ttf", 28);

    using Arya::ImageView;
    auto background = ImageView::create();
    background->setMaterial(matGray);
    background->setPosition(vec2(-1.0f, 1.0f), vec2(65.0f, -35.0f)); //middle-top + (0, -10px)
    background->setSize(vec2(0.0f, 0.0f), vec2(110.0f, 50.0f)); //fullwidth + (-20px, +300px)
    background->addToRootView();

    using Arya::Label;
    _fpsCounterLabel = Label::create();
    _fpsCounterLabel->setPosition(vec2(0.0f, 0.0f), vec2(0.0f, 0.0f));
    _fpsCounterLabel->setSize(vec2(0.0f, 0.0f), vec2(80.0f, 30.0f));
    if (font)
        _fpsCounterLabel->setFont(font);
    _fpsCounterLabel->setText("0 FPS");
    background->add(_fpsCounterLabel);

    return true;
}

void GameSessionClient::update(float elapsedTime)
{
    total_time += elapsedTime;

    for (int i = 99; i >= 1; --i)
        _elapsedTimes[i] = _elapsedTimes[i - 1];
    _elapsedTimes[0] = elapsedTime;

    auto sum_of_totals = std::accumulate(_elapsedTimes.begin(),
            _elapsedTimes.end(), 0.0f);
    auto average = sum_of_totals / 100.0f;

    auto fps = (int)(1.0f / average);
    stringstream ss;
    ss << fps << " FPS";
    _fpsCounterLabel->setText(ss.str());

    if (input)
        input->update(elapsedTime);

    _camera->update(elapsedTime);

    for(auto unitIter : getUnitMap())
    {
        auto unit = unitIter.second;
        unit->update(elapsedTime, total_time);
    }
}

void GameSessionClient::updateGameLogic(int elapsedTime)
{
    //FIXME what to do with this function
    return;
}

shared_ptr<Unit> GameSessionClient::createUnit(int x, int y)
{
    auto unit = GameSession::createUnit(x, y);

    if (!unit)
        return unit;

    GameLogInfo << "GameSessionClient::createUnit()" << endLog;
    // also create a unit entity
    auto unitEntity = make_shared<TriangleEntity>(unit, _grid_entity);
    unit->setEntity(unitEntity);

    unitEntity->setTintColor((*_currentFactionIter)->getColor());

    return unit;
}

} // namespace Prismer
