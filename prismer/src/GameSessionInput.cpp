#include "GameSessionInput.h"
#include "GameLogger.h"
#include <glm/gtx/rotate_vector.hpp>

namespace Prismer {

using Arya::Camera;
using std::bind;

GameSessionInput::GameSessionInput()
{
}

GameSessionInput::~GameSessionInput()
{
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

    //TODO: Get keys from config
    input->bind("W", [this](bool down) {
            // create unit?
            GameLogInfo << "create unit" << endLog;
        });
}

void GameSessionInput::update(float elapsedTime)
{
    return;
}

} // namespace Prismer
