#ifndef PLATFORMER_GAME_H
#define PLATFORMER_GAME_H

#include <cstdint>
#include <random>
#include "MicroBit.h"
#include "GameState.h"

namespace platformer {

    static const int GAME_ID = 0x1337;
    static const int SCREEN_SIZE = 5;

    class Game {
    private:
        MicroBit *microBit = new MicroBit();
        MicroBitImage *screen = new MicroBitImage(SCREEN_SIZE, SCREEN_SIZE);
        GameState *state;
        bool changingState = false;
        bool multiplayer = false;
        bool connected = false;
        const int id = std::rand(); // NOLINT
        int partnerId = -1;
    public:
        Game();

        virtual ~Game();

        void run();

        ByteBuf createPacket();

        void sendPacket(ByteBuf buf);

        MicroBit *getMicroBit() const;

        MicroBitImage *getScreen() const;

        GameState *getState() const;

        void setState(GameState *state);

        bool isChangingState() const;

        bool isMultiplayer() const;

        void setMultiplayer(bool multiplayer);

        bool isConnected() const;

        void setConnected(bool connected);

        int getId() const;

        int getPartnerId() const;

        void setPartnerId(int partnerId);
    };

}

#endif // PLATFORMER_GAME_H
