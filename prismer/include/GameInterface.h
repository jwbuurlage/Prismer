// interface is owned by game session client
// should read global states such as 'messages' and 'errors'
// and 'bindings', and show relevant information on screen.
//
// would be perfect if this would be scriptable

#pragma once

#include <array>

namespace Arya {
    class View;
    class Label;
}

namespace Prismer {

using std::array;

class GameInterface {
    public:
        GameInterface();
        ~GameInterface();

        void toggleFPS();
        void update(float elapsedTime);

    private:
        shared_ptr<Arya::View> _interfaceView;

        // FPS counter
        bool _showFPS = false;
        shared_ptr<Arya::ImageView> _fpsView;
        shared_ptr<Arya::Label> _fpsCounterLabel;
        array<float, 100> _elapsedTimes;
};

} // namespace Prismer
