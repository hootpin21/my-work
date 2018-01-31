#ifndef PLATFORMER_PLAYGAMESTATE_H
#define PLATFORMER_PLAYGAMESTATE_H

#include "Player.h"
#include "World.h"
#include "Game.h"

namespace platformer {

    class PlayGameState : public GameState {
    private:
        static const int TICK_RATE = 200;
        static const int HALF_SCREEN = 2; // Used for relational rendering to player.

        int score = 0;
        Player *player = new Player();
        World *world = createWorld1();
        Game *game;
        bool displayCoins = false;
    public:
        explicit PlayGameState(Game *game);

        ~PlayGameState() override;

        void onButtonAPress() override;

        void onButtonBPress() override;

        void jump();

        void run() override;

        void tick();

        void render();

        void renderBlock(int offsetX, int offsetY, int x, int y) const;
    };

}

#endif //PLATFORMER_PLAYGAMESTATE_H
