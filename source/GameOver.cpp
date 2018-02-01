#include "GameOver.h"
#include "Session.h"
#include "Menu.h"

namespace platformer {

    GameOver::GameOver(Game *game) : game(game) {}

    void GameOver::onButtonAPress() {
        game->getMicroBit()->display.stopAnimation();
        auto *nextState = new Menu(game);
        game->setState(nextState);
    }

    void GameOver::onButtonBPress() {
    }

    void GameOver::run() {
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

    void GameOver::tick() {
        gameOverTicks++;

        if (gameOverTicks >= GAME_OVER_FLASHES) {
            auto *nextState = new Session(game, createWorld(0));
            game->setState(nextState);
        }
    }

    void GameOver::render() const {
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
