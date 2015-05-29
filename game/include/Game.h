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
        Arya::InputBinding bindQ;
        Arya::InputBinding bindW;
        Arya::InputBinding bindEscape;
        
        void update(float dt);

        // Total running time of the full game
        // Note that the running time of the session is in GameSessionClient
        float totalTime;

        GameSessionClient* session;
};
