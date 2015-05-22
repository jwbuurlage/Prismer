#include "GameSessionInput.h"
#include "GameLogger.h"
#include "GameSessionClient.h"
#include <glm/gtx/rotate_vector.hpp>

using Arya::Camera;
using std::bind;

GameSessionInput::GameSessionInput(GameSessionClient* s)
{
    session = s;

    goingForward = goingBackward = goingLeft = goingRight = goingUp = goingDown = false;
    rotatingRight = rotatingLeft = false;
    mouseLeft = mouseRight = mouseTop = mouseBot = false;
    draggingLeftMouse = draggingRightMouse = false;
    slowMode = false;

    forceDirection = vec3(0.0f);
    specMovement = vec3(0.0f);
    specPos = vec3(0.0f,0.0f, 0.0f);
    originalMousePos = vec2(0.0);

    Camera* cam = Arya::Locator::getRoot().getGraphics()->getCamera();
    if(cam) specPos = cam->getPosition();
}

GameSessionInput::~GameSessionInput()
{
    Arya::InputSystem* input = Arya::Locator::getRoot().getInputSystem();
    input->unbind("W");
    input->unbind("A");
    input->unbind("S");
    input->unbind("D");
    input->unbind("Q");
    input->unbind("E");
    input->unbind("Z");
    input->unbind("X");
    input->unbind("R");
    input->unbind(Arya::INPUT_MOUSEBUTTON);
    input->unbind(Arya::INPUT_MOUSEWHEEL);
    input->unbind(Arya::INPUT_MOUSEMOVEMENT);
}

void GameSessionInput::init()
{
    Arya::InputSystem* input = Arya::Locator::getRoot().getInputSystem();

    input->bind(Arya::INPUT_MOUSEWHEEL,
            [this](int delta) { mouseWheelMoved(delta); });
    input->bindMouseMove(
            [this](int x, int y, int dx, int dy) { mouseMoved(x,y,dx,dy); });
    input->bindMouseButton(
            [this](Arya::MOUSEBUTTON btn, bool down, int x, int y)
            { mouseDown(btn,down,x,y); });

    //TODO: Get keys from config
    input->bind("W", [this](bool down) { goingForward   = down; computeForce(); });
    input->bind("A", [this](bool down) { goingLeft      = down; computeForce(); });
    input->bind("S", [this](bool down) { goingBackward  = down; computeForce(); });
    input->bind("D", [this](bool down) { goingRight     = down; computeForce(); });
    input->bind("Q", [this](bool down) { goingDown      = down; computeForce(); });
    input->bind("E", [this](bool down) { goingUp        = down; computeForce(); });
    input->bind("Z", [this](bool down) { rotatingLeft   = down; computeForce(); });
    input->bind("X", [this](bool down) { rotatingRight  = down; computeForce(); });
    input->bind("R", [this](bool down) { /* sendEvent(EVENT_GAME_FULLSTATE_REQUEST); */ (void)down; });
}

void GameSessionInput::update(float elapsedTime)
{
    Camera* cam = Arya::Locator::getRoot().getGraphics()->getCamera();

    specMovement*=pow(.002f,elapsedTime);

    if(cam != 0)
    {
        cam->setPositionSmooth(specPos);

        if( rotatingLeft && !rotatingRight )
            cam->rotate(1.5f * elapsedTime, 0.0f);
        else if( rotatingRight && !rotatingLeft )
            cam->rotate(-1.5f * elapsedTime, 0.0f);

        vec3 force = forceDirection;
        force = glm::rotateZ(force, cam->getYaw());
        float speedFactor = 4000.0f;
        if(slowMode) speedFactor *= 0.125f;
        float zoomSpeedFactor = cam->getZoom()/300.0f;
        specMovement += force * speedFactor * zoomSpeedFactor * elapsedTime;
    }

    specPos += specMovement * elapsedTime;

    return;
}

void GameSessionInput::computeForce()
{
    forceDirection = vec3(0.0f);
    if(goingForward || mouseTop)    forceDirection.y += 1.0f;
    if(goingBackward || mouseBot)   forceDirection.y -= 1.0f;
    if(goingLeft || mouseLeft)      forceDirection.x -= 1.0f;
    if(goingRight || mouseRight)    forceDirection.x += 1.0f;
    if(goingUp)                     forceDirection.z += 2.0f;
    if(goingDown)                   forceDirection.z -= 2.0f;
    if(forceDirection != vec3(0.0f))
        forceDirection = glm::normalize(forceDirection);
}

void GameSessionInput::mouseDown(Arya::MOUSEBUTTON button, bool buttonDown, int x, int y)
{
    (void)x; (void)y;
    if(button == Arya::MOUSEBUTTON_LEFT)
    {
        draggingLeftMouse = (buttonDown == true);

        if (!buttonDown)
        {
            auto gr = Arya::Locator::getRoot().getGraphics();
            auto cam = gr->getCamera();

            vec2 mouse = gr->normalizeMouseCoordinates(x,y);
            vec3 world1 = cam->getWorldCoordinates(vec3(mouse, -1.0f));
            vec3 world2 = cam->getWorldCoordinates(vec3(mouse, 1.0f));
            vec3 diff = world2-world1;
            if (glm::abs(diff.z) > 0.001)
            {
                vec3 point = world1 - (world1.z/diff.z)*diff;
                point.z += 0.02f;
                if (session && session->debugEntity)
                    session->debugEntity->setPosition(point);
            }

            // cross line through world1,world2 with z=0
            // line = a + lambda*(b-a)
            // line.z == 0
            // lambda = -a.z/(b.z-a.z)
        }
    }
    else if(button == Arya::MOUSEBUTTON_RIGHT)
    {
        draggingRightMouse = (buttonDown == true);
    }
}

void GameSessionInput::mouseWheelMoved(int delta)
{
    Camera* cam = Arya::Locator::getRoot().getGraphics()->getCamera();
    if (!cam) return;
    cam->camZoomSpeed -= 50.0f*delta;
    return;
}

void GameSessionInput::mouseMoved(int x, int y, int dx, int dy)
{
    (void)dx; (void)dy;
    int padding = 10;

    mouseLeft = (x < padding);
    mouseTop = (y < padding);
    mouseRight = (x > Arya::Locator::getRoot().getWindowWidth() - padding);
    mouseBot = (y > Arya::Locator::getRoot().getWindowHeight() - padding);

    computeForce();
    return;
}

