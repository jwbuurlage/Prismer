#include <glm/glm.hpp>
#include "Arya.h"
#include <vector>

using std::vector;
using Arya::InputBinding;
using Arya::MousePos;
class GameSessionClient;

class GameSessionInput
{
    public:
        GameSessionInput(GameSessionClient* s);
        virtual ~GameSessionInput();

        void init();

        void update(float elapsedTime);

    private: 
        GameSessionClient* session;

        vector<InputBinding> bindings;

        bool mouseDown(Arya::MOUSEBUTTON button, bool buttonDown, const MousePos& pos);
        void mouseWheelMoved(int delta);
        void mouseMoved(const MousePos& pos, int dx, int dy);

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
