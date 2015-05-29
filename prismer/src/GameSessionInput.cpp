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

                // bad way to do this, but just for lolz
                int x = (_session->getGrid()->getWidth() / 2);
                int y = (_session->getGrid()->getHeight() / 2);

                auto unit = _session->createUnit(unitInfo, x, y);
                if (!unit) {
                    GameLogInfo << "cannot create unit" << endLog;
                } else {
                    GameLogInfo << "create unit " << x << " " << y << endLog;
                }
            }
        });

    input->bind("l", [this](bool down) {
            // create unit?
            if (down)
                _session->_listUnits();
        });
}

void GameSessionInput::update(float elapsedTime)
{
    return;
}

} // namespace Prismer
