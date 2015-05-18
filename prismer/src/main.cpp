#include "Game.h"

int main()
{
    Prismer::Game game;

    if(!game.init()) {
        return -1;
    }

    game.run();

    return 0;
}
