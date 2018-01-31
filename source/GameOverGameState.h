#ifndef PLATFORMER_GAMEOVERGAMESTATE_H
#define PLATFORMER_GAMEOVERGAMESTATE_H

#include "Game.h"

namespace platformer {

    class GameOverGameState : public GameState {
    private:
        static const int TICK_RATE = 200;
        static const int GAME_OVER_FLASHES = 5;

        Game *game;
        int gameOverTicks = 0;
    public:
        explicit GameOverGameState(Game *game);

        void onButtonAPress() override;

        void onButtonBPress() override;

        void run() override;

        void tick();

        void render();
    };

}

#endif //PLATFORMER_GAMEOVERGAMESTATE_H
