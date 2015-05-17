#include "Arya.h"
#include <memory>

namespace Prismer {

using std::shared_ptr;
using std::make_shared;

class GameSessionClient;

class Game
{
    public:
        Game();
        ~Game();

        bool init();
        void run();

    private:
        Arya::Root* root;
        
        void update(float dt);

        float totalTime;

        shared_ptr<GameSessionClient> session;
};

} // namespace Prismer
