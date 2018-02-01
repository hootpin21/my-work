#include "Menu.h"
#include "Session.h"

namespace platformer {

    Menu::Menu(Game *game) : game(game) {}

    void Menu::onButtonAPress() {
        game->getMicroBit()->display.stopAnimation();
    }

    void Menu::onButtonBPress() {
        game->getMicroBit()->display.stopAnimation();
        auto *nextState = new Session(game, createWorld(selectedWorld));
        game->setState(nextState);
    }

    void Menu::run() {
        game->getMicroBit()->display.scroll("SELECT WORLD", 80);

        while (game->getState() == this) {
            game->getScreen()->clear();
            tick();
            render();
            game->getMicroBit()->sleep(SELECT_TICK_RATE);
        }

        delete this;
    }

    void Menu::tick() {
        int accelerometerX = game->getMicroBit()->accelerometer.getX();

        if (accelerometerX < -300) {
            selectedWorld--;

            if (selectedWorld <= 0) {
                selectedWorld = WORLD_COUNT;
            }
        }

        if (accelerometerX > 300) {
            selectedWorld++;

            if (selectedWorld > WORLD_COUNT) {
                selectedWorld = 1;
            }
        }
    }

    void Menu::render() const {
        game->getMicroBit()->display.print(selectedWorld);
    }

}
