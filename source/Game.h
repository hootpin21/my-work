#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <cstdint>
#include "Player.h"
#include "MicroBit.h"

namespace platformer {

    class Game {
    private:
        static const int TICK_RATE = 200;

        MicroBit *microBit = new MicroBit();
        MicroBitImage *screen = new MicroBitImage((int16_t) 5, (int16_t) 5);
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
    };

} // namespace platformer

#endif // PLATFORMER_GAME_H
