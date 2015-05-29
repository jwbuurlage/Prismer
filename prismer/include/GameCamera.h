#pragma once
#include <AryaBinding.h>

namespace Prismer {

    using Arya::InputBinding;
    using Arya::MousePos;
    using Arya::CHAIN_LAST;

class GameCamera
{
    public:
        GameCamera();
        ~GameCamera();

        void update(float elapsedTime);

    private:
        bool mouseDown(Arya::MOUSEBUTTON button, bool buttonDown, const MousePos&);
        void mouseWheelMoved(int delta);
        void mouseMoved(int x, int y, int dx, int dy);

        vector<InputBinding> bindings;

        //For key movement
        bool goingForward, goingBackward, goingLeft, goingRight, goingUp, goingDown, rotatingLeft, rotatingRight;
        bool mouseLeft, mouseRight, mouseTop, mouseBot; //whether mouse is at edge
        bool draggingLeftMouse, draggingRightMouse;
        bool slowMode; //Precise movement

        void computeForce();

        vec3 forceDirection;
        vec3 specMovement;
        vec3 specPos;

        vec2 originalMousePos;
};

} // namespace Prismer
