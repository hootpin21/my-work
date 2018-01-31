#include <stdexcept>
#include "Game.h"
#include "PlayGameState.h"

namespace platformer {

    namespace {
        Game *game = nullptr; // Singleton reference.

        void onButtonAPress(MicroBitEvent) {
            game->getState()->onButtonAPress();
        }

        void onButtonBPress(MicroBitEvent) {
            game->getState()->onButtonBPress();
        }
    }

    void Game::run() {
        // Register event handlers for button presses.
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonAPress);
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBPress);

        // Now enter the game loop.
        state = new PlayGameState(this);

        while (state != nullptr) {
            state->run();
        }
    }

    Game::Game() {
        game = this;
    }

    Game::~Game() {
        delete microBit;
        delete screen;
    }

    MicroBit *Game::getMicroBit() const {
        return microBit;
    }

    MicroBitImage *Game::getScreen() const {
        return screen;
    }

    GameState *Game::getState() const {
        return state;
    }

    void Game::setState(GameState *state) {
        Game::state = state;
    }

}
