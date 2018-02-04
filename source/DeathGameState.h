#ifndef PLATFORMER_DEATH_GAME_STATE_H
#define PLATFORMER_DEATH_GAME_STATE_H

#include "Game.h"

namespace platformer {

    /**
     * Game state that flashes the whole screen {@link Game#GAME_OVER_FLASHES}
     * times. Then switches back to the {@link PlayGameState} in the starting
     * position with a fresh world and score. If the opponent ever completes the
     * current world during this state, we automatically loose.
     */
    class DeathGameState : public GameState {
    private:
        // Duration in millis between game ticks.
        static const int TICK_RATE = 200;

        // Number of screen flashes to display.
        static const int FLASH_COUNT = 5;

        // The currently running game.
        Game *game;

        // ID of the wast world played.
        int worldId;

        // Screen flash counter.
        int counter = 0;

    public:
        /**
         * Constructs a new {@link DeathGameState}.
         *
         * @param game the currently running {@link Game}.
         * @param worldId the world ID that is currently being played.
         */
        DeathGameState(Game *game, int worldId);

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
         * Ticks the death game state, toggles whether the LEDs should be on or off.
         */
        void tick();

        /**
         * Renders the LEDs on or off depending on what was determined after
         * ticking.
         */
        void render() const;
    };

}

#endif //PLATFORMER_DEATH_GAME_STATE_H
