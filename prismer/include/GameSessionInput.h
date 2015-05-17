#include <glm/glm.hpp>
#include "Arya.h"

class GameSessionInput
{
    public:
        GameSessionInput();
        virtual ~GameSessionInput();

        void init();

        void update(float elapsedTime);

    private: 
        void mouseDown(Arya::MOUSEBUTTON button, bool buttonDown, int x, int y);
        void mouseWheelMoved(int delta);
        void mouseMoved(int x, int y, int dx, int dy);

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
