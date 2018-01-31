#include "Game.h"

namespace platformer {

    namespace {
        // This is an incredibly gross hack to get things working. Apparently
        // the API dislikes function pointers to class members.
        // TODO: Figure out how to implement this more elegantly.
        Game *game;

        void onButtonAPress(MicroBitEvent event) {
            game->jump(event);
        }

        void onButtonBPress(MicroBitEvent event) {
            game->jump(event);
        }
    }

    void Game::jump(MicroBitEvent) {
        BlockType below = world->getRelativeBlock(*player->getLocation(), 0, 1);
        if (below == FOREGROUND) {
            player->jump();
        }
    }

    void Game::tickRunning() {
        Vector2i *location = player->getLocation();
        Vector2i *velocity = player->getVelocity();
        Vector2i relativeCenter = world->getRelativeLocation(*player->getLocation(), 0, 0);

        BlockType center = world->getBlock(relativeCenter);
        BlockType below = world->getRelativeBlock(*player->getLocation(), 0, 1);
        BlockType left = world->getRelativeBlock(*player->getLocation(), -1, 0);
        BlockType right = world->getRelativeBlock(*player->getLocation(), 1, 0);

        if (center == COIN_OFF || center == COIN_ON) {
            world->setBlock(relativeCenter, AIR);
            score++;
        }

        if (below != FOREGROUND) {
            velocity->addY(-1);
        }

        int accelerometerX = microBit->accelerometer.getX();

        if (accelerometerX < -300 && location->getX() > 0 && left != FOREGROUND) {
            location->addX(-1);
        }

        if (accelerometerX > 300 && location->getX() < (world->getMaxX() - 2) && right != FOREGROUND) {
            location->addX(1);
        }

        if (velocity->getY() > 0) {
            location->addY(1);
        } else if (velocity->getY() < 0) {
            if (below == FOREGROUND) {
                velocity->setY(0);
            } else {
                location->addY(-1);

                if (location->getY() < 0) {
                    state = GAME_OVER;
                }
            }
        }

        if (velocity->getX() > 0) {
            location->addX(1);
        } else if (velocity->getX() < 0) {
            location->addX(-1);
        }
    }

    void Game::renderRunning() const {
        // Render the player.
        Vector2i *location = player->getLocation();
        int offsetX = HALF_SCREEN;
        int offsetY = HALF_SCREEN;

        if (location->getY() <= HALF_SCREEN) {
            offsetY += HALF_SCREEN - location->getY();
        }

        if (location->getY() >= (world->getMaxY() - HALF_SCREEN)) {
            offsetY -= HALF_SCREEN - ((world->getMaxY() - 1) - location->getY());
        }

        if (location->getX() <= HALF_SCREEN) {
            offsetX -= HALF_SCREEN - location->getX();
        }

        if (location->getX() >= (world->getMaxX() - HALF_SCREEN - 1)) {
            offsetX += HALF_SCREEN - ((world->getMaxX() - 2) - location->getX());
        }

        screen->setPixelValue((uint16_t) offsetX, (uint16_t) offsetY, 255);

        // Render the map.
        for (int x = 0; x < SCREEN_SIZE; x++) {
            for (int y = 0; y < SCREEN_SIZE; y++) {
                renderBlock(offsetX, offsetY, x, y);
            }
        }
    }

    void Game::renderBlock(int offsetX, int offsetY, int x, int y) const {
        Vector2i blockLocation = world->getRelativeLocation(*player->getLocation(), x - offsetX, y - offsetY);
        BlockType blockType = world->getBlock(blockLocation);

        switch (blockType) {
            case AIR:
                break;
            case FOREGROUND:
            case BACKGROUND:
                screen->setPixelValue((uint16_t) x, (uint16_t) y, 255);
                break;
            case COIN_ON:
                screen->setPixelValue((uint16_t) x, (uint16_t) y, 255);
                world->setBlock(blockLocation, COIN_OFF);
                break;
            case COIN_OFF:
                world->setBlock(blockLocation, COIN_ON);
                break;
        }
    }

    void Game::tickGameOver() {
        gameOverTicks++;

        if (gameOverTicks >= GAME_OVER_FLASHES) {
            state = RESET;
        }
    }

    void Game::renderGameOver() const {
        if (gameOverTicks % 2 == 0) {
            return;
        }

        for (uint16_t x = 0; x < SCREEN_SIZE; x++) {
            for (uint16_t y = 0; y < SCREEN_SIZE; y++) {
                screen->setPixelValue(x, y, 255);
            }
        }
    }

    void Game::run() {
        // Reset all game state.
        state = RESET;
        score = 0;
        gameOverTicks = 0;
        player->getLocation()->set(1, 5);
        screen->clear();
        game = this;

        // Register event handlers for button presses.
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonAPress);
        microBit->messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonBPress);

        // Now enter the game loop.
        state = RUNNING;

        while (state != RESET) {

            // Ensure the screen is clear.
            screen->clear();

            switch (state) {
                case RUNNING:
                    tickRunning();
                    renderRunning();
                    break;
                case GAME_OVER:
                    tickGameOver();
                    renderGameOver();
                    break;
                case INIT:
                    break;
                case RESET:
                    break;
                case COMPLETE:
                    break;
            }

            microBit->display.image.paste(*screen);
            microBit->sleep(TICK_RATE);
        }
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

    GameState Game::getState() const {
        return state;
    }

    void Game::setState(GameState state) {
        Game::state = state;
    }

    int Game::getScore() const {
        return score;
    }

    void Game::setScore(int score) {
        Game::score = score;
    }

} // namespace platformer
