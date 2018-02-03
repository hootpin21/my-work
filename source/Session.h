#ifndef PLATFORMER_PLAY_GAME_STATE_H
#define PLATFORMER_PLAY_GAME_STATE_H

#include "Player.h"
#include "World.h"
#include "Game.h"

namespace platformer {

    class Session : public GameState {
    private:
        static const int TICK_RATE = 200;
        static const int HALF_SCREEN = 2; // Used for relational rendering to player.

        int score = 0;
        int partnerScore = -1;
        bool partnerComplete = false;
        Player *player = new Player();
        World *world;
        Game *game;
        bool displayCoins = false;
    public:
        explicit Session(Game *game, World *world);

        ~Session() override;

        void onButtonAPress() override;

        void onButtonBPress() override;

        void onButtonABPress() override;

        void onMessage(ByteBuf &in) override;

        void jump();

        void run() override;

        void tick();

        void render() const;

        void renderBlock(int offsetX, int offsetY, int x, int y) const;

        void handleCompletion() const;

        void handleDeath() const;
    };

}

#endif //PLATFORMER_PLAY_GAME_STATE_H
