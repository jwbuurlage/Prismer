#include "InputSystem.h"
#include "common/Logger.h"
#include <string>
#include <algorithm>
#include <SDL2/SDL.h>
using std::string;

namespace Arya
{
    // sdl keymap
    map <string,SDL_Keycode> keyMap;

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
        CallbackChainElement<bool(int,const MousePos&)> keyDown;
        CallbackChainElement<bool(int,const MousePos&)> keyUp;
        CallbackChainElement<bool(MOUSEBUTTON,bool,const MousePos&)> mouseButton;
        CallbackChainElement<bool(const MousePos&,int, int)> mouseMove;
        CallbackChainElement<bool(int,const MousePos&)> mouseWheel;

        map<InputKey,CallbackChainElement<bool(bool,const MousePos&)> > keys;
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
            keyMap["tab"] = SDLK_TAB;
            keyMap["tilde"] = SDLK_BACKQUOTE;
        }
    }

    InputSystem::~InputSystem()
    {
        //all bindings are taken care of using the smart-pointers
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
                    //Do not count repeated keys, only for text-capture mode
                    if(event.key.repeat) break;
                    InputKey k(event.key.keysym);
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
                                break;
                            elem = elem->next;
                        }
                    }
                }
                break;
            case SDL_KEYUP:
                {
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
