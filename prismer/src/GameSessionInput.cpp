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
using Arya::MousePos;

GameSessionInput::GameSessionInput(shared_ptr<GameSession> session)
{
    _session = session;
}

void GameSessionInput::init()
{
    auto input = Arya::Locator::getRoot().getInputSystem();

    bindingC = input->bind("c", [this](bool down, const MousePos&) {
            // create unit?
            if(down) {
                auto colors = vector<ColorID> { ColorID::red, ColorID::red };

                // bad way to do this, but just for lolz
                int x = (_session->getGrid()->getWidth() / 2);
                int y = (_session->getGrid()->getHeight() / 2);t pu

                auto unit = _session->createUnit(x, y);
                if (!unit) {
                    GameLogInfo << "cannot create unit" << endLog;
                } else {
                    GameLogInfo << "create unit " << x << " " << y << endLog;
                }
                return true;
            }
            return false;
        });

    bindingL = input->bind("l", [this](bool down, const MousePos&) {
            // create unit?
            if (down)
                _session->_listUnits();
            return down;
        });
}

void GameSessionInput::update(float elapsedTime)
{
    return;
}

} // namespace Prismer
