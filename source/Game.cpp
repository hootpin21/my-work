#include "Game.h"

namespace platformer {

    namespace {
        const int mapX = 21;
        const int mapY = 7;

        char map[mapY][mapX] = {
                "---------.----------",
                "-------------.------",
                "------------------=-",
                "----.---+++------==-",
                "-----++-----------=-",
                "--.---------++----=-",
                "++++------------++++"
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
        Vector2i relativeCenter = getRelativeLocation(0, 0);

        BlockType center = getBlock(&relativeCenter);
        BlockType below = getRelativeBlock(0, 1);
        BlockType left = getRelativeBlock(-1, 0);
        BlockType right = getRelativeBlock(1, 0);

        if (center == COIN_OFF || center == COIN_ON) {
            map[relativeCenter.getY()][relativeCenter.getX()] = AIR;
            score++;
        }

        if (below != FOREGROUND) {
            velocity->addY(-1);
        }

        int accelerometerX = microBit->accelerometer.getX();

        if (accelerometerX < -300 && location->getX() > 0 && left != FOREGROUND) {
            location->addX(-1);
        }

        if (accelerometerX > 300 && location->getX() < (mapX - 2) && right != FOREGROUND) {
            location->addX(1);
        }

        if (velocity->getY() > 0) {
            location->addY(1);
        } else if (velocity->getY() < 0) {
            if (below == FOREGROUND) {
                velocity->setY(0);
            } else {
                location->addY(-1);
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

        if (location->getX() >= (mapX - HALF_SCREEN - 1)) {
            offsetX += HALF_SCREEN - ((mapX - 2) - location->getX());
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
        Vector2i blockLocation = getRelativeLocation(x - offsetX, y - offsetY);
        BlockType blockType = getBlock(&blockLocation);

        switch (blockType) {
            case AIR:
                break;
            case FOREGROUND:
            case BACKGROUND:
                screen->setPixelValue((uint16_t) x, (uint16_t) y, 255);
                break;
            case COIN_ON:
                screen->setPixelValue((uint16_t) x, (uint16_t) y, 255);
                map[blockLocation.getY()][blockLocation.getX()] = COIN_OFF;
                break;
            case COIN_OFF:
                map[blockLocation.getY()][blockLocation.getX()] = COIN_ON;
                break;
        }
    }

    Vector2i Game::getRelativeLocation(int offsetX, int offsetY) const {
        int relativeX = offsetX + player->getLocation()->getX();
        int relativeY = offsetY + ((mapY - 1) - player->getLocation()->getY());
        return {relativeX, relativeY};
    }

    BlockType Game::getRelativeBlock(int offsetX, int offsetY) const {
        Vector2i location = getRelativeLocation(offsetX, offsetY);
        BlockType blockType = getBlock(&location);
        return blockType;
    }

    BlockType Game::getBlock(const Vector2i *location) const {
        if (location->getX() < 0 || location->getX() >= (mapX - 1) ||
            location->getY() < 0 || location->getY() >= mapY) {
            return AIR;
        }
        return (BlockType) map[location->getY()][location->getX()];
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
        player->getLocation()->set(1, 5);
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
