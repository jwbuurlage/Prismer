#include "Game.h"

int main()
{
    auto game = new Prismer::Game();

    if(!game->init()) {
        return -1;
    }

    game->run();

    return 0;
}
