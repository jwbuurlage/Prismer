#include "GameSession.h"
#include "GameSessionInput.h"
#include "GameLogger.h"
#include "Unit.h"

namespace Prismer {

using Arya::Camera;

GameSessionInput::GameSessionInput(shared_ptr<GameSession> session)
{
    _session = session;
}

void GameSessionInput::init()
{
    auto input = Arya::Locator::getRoot().getInputSystem();

    input->bind(Arya::INPUT_MOUSEWHEEL,
            [this](int delta) {
                GameLogInfo << "wheel" << endLog;
        });

    input->bindMouseMove(
            [this](int x, int y, int dx, int dy) {
                //mouseMoved(x,y,dx,dy);
        });

    input->bindMouseButton(
            [this](Arya::MOUSEBUTTON btn, bool down, int x, int y) {
                // mouseDown(btn,down,x,y);
                GameLogInfo << "button" << endLog;
        });

    input->bind("w", [this](bool down) {
            // create unit?
            if(down) {
                GameLogInfo << "create unit" << endLog;
                auto unit = _session->createUnit(UnitInfo(1));
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
