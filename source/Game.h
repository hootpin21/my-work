#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <cstdint>
#include "Player.h"
#include "MicroBit.h"
#include "World.h"

namespace platformer {

    enum GameState {
        INIT,
        RUNNING,
        GAME_OVER,
        RESET,
        COMPLETE
    };

    class Game {
    private:
        static const int TICK_RATE = 200;
        static const int SCREEN_SIZE = 5; // Both X/Y sizes are equal on the micro:bit.
        static const int HALF_SCREEN = 2; // Used for relational rendering to player.
        static const int GAME_OVER_FLASHES = 5;

        MicroBit *microBit = new MicroBit();
        MicroBitImage *screen = new MicroBitImage(SCREEN_SIZE, SCREEN_SIZE);
        Player *player = new Player();
        World *world = createWorld1();
        GameState state = INIT;
        int score = 0;
        int gameOverTicks = 0;
    public:
        virtual ~Game();

        void jump(MicroBitEvent event);

        void tickRunning();

        void tickGameOver();

        void run();

        MicroBit *getMicroBit() const;

        MicroBitImage *getScreen() const;

        Player *getPlayer() const;

        GameState getState() const;

        void setState(GameState state);

        int getScore() const;

        void setScore(int score);

        void renderBlock(int offsetX, int offsetY, int x, int y) const;

        void renderRunning() const;

        void renderGameOver() const;
    };

} // namespace platformer

#endif // PLATFORMER_GAME_H
