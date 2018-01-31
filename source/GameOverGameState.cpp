#include "GameOverGameState.h"
#include "PlayGameState.h"

namespace platformer {

    GameOverGameState::GameOverGameState(platformer::Game *game) : game(game) {}

    void GameOverGameState::onButtonAPress() {
    }

    void GameOverGameState::onButtonBPress() {
    }

    void GameOverGameState::run() {
        gameOverTicks = 0;

        while (game->getState() == this) {
            game->getScreen()->clear();
            tick();
            render();
            game->getMicroBit()->display.image.paste(*game->getScreen());
            game->getMicroBit()->sleep(TICK_RATE);
        }

        delete this;
    }

    void GameOverGameState::tick() {
        gameOverTicks++;

        if (gameOverTicks >= GAME_OVER_FLASHES) {
            auto *nextState = new PlayGameState(game);
            game->setState(nextState);
        }
    }

    void GameOverGameState::render() const {
        if (gameOverTicks % 2 == 0) {
            return;
        }

        for (uint16_t x = 0; x < SCREEN_SIZE; x++) {
            for (uint16_t y = 0; y < SCREEN_SIZE; y++) {
                game->getScreen()->setPixelValue(x, y, 255);
            }
        }
    }

}
