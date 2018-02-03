#ifndef PLATFORMER_CONNECT_H
#define PLATFORMER_CONNECT_H

#include "GameState.h"
#include "Game.h"

namespace platformer {
    class Connect : public GameState {
    private:
        static const int TICK_RATE = 500;

        Game *game;
    public:
        explicit Connect(Game *game);

        void onButtonAPress() override;

        void onButtonBPress() override;

        void onButtonABPress() override;

        void onMessage(ByteBuf &in) override;

        void run() override;

        void stop();
    };
}

#endif //PLATFORMER_CONNECT_H
