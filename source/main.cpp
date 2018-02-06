#include "Game.h"

int main() {
    // Create and run the game.
    auto game = new platformer::Game();
    game->run();

    // Cleanup and exit.
    delete game;
    return EXIT_SUCCESS;
}
