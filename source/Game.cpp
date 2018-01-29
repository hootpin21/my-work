#include "Game.h"

namespace platformer {

    namespace {
//        int map[8][10] = {
//                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                {0, 0, 0, 1, 1, 1, 0, 0, 0, 0},
//                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
//                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
//        };

        // This is an incredibly gross hack to get things working. Apparently
        // the API dislikes function pointers to class members.
        // TODO: Figure out how to implement this more elegantly.
        Game *game;

        void jumpHelper(MicroBitEvent event) {
            game->jump(event);
        }

        void mechanicsHelper() {
            game->mechanics();
        }
    }

    void Game::gameOver() {
        microBit->display.clear();
        microBit->display.scroll("GAME OVER! SCORE:");
        microBit->display.scroll(score);
    }

    void Game::tick() {
        Vector2i *location = player->getLocation();

        if (!player->isOnGround()) {
            location->addY(-1);
        }

        int accelerometerX = microBit->accelerometer.getX();

        if (accelerometerX < -300 && location->getX() > 0) {
            location->addX(-1);
        }

        if (accelerometerX > 300 && location->getX() < 4) {
            location->addX(1);
        }

        microBit->sleep(TICK_SPEED);
    }

    void Game::jump(MicroBitEvent) {
        player->jump();
    }

    void Game::mechanics() {
        while (!state) {
            tick();
        }
    }

    void Game::run() {
        // Reset all game state.
        state = 0;
        score = 0;
        player->getLocation()->set(2, 3);

        // Ensure the screen is clear.
        screen->clear();

        // Spawn fiber to handle the game mechanics.
        game = this;
        create_fiber(mechanicsHelper);

        // Register event handlers for button presses.
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, jumpHelper);
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, jumpHelper);

        // Now just keep the screen refreshed.
        while (!state) {
            Vector2i *location = player->getLocation();
            screen->setPixelValue((int16_t) location->getX(), (int16_t) location->getY(), 255);
            microBit->display.image.paste(*screen);
            microBit->sleep(10);
        }

        // Display GAME OVER and score
        gameOver();
    }

    Game::~Game() {
        delete microBit;
        delete screen;
        delete player;
    }

    MicroBit *Game::getMicroBit() const {
        return microBit;
    }

    MicroBitImage *Game::getScreen() const {
        return screen;
    }

    Player *Game::getPlayer() const {
        return player;
    }

    int Game::getState() const {
        return state;
    }

    void Game::setState(int state) {
        Game::state = state;
    }

    int Game::getScore() const {
        return score;
    }

    void Game::setScore(int score) {
        Game::score = score;
    }

    bool Game::complete() {
        return false;
    }

} // namespace platformer
