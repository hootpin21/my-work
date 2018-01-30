#include "Game.h"

int main() {
    // Initialise the micro:bit runtime.
    auto *game = new platformer::Game();
    game->getMicroBit()->init();

    // Welcome message
//    game->getMicroBit()->display.scroll("PLATFORMER!", 80);

    // Keep playing until complete
    while (!game->complete()) {
        game->run();
    }

    delete game;
}
