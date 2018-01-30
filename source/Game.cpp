#include "Game.h"

namespace platformer {

    namespace {
        const int mapX = 10;
        const int mapY = 7;

        char map[mapY][mapX] = {
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 1, 1, 1, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {0, 0, 0, 0, 0, 0, 0, 0, 1, 0},
                {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

        // This is an incredibly gross hack to get things working. Apparently
        // the API dislikes function pointers to class members.
        // TODO: Figure out how to implement this more elegantly.
        Game *game;

        void jumpHelper(MicroBitEvent event) {
            game->jump(event);
        }

        void gameLoopHelper() {
            game->gameLoop();
        }
    }

    void Game::gameOver() {
        microBit->display.clear();
        microBit->display.scroll("GAME OVER! SCORE:");
        microBit->display.scroll(score);
    }

    void Game::jump(MicroBitEvent) {
        player->jump();
    }

    void Game::tick() {
        Vector2i *location = player->getLocation();
        Vector2i *velocity = player->getVelocity();

        if (location->getY() > 1) {
            velocity->addY(-1);
        }

        int accelerometerX = microBit->accelerometer.getX();

        if (accelerometerX < -300 && location->getX() > 0) {
            location->addX(-1);
        }

        if (accelerometerX > 300 && location->getX() < (mapX -1)) {
            location->addX(1);
        }

        if (velocity->getY() > 0) {
            location->addY(1);
        } else if (velocity->getY() < 0) {
            location->addY(-1);

            if (location->getY() < 1) {
                location->setY(1);
                velocity->setY(0);
            }
        }

        if (velocity->getX() > 0) {
            location->addX(1);
        } else if (velocity->getX() < 0) {
            location->addX(-1);
        }

        microBit->sleep(TICK_RATE);
    }

    void Game::render() {
        // Ensure the screen is clear.
        screen->clear();

        // Render the player.
        Vector2i *location = player->getLocation();
        int offsetX = HALF_SCREEN;
        int offsetY = HALF_SCREEN;

        if (location->getY() <= HALF_SCREEN) {
            offsetY += HALF_SCREEN - location->getY();
        }

        if (location->getY() >= (mapY - HALF_SCREEN)) {
            offsetY -= HALF_SCREEN - ((mapY - 1) - location->getY());
        }

        if (location->getX() <= HALF_SCREEN) {
            offsetX -= HALF_SCREEN - location->getX();
        }

        if (location->getX() >= (mapX - HALF_SCREEN)) {
            offsetX += HALF_SCREEN - ((mapX - 1) - location->getX());
        }

        screen->setPixelValue((uint16_t) offsetX, (uint16_t) offsetY, 255);

        // Render the map.
        for (int x = 0; x < SCREEN_SIZE; x++) {
            for (int y = 0; y < SCREEN_SIZE; y++) {
                // Get the relative map coordinates for the players position.
                int relativeX = (x - offsetX) + location->getX();
                int relativeY = (y - offsetY) + ((mapY - 1) - location->getY());

                // Do not render unmapped coordinates.
                if (relativeX < 0 || relativeX >= mapX || relativeY < 0 || relativeY >= mapY) {
                    continue;
                }

                if (map[relativeY][relativeX] == 1) {
                    screen->setPixelValue((uint16_t) x, (uint16_t) y, 255);
                }
            }
        }
    }

    void Game::gameLoop() {
        while (!state) {
            tick();
            render();
        }
    }

    void Game::run() {
        // Reset all game state.
        state = 0;
        score = 0;
        player->getLocation()->set(2, 3);
        screen->clear();

        // Spawn fiber to handle the game loop.
        game = this;
        create_fiber(gameLoopHelper);

        // Register event handlers for button presses.
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, jumpHelper);
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, jumpHelper);

        // Now just keep the screen refreshed.
        while (!state) {
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
