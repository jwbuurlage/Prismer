#include "InputSystem.h"
#include "common/Logger.h"
#include <string>
#include <algorithm>
#include <SDL2/SDL.h>
using std::string;

namespace Arya
{
    map <string,SDL_Keycode> keyMap;

    InputSystem::InputSystem()
    {
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
            keyMap["tab"] = SDLK_TAB;
            keyMap["tilde"] = SDLK_BACKQUOTE;
        }
    }

    InputSystem::~InputSystem()
    {
    }

    void InputSystem::bind(INPUTEVENT event, function<void(int)> f)
    {
        if( event == INPUT_KEYDOWN )
            bindingKeyDown = f;
        else if( event == INPUT_KEYUP )
            bindingKeyUp = f;
        else if( event == INPUT_MOUSEWHEEL )
            bindingMouseWheel = f;
        else
            LogWarning << "Trying to bind event with invalid function signature or invalid event type" << endLog;
    }

    void InputSystem::bindMouseButton(function<void(MOUSEBUTTON,bool,int,int)> f)
    {
        bindingMouseBtn = f;
    }

    void InputSystem::bindMouseMove(function<void(int,int,int,int)> f)
    {
        bindingMouseMovement = f;
    }

    void InputSystem::bind(const char* key, function<void(bool)> f)
    {
        InputKey k;
        if (!k.parseKey(key)) {
            LogWarning << "Unable to bind key. Could not parse: " << key << endLog;
            return;
        }
        if (bindingKey.find(k) != bindingKey.end()) LogInfo << "Rebinding key '" << key << "' (old binding removed)" << endLog;
        bindingKey[k] = f;
        return;
    }

    void InputSystem::unbind(INPUTEVENT event)
    {
        switch(event) {
            case INPUT_KEYDOWN:
                bindingKeyDown = nullptr;
                break;
            case INPUT_KEYUP:
                bindingKeyUp = nullptr;
                break;
            case INPUT_MOUSEBUTTON:
                bindingMouseBtn = nullptr;
                break;
            case INPUT_MOUSEMOVEMENT:
                bindingMouseMovement = nullptr;
                break;
            case INPUT_MOUSEWHEEL:
                bindingMouseWheel = nullptr;
                break;
            default:
                break;
        }
    }

    void InputSystem::unbind(const char* key)
    {
        InputKey k;
        if (!k.parseKey(key)) {
            LogWarning << "Cannot unbind key. Unable to parse: " << key << endLog;
            return;
        }
        auto f = bindingKey.find(k);
        if (f == bindingKey.end()) {
            LogInfo << "Trying to unbind '" << key << "' but the binding did not exist." << endLog;
            return;
        }
        bindingKey.erase(f);
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
        switch(event.type) {
            case SDL_KEYDOWN:
                {
                    //Do not count repeated keys, only for text-capture mode
                    if(event.key.repeat) break;
                    InputKey k(event.key.keysym);
                    if( bindingKeyDown ) bindingKeyDown(k.keysym);
                    auto f = bindingKey.find(k);
                    if( f != bindingKey.end() ) f->second(true);
                }
                break;
            case SDL_KEYUP:
                {
                    //Do not count repeated keys, only for text-capture mode
                    if(event.key.repeat) break;
                    InputKey k(event.key.keysym);
                    if( bindingKeyUp ) bindingKeyUp(k.keysym);
                    auto f = bindingKey.find(k);
                    if( f != bindingKey.end() ) f->second(false);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if( bindingMouseBtn ) {
                    bindingMouseBtn(translateButton(event.button.button), true,
                            event.button.x, windowHeight - event.button.y);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if( bindingMouseBtn ) {
                    bindingMouseBtn(translateButton(event.button.button), false,
                            event.button.x, windowHeight - event.button.y);
                }
                break;
            case SDL_MOUSEMOTION:
                if( bindingMouseMovement ) {
                    bindingMouseMovement(event.motion.x, windowHeight - event.motion.y, event.motion.xrel, -event.motion.yrel);
                }
                break;
            case SDL_MOUSEWHEEL:
                if( bindingMouseWheel ) {
                    bindingMouseWheel(event.wheel.y);
                }
                break;
            case SDL_TEXTINPUT:
                //TODO
                //If the key pressed is a letter or any other
                //text based input, SDL gives this event
                //which can be used for properly using textboxes
                //so that keys like ^ ' ~ work as expected
                break;
            default:
                LogWarning << "Unkown event received in InputSystem" << endLog;
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

}
