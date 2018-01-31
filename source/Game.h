#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <cstdint>
#include "Player.h"
#include "MicroBit.h"
#include "World.h"
#include "GameState.h"

namespace platformer {

    static const int SCREEN_SIZE = 5;

    class Game {
    private:
        MicroBit *microBit = new MicroBit();
        MicroBitImage *screen = new MicroBitImage(SCREEN_SIZE, SCREEN_SIZE);
        GameState *state;
    public:
        Game();

        virtual ~Game();

        void run();

        MicroBit *getMicroBit() const;

        MicroBitImage *getScreen() const;

        GameState *getState() const;

        void setState(GameState *state);
    };

}

#endif // PLATFORMER_GAME_H
