#ifndef PLATFORMER_GAME_OVER_GAME_STATE_H
#define PLATFORMER_GAME_OVER_GAME_STATE_H

#include "Game.h"

namespace platformer {

    class GameOver : public GameState {
    private:
        static const int TICK_RATE = 200;
        static const int GAME_OVER_FLASHES = 5;

        Game *game;
        int worldId;
        int gameOverTicks = 0;
    public:
        GameOver(Game *game, int worldId);

        void onButtonAPress() override;

        void onButtonBPress() override;

        void onButtonABPress() override;

        void onMessage(ByteBuf &in) override;

        void run() override;

        void tick();

        void render() const;
    };

}

#endif //PLATFORMER_GAME_OVER_GAME_STATE_H
