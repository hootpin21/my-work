#ifndef PLATFORMER_WIN_GAME_STATE_H
#define PLATFORMER_WIN_GAME_STATE_H

#include "GameState.h"
#include "Game.h"

namespace platformer {
    class Menu : public GameState {
    private:
        static const int DISPLAY_TICK_RATE = 200;
        static const int SELECT_TICK_RATE = 450;

        Game *game;
        int selectedWorld = 1;
    public:
        explicit Menu(Game *game);

        void onButtonAPress() override;

        void onButtonBPress() override;

        void run() override;

        void tick();

        void render() const;
    };
}

#endif //PLATFORMER_WIN_GAME_STATE_H
