#include <vector>
#include <memory>

#include "common.h"
#include "Unit.h"
#include "Tile.h"
#include "GameSession.h"
#include "GameSessionInput.h"
#include "GameLogger.h"
#include "Grid.h"
#include "Colors.h"
#include "Shapes/Circle.h"

namespace Prismer {

using std::make_shared;
using std::vector;
using Arya::Camera;

GameSessionInput::GameSessionInput(shared_ptr<GameSession> session)
{
    _session = session;
}

void GameSessionInput::init()
{
    auto input = Arya::Locator::getRoot().getInputSystem();

    input->bind("c", [this](bool down) {
            // create unit?
            if(down) {
                auto colors = vector<ColorID> { ColorID::red, ColorID::red };
                auto unitInfo = UnitInfo(Circle(colors));

                int seed = _session->generateId();

                // bad way to do this, but just for lolz
                int x = GRandom::genrand() % (_session->getGrid()->getWidth() / 2);
                int y = GRandom::genrand() % (_session->getGrid()->getHeight() / 2);

                GameLogInfo << "create unit " << x << " " << y << endLog;

                auto unit = _session->createUnit(unitInfo, x, y);
            }
        });

    input->bind("l", [this](bool down) {
            // create unit?
            if (down)
                _session->_listUnits();
        });

    input->bind("1", [this](bool down) {
            static auto tile = make_shared<TileInfo>();
            // create unit?
            if (down) {
                auto unit = _session->getUnitById(1);
                unit->cast(0, tile);
            }
        });
}

void GameSessionInput::update(float elapsedTime)
{
    return;
}

} // namespace Prismer
