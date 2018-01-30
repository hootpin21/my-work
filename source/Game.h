#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <cstdint>
#include "Player.h"
#include "MicroBit.h"

namespace platformer {

    enum BlockType : char {
        AIR = '-',
        FOREGROUND = '+',
        BACKGROUND = '=',
        COIN_ON = '.',
        COIN_OFF = ','
    };

    class Game {
    private:
        static const int TICK_RATE = 200;
        static const int SCREEN_SIZE = 5; // Both X/Y sizes are equal on the micro:bit.
        static const int HALF_SCREEN = 2; // Used for relational rendering to player.

        MicroBit *microBit = new MicroBit();
        MicroBitImage *screen = new MicroBitImage(SCREEN_SIZE, SCREEN_SIZE);
        Player *player = new Player();
        int state = 0;
        int score = 0;
    public:
        virtual ~Game();

        void gameOver();

        void jump(MicroBitEvent event);

        void tick();

        void render();

        void gameLoop();

        void run();

        MicroBit *getMicroBit() const;

        MicroBitImage *getScreen() const;

        Player *getPlayer() const;

        int getState() const;

        void setState(int state);

        int getScore() const;

        void setScore(int score);

        bool complete();

        Vector2i getRelativeLocation(int offsetX, int offsetY) const;

        BlockType getRelativeBlock(int offsetX, int offsetY) const;

        BlockType getBlock(const Vector2i *location) const;

        void renderBlock(int offsetX, int offsetY, int x, int y) const;
    };

} // namespace platformer

#endif // PLATFORMER_GAME_H
