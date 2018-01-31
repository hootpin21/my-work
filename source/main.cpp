#include "Game.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma ide diagnostic ignored "OCDFAInspection"

int main(int argc, char *argv[]) {
    // Initialise the micro:bit runtime.
    auto *game = new platformer::Game();
    game->getMicroBit()->init();

    // Continuously run the game.
    while (true) {
        game->run();
    }

    delete game;
    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop
