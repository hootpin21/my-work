#ifndef PLATFORMER_MENU_GAME_STATE_H
#define PLATFORMER_MENU_GAME_STATE_H

#include "Game.h"

namespace platformer {

    /**
     * Game state that grants the ability to toggle multiplayer mode and a
     * selection of different worlds.
     */
    class MenuGameState : public GameState {
    private:
        // Duration in millis to sleep between ticks.
        static const int TICK_RATE = 450;

        // The currently running game.
        Game *game;

        // One-indexed world ID currently selected.
        int selectedWorld = 1;

    public:
        /**
         * Constructs a new {@link MenuGameState}.
         *
         * @param game the currently running {@link Game}.
         */
        explicit MenuGameState(Game *game);

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
         * Updates the world selection depending on the accelerometer.
         */
        void tick();

        /**
         * Displays the world currently selected.
         */
        void render() const;
    };

}

#endif //PLATFORMER_MENU_GAME_STATE_H
