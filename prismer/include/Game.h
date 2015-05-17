#include "Arya.h"

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

        GameSessionClient* session;
};
