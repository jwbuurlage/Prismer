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
}

void GameSessionInput::init()
{
    Arya::InputSystem* input = Arya::Locator::getRoot().getInputSystem();

    bindings.push_back(input->bind(Arya::INPUT_MOUSEWHEEL,
            [this](int delta, const MousePos&)
            {
            mouseWheelMoved(delta);
            return true;
            }, Arya::CHAIN_LAST));
    bindings.push_back(input->bindMouseMove(
            [this](const MousePos& pos, int dx, int dy)
            {
            mouseMoved(pos,dx,dy);
            return true;
            }, Arya::CHAIN_LAST));
    bindings.push_back(input->bindMouseButton(
            [this](Arya::MOUSEBUTTON btn, bool down, const MousePos& pos)
            {
            return mouseDown(btn,down,pos);
            }, Arya::CHAIN_LAST));

    //TODO: Get keys from config
    bindings.push_back(input->bind("W", [this](bool down, const MousePos&) { goingForward   = down; computeForce(); return true; }));
    bindings.push_back(input->bind("A", [this](bool down, const MousePos&) { goingLeft      = down; computeForce(); return true; }));
    bindings.push_back(input->bind("S", [this](bool down, const MousePos&) { goingBackward  = down; computeForce(); return true; }));
    bindings.push_back(input->bind("D", [this](bool down, const MousePos&) { goingRight     = down; computeForce(); return true; }));
    bindings.push_back(input->bind("Q", [this](bool down, const MousePos&) { goingDown      = down; computeForce(); return true; }));
    bindings.push_back(input->bind("E", [this](bool down, const MousePos&) { goingUp        = down; computeForce(); return true; }));
    bindings.push_back(input->bind("Z", [this](bool down, const MousePos&) { rotatingLeft   = down; computeForce(); return true; }));
    bindings.push_back(input->bind("X", [this](bool down, const MousePos&) { rotatingRight  = down; computeForce(); return true; }));
    //bindings.push_back(input->bind("R", [this](bool down, const MousePos&) { /* sendEvent(EVENT_GAME_FULLSTATE_REQUEST); */ (void)down; }));
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

bool GameSessionInput::mouseDown(Arya::MOUSEBUTTON button, bool buttonDown, const MousePos& pos)
{
    int x = pos.x, y = pos.y;
    if(button == Arya::MOUSEBUTTON_LEFT)
    {
        draggingLeftMouse = (buttonDown == true);

        if (!buttonDown)
        {
            auto gr = Arya::Locator::getRoot().getGraphics();
            auto cam = gr->getCamera();
            vec3 p = cam->intersectViewRay(gr->normalizeMouseCoordinates(x,y), vec4(0.0f, 0.0f, 1.0f, 0.0f));

            p.z += 0.02f;
            if (session && session->debugEntity)
                session->debugEntity->setPosition(p);

            return true;
        }
    }
    else if(button == Arya::MOUSEBUTTON_RIGHT)
    {
        draggingRightMouse = (buttonDown == true);
        
        if (buttonDown)
            LogDebug << "Rightclicked at (x,y) = (" << x << ',' << y << ')' << endLog;
    }
    return false;
}

void GameSessionInput::mouseWheelMoved(int delta)
{
    Camera* cam = Arya::Locator::getRoot().getGraphics()->getCamera();
    if (!cam) return;
    cam->camZoomSpeed -= 50.0f*delta;
    return;
}

void GameSessionInput::mouseMoved(const MousePos& pos, int dx, int dy)
{
    (void)dx; (void)dy;
    int padding = 10;

    mouseLeft = (pos.x < padding);
    mouseBot = (pos.y < padding);
    mouseRight = (pos.x > Arya::Locator::getRoot().getWindowWidth() - padding);
    mouseTop = (pos.y > Arya::Locator::getRoot().getWindowHeight() - padding);

    computeForce();
    return;
}

