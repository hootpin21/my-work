#ifndef PLATFORMER_CONNECT_GAME_STATE_H
#define PLATFORMER_CONNECT_GAME_STATE_H

#include "GameState.h"
#include "Game.h"

namespace platformer {

    /**
     * Game state that attempts to find and connect to other devices to play
     * the competitive multiplayer mode.
     */
    class ConnectGameState : public GameState {
    private:
        // Duration in millis between game ticks.
        static const int TICK_RATE = 500;

        // The currently running game.
        Game *game;

    public:
        /**
         * Constructs a new {@link ConnectGameState}.
         *
         * @param game the currently running {@link Game}.
         */
        explicit ConnectGameState(Game *game);

        /**
         * {@inheritDoc}
         */
        void onButtonAPress() override;

        /**
         * {@inheritDoc}
         */
        void onButtonBPress() override;

        /**
         * {@inheritDoc}
         */
        void onButtonABPress() override;

        /**
         * {@inheritDoc}
         */
        void onMessage(ByteBuf &in) override;

        /**
         * {@inheritDoc}
         */
        void run() override;

        /**
         * Stops attempting to find and connect with other devices, and switches
         * the game back to running the menu {@link MenuGameState}.
         */
        void stop();
    };

}

#endif //PLATFORMER_CONNECT_GAME_STATE_H
