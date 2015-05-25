#pragma once
#include <functional>
#include <map>
#include <memory>
#include "AryaBinding.h"

union SDL_Event;
struct SDL_Keysym;

namespace Arya
{
    using std::function;
    using std::map;
    using std::unique_ptr;
    using std::make_unique;

    enum INPUTEVENT
    {
        INPUT_KEYDOWN = 1,
        INPUT_KEYUP,
        INPUT_MOUSEBUTTON,
        INPUT_MOUSEMOVEMENT,
        INPUT_MOUSEWHEEL
    };

    enum MOUSEBUTTON
    {
        MOUSEBUTTON_LEFT = 1,
        MOUSEBUTTON_MIDDLE,
        MOUSEBUTTON_RIGHT,
        MOUSEBUTTON_OTHER
    };

    enum CHAIN_POS
    {
        CHAIN_FIRST = 1,
        CHAIN_LAST = 2,
    };

    struct MousePos
    {
        //x from left to right, 0 to windowWidth in pixels
        //y from BOTTOM to TOP, 0 to windowHeigt in pixels
        //nX from -1 to 1 left to right
        //nY from -1 to 1 bottom to top
        int x, y;
        float nX, nY;
    };

    class InputSystem
    {
        public:
            InputSystem();
            ~InputSystem();

            //! Register for a callback
            //! Different events require different callback prototypes
            //! KEYDOWN and KEYUP           bool(key, mousepos)
            //! MOUSEWHEEL                  bool(int delta, mousepos)
            //! MOUSEBUTTON                 bool(MOUSEBUTTON btn, bool down, mousepos)
            //! MOUSEMOVEMENT               bool(mousepos, int relativex, int relativey)
            //! key                         bool(bool down, mousepos)
            InputBinding bind(INPUTEVENT event, function<bool(int,const MousePos&)> f, CHAIN_POS pos = CHAIN_FIRST);
            InputBinding bindMouseMove(function<bool(const MousePos&,int,int)> f, CHAIN_POS pos = CHAIN_FIRST);
            InputBinding bindMouseButton(function<bool(MOUSEBUTTON,bool,const MousePos&)> f, CHAIN_POS pos = CHAIN_FIRST);
            InputBinding bind(const char* key, function<bool(bool,const MousePos&)> f, CHAIN_POS pos = CHAIN_FIRST);
            
            //! Handle an input related event generated by SDL
            //! Called by Root
            void handleInputEvent(const SDL_Event& event);

            //! Called by Root when window resizes
            void resize(int w, int h) { windowWidth = w; windowHeight = h; }
        private:
            int windowWidth, windowHeight;

            //pimpl
            struct Bindings;
            unique_ptr<Bindings> bindings;

            struct InputKey
            {
                int keysym;
                int mod;

                InputKey() : keysym(0), mod(0) {}
                InputKey(int k, int m) : keysym(k), mod(m) {}
                InputKey(const SDL_Keysym& sdlkey);
                ~InputKey(){}

                //! Parse and save into this struct
                bool parseKey(const char* key);

                bool operator<(const InputKey& rhs) const {
                    if( keysym != rhs.keysym ) return keysym < rhs.keysym;
                    if( mod != rhs.mod ) return mod < rhs.mod;
                    return false;
                }
            };

    };
}
