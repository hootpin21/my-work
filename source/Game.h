#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <cstdint>
#include "Player.h"
#include "MicroBit.h"

#define TICK_SPEED 50

namespace platformer {

    class Game {
    private:
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

        void mechanics();

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
