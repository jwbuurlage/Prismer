#include "InputSystem.h"
#include "common/Logger.h"
#include <algorithm>
#include <string>

namespace Arya {

using std::string;

// SDL keymaps
static map<string, SDL_Keycode> keyMap;
static map<string, SDL_GameControllerButton> buttonMap;

// directions are:
// InputSystem::Bindings::xx is called first
// InputSystem::Bindings::xx->next is called second
// last one in line has next==0
// if prev==0 then that chain element is the first (root) element

template<typename T>
class  CallbackChainElement : public BindingBase
{
    public:
        CallbackChainElement()
        {
            prev = 0;
            next = 0;
            f = nullptr;
        }

        ~CallbackChainElement()
        {
            if (next) { next->prev = prev; }
            if (prev) { prev->next = next; }
        }

        InputBinding addNew(const function<T>& f, CHAIN_POS pos)
        {
            auto cur = this;
            if (pos == CHAIN_LAST)
                while (cur->next) cur = cur->next;

            CallbackChainElement<T>* elem = new CallbackChainElement<T>;
            elem->f = f;

            //link it into the list, after cur
            elem->prev = cur;
            elem->next = cur->next;
            if (cur->next) cur->next->prev = elem;
            cur->next = elem;
            return unique_ptr<BindingBase>(elem);
        }

        function<T> f;
        CallbackChainElement<T>* prev;
        CallbackChainElement<T>* next;
};

struct InputSystem::Bindings{
    // These are not pointers!!!
    // The 'next' pointers of these are the first callbacks
    // This ensures that when the user deletes the 'root' callback
    // then this object is not invalidated
    CallbackChainElement<bool(int, const MousePos&)> keyDown;
    CallbackChainElement<bool(int, const MousePos&)> keyUp;
    CallbackChainElement<bool(MOUSEBUTTON, bool, const MousePos&)> mouseButton;
    CallbackChainElement<bool(const MousePos&, int, int)> mouseMove;
    CallbackChainElement<bool(int, const MousePos&)> mouseWheel;
    CallbackChainElement<bool(const char*)> textInput;

    map<InputKey, CallbackChainElement<bool(bool,const MousePos&)>> keys;
    map<InputButton, CallbackChainElement<bool(bool)>> buttons;
};

InputSystem::InputSystem()
{
    bindings = make_unique<Bindings>();

    windowWidth = 0; windowHeight = 0;
    if(keyMap.empty()) {
        //Note that a-z and 0-9 are taken care of separately
        //All strings must be lowercase!
        keyMap["space"] = SDLK_SPACE;
        keyMap["enter"] = SDLK_RETURN;
        keyMap["return"] = SDLK_RETURN;
        keyMap["escape"] = SDLK_ESCAPE;
        keyMap["esc"] = SDLK_ESCAPE;
        keyMap["plus"] = SDLK_PLUS;
        keyMap["+"] = SDLK_PLUS;
        keyMap["minus"] = SDLK_MINUS;
        keyMap["-"] = SDLK_MINUS;
        keyMap["slash"] = SDLK_SLASH;
        keyMap["/"] = SDLK_SLASH;
        keyMap["period"] = SDLK_PERIOD;
        keyMap["."] = SDLK_PERIOD;
        keyMap["comma"] = SDLK_COMMA;
        keyMap[","] = SDLK_COMMA;
        keyMap["tab"] = SDLK_TAB;
        keyMap["tilde"] = SDLK_BACKQUOTE;
        keyMap["backspace"] = SDLK_BACKSPACE;
        keyMap["f1"] = SDLK_F1;
        keyMap["f2"] = SDLK_F2;
        keyMap["f3"] = SDLK_F3;
        keyMap["f4"] = SDLK_F4;
        keyMap["f5"] = SDLK_F5;
        keyMap["f6"] = SDLK_F6;
        keyMap["f7"] = SDLK_F7;
        keyMap["f8"] = SDLK_F8;
        keyMap["f9"] = SDLK_F9;
        keyMap["f10"] = SDLK_F10;
        keyMap["f11"] = SDLK_F11;
        keyMap["f12"] = SDLK_F12;
    }

    if(buttonMap.empty()) {
        // other buttons are 'invalid' and 'max'
        buttonMap["a"] = SDL_CONTROLLER_BUTTON_A;
        buttonMap["b"] = SDL_CONTROLLER_BUTTON_B;
        buttonMap["x"] = SDL_CONTROLLER_BUTTON_X;
        buttonMap["y"] = SDL_CONTROLLER_BUTTON_Y;
        buttonMap["back"] = SDL_CONTROLLER_BUTTON_BACK;
        buttonMap["guide"] = SDL_CONTROLLER_BUTTON_GUIDE;
        buttonMap["start"] = SDL_CONTROLLER_BUTTON_START;
        buttonMap["left_stick"] = SDL_CONTROLLER_BUTTON_LEFTSTICK;
        buttonMap["right_stick"] = SDL_CONTROLLER_BUTTON_RIGHTSTICK;
        buttonMap["left_shoulder"] = SDL_CONTROLLER_BUTTON_LEFTSHOULDER;
        buttonMap["right_shoulder"] = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER;
        buttonMap["up"] = SDL_CONTROLLER_BUTTON_DPAD_UP;
        buttonMap["down"] = SDL_CONTROLLER_BUTTON_DPAD_DOWN;
        buttonMap["left"] = SDL_CONTROLLER_BUTTON_DPAD_LEFT;
        buttonMap["right"] = SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    }

}

InputSystem::~InputSystem()
{
    //all bindings are taken care of using the smart-pointers
    for (auto& handle : controllerHandles) {
        if (handle) {
            SDL_GameControllerClose(handle);
        }
    }
}

void InputSystem::initializeControllers()
{
    SDL_InitSubSystem(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

    // Initialize controller
    int maxJoysticks = SDL_NumJoysticks();
    for (int idx = 0; idx < maxJoysticks; ++idx) {
        if (!SDL_IsGameController(idx)) {
            LogWarning << "Game controller with ID: " << idx << " is not supported" << endLog;
            continue;
        }
        controllerHandles.push_back(SDL_GameControllerOpen(idx));

    }
    LogInfo << "Number of game controllers found: " << controllerHandles.size() << endLog;
    if (controllerHandles.size() > 0)
        _controllerEnabled = true;
}

InputBinding InputSystem::bind(INPUTEVENT event, function<bool(int,const MousePos&)> f, CHAIN_POS pos)
{
    if( event == INPUT_KEYDOWN )
        return bindings->keyDown.addNew(f, pos);
    else if( event == INPUT_KEYUP )
        return bindings->keyUp.addNew(f, pos);
    else if( event == INPUT_MOUSEWHEEL )
        return bindings->mouseWheel.addNew(f, pos);
    else
        LogWarning << "Trying to bind event with invalid function signature or invalid event type" << endLog;
    return nullptr;
}

InputBinding InputSystem::bindMouseButton(function<bool(MOUSEBUTTON,bool,const MousePos&)> f, CHAIN_POS pos)
{
    return bindings->mouseButton.addNew(f, pos);
}

InputBinding InputSystem::bindMouseMove(function<bool(const MousePos&,int,int)> f, CHAIN_POS pos)
{
    return bindings->mouseMove.addNew(f, pos);
}

InputBinding InputSystem::bind(const char* key, function<bool(bool,const MousePos&)> f, CHAIN_POS pos)
{
    InputKey k;
    if (!k.parseKey(key)) {
        LogWarning << "Unable to bind key. Could not parse: " << key << endLog;
        return nullptr;
    }
    // will add a new one if non-existent
    return bindings->keys[k].addNew(f, pos);
}

InputBinding InputSystem::bindControllerButton(const char* key, function<bool(bool)> f, CHAIN_POS pos)
{
    InputButton b;
    if (!b.parseButton(key)) {
        LogWarning << "Unable to bind key. Could not parse: " << key << endLog;
        return nullptr;
    }
    // will add a new one if non-existent
    return bindings->buttons[b].addNew(f, pos);
}

InputBinding InputSystem::bindTextInput(function<bool(const char*)> f, CHAIN_POS pos)
{
    auto x = bindings->textInput.addNew(f,pos);
    SDL_StartTextInput();
    //TODO: call SDL_StopTextInput() when last textInput handler is deleted
    return x;
}

MOUSEBUTTON translateButton(Uint8 btn)
{
    if( btn == SDL_BUTTON_LEFT )   return MOUSEBUTTON_LEFT;
    if( btn == SDL_BUTTON_MIDDLE ) return MOUSEBUTTON_MIDDLE;
    if( btn == SDL_BUTTON_RIGHT )  return MOUSEBUTTON_RIGHT;
    return MOUSEBUTTON_OTHER;
}

void InputSystem::handleInputEvent(const SDL_Event& event)
{
    MousePos mousePos;
    switch (event.type) {
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            mousePos.x = event.button.x;
            mousePos.y = windowHeight - event.button.y;
            break;
        case SDL_MOUSEMOTION:
            mousePos.x = event.motion.x;
            mousePos.y = windowHeight - event.motion.y;
            break;
        default:
            SDL_GetMouseState(&mousePos.x, &mousePos.y);
            break;
    }
    mousePos.nX = -1.0f + (2.0f * mousePos.x) / float(windowWidth);
    mousePos.nY = -1.0f + (2.0f * mousePos.y) / float(windowHeight);

    switch(event.type) {
        case SDL_KEYDOWN:
            {
                InputKey k(event.key.keysym);

                //first handle textinput if enabled
                if (bindings->textInput.next)
                {
                    char text[2] = {0};
                    if (k.keysym == SDLK_BACKSPACE)
                        text[0] = '\x08';
                    else if (k.keysym == SDLK_RETURN)
                        text[0] = '\r';
                    else if (k.keysym == SDLK_ESCAPE)
                        text[0] = '\x1B';
                    else
                        break; //do not handle other keys
                    auto elem = bindings->textInput.next;
                    while (elem)
                    {
                        if (elem->f(text))
                            break;
                        elem = elem->next;
                    }
                }
                else
                {
                    //Do not count repeated keys, that for text-capture mode
                    if(event.key.repeat) break;
                    auto elem = bindings->keyDown.next;
                    while (elem)
                    {
                        if (elem->f(k.keysym, mousePos))
                            break;
                        elem = elem->next;
                    }
                    auto a = bindings->keys.find(k);
                    if( a != bindings->keys.end() )
                    {
                        auto elem = a->second.next;
                        while (elem)
                        {
                            if (elem->f(true, mousePos))
                            {
                                lastHandledTimeStamp = event.key.timestamp;
                                break;
                            }
                            elem = elem->next;
                        }
                    }
                }
            }
            break;
        case SDL_KEYUP:
            {
                //Check if text input active
                if (bindings->textInput.next) break;
                //Do not count repeated keys, only for text-capture mode
                if(event.key.repeat) break;
                InputKey k(event.key.keysym);
                auto elem = bindings->keyUp.next;
                while (elem)
                {
                    if (elem->f(k.keysym, mousePos))
                        break;
                    elem = elem->next;
                }
                auto a = bindings->keys.find(k);
                if( a != bindings->keys.end() )
                {
                    auto elem = a->second.next;
                    while (elem)
                    {
                        if (elem->f(false, mousePos))
                            break;
                        elem = elem->next;
                    }
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            {
                auto elem = bindings->mouseButton.next;
                while (elem)
                {
                    if (elem->f(translateButton(event.button.button), true, mousePos))
                        break;
                    elem = elem->next;
                }
            }
            break;
        case SDL_MOUSEBUTTONUP:
            {
                auto elem = bindings->mouseButton.next;
                while (elem)
                {
                    if (elem->f(translateButton(event.button.button), false, mousePos))
                        break;
                    elem = elem->next;
                }
            }
            break;
        case SDL_MOUSEMOTION:
            {
                auto elem = bindings->mouseMove.next;
                while (elem)
                {
                    if (elem->f(mousePos, event.motion.xrel, -event.motion.yrel))
                        break;
                    elem = elem->next;
                }
            }
            break;
        case SDL_MOUSEWHEEL:
            {
                auto elem = bindings->mouseWheel.next;
                while (elem)
                {
                    if (elem->f(event.wheel.y, mousePos))
                        break;
                    elem = elem->next;
                }
            }
            break;
        case SDL_TEXTEDITING:
            //See online SDL info.
            //We currently do not support it
            break;
        case SDL_TEXTINPUT:
            {
                if (lastHandledTimeStamp == event.text.timestamp) break;
                auto elem = bindings->textInput.next;
                while (elem)
                {
                    if (elem->f(event.text.text))
                        break;
                    elem = elem->next;
                }
            }
            break;
        case SDL_CONTROLLERDEVICEADDED:
            LogWarning << "Controller added, not supported yet. Behaviour undefined." << endLog;
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            LogDebug << "Controller removed, not supported yet. Behaviour undefined." << endLog;
            break;
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            {
                bool down = event.type == SDL_CONTROLLERBUTTONDOWN;

                InputButton b((SDL_GameControllerButton)event.cbutton.button);
                auto a = bindings->buttons.find(b);
                if (a != bindings->buttons.end())
                {
                    auto elem = a->second.next;
                    while (elem)
                    {
                        if (elem->f(down))
                            break;
                        elem = elem->next;
                    }
                }
            }
            break;
       case SDL_CONTROLLERAXISMOTION:
            //LogDebug << "Controller axis motion" << endLog;
            break;

        default:
            LogWarning << "Unknown event received in InputSystem" << endLog;
            break;
    }
}

InputSystem::InputKey::InputKey(const SDL_Keysym& sdlkey)
{
    keysym = (int)sdlkey.sym;
    //remove numlock and capslock
    mod = (int)sdlkey.mod & ~(KMOD_NUM | KMOD_CAPS);
    //if ONE of the shift bits (left or right) is set, then set both
    //same for control and alt
    if (mod & KMOD_SHIFT) mod |= KMOD_SHIFT;
    if (mod & KMOD_CTRL) mod |= KMOD_CTRL;
    if (mod & KMOD_ALT) mod |= KMOD_ALT;
}

bool InputSystem::InputKey::parseKey(const char* _key)
{
    if (_key == 0 || _key[0] == 0) return false;
    string key(_key);
    //remove spaces and transform to lowercase
    key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    keysym = 0;
    mod = 0;

    //If the string contains any of the substrings shown below
    //then remove it from the string. At the end there
    //should be only one character left which will be the letter
    string::size_type pos;
    pos = key.find("shift+");
    if (pos != string::npos) {
        mod |= KMOD_SHIFT;
        key.erase(pos,6);
    }
    pos = key.find("ctrl+");
    if (pos != string::npos) {
        mod |= KMOD_CTRL;
        key.erase(pos,5);
    }
    pos = key.find("alt+");
    if (pos != string::npos) {
        mod |= KMOD_ALT;
        key.erase(pos,4);
    }
    pos = key.find("s+");
    if (pos != string::npos) {
        mod |= KMOD_SHIFT;
        key.erase(pos,2);
    }
    pos = key.find("c+");
    if (pos != string::npos) {
        mod |= KMOD_CTRL;
        key.erase(pos,2);
    }
    pos = key.find("a+");
    if (pos != string::npos) {
        mod |= KMOD_ALT;
        key.erase(pos,2);
    }
    //Now parse the remaining part: the actual key
    if (key.size() == 1 && (
                (key[0] >= 'a' && key[0] <= 'z')
                || (key[0] >= '0' && key[0] <= '9') )) {
        keysym = (int)key[0];
    } else {
        auto iter = keyMap.find(key);
        if (iter != keyMap.end()) {
            keysym = iter->second;
        } else {
            return false;
        }
    }
    return true;
}

bool InputSystem::InputButton::parseButton(const char* _key)
{
    if (_key == 0 || _key[0] == 0) return false;
    string key(_key);

    // to general position (no spaces, lowercase)
    key.erase(std::remove_if(key.begin(), key.end(), isspace), key.end());
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    button = SDL_CONTROLLER_BUTTON_INVALID;

    auto iter = buttonMap.find(key);
    if (iter != buttonMap.end()) {
        button = iter->second;
    } else {
        return false;
    }

    return true;
}


} // namespace Arya
