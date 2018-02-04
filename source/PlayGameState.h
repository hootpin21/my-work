#ifndef PLATFORMER_PLAY_GAME_STATE_H
#define PLATFORMER_PLAY_GAME_STATE_H

#include "Player.h"
#include "World.h"
#include "Game.h"

namespace platformer {

    /**
     * Game state for actually playing the game. Collect coins to increase
     * score, reach the flag to complete the game. The player with the highest
     * score wins.
     */
    class PlayGameState : public GameState {
    private:
        // Duration in millis to sleep between ticks.
        static const int TICK_RATE = 200;

        // Center coordinates (X/Y are the same) of the screen.
        // Used for relational rendering to the player.
        static const int SCREEN_CENTER = 2;

        // Our current score.
        int score = 0;

        // The score of our opponent.
        int partnerScore = -1;

        // Whether our opponent has completed the world.
        bool partnerComplete = false;

        // The player.
        Player *player = new Player();

        // The world currently being played.
        World *world;

        // The currently running game.
        Game *game;

        // Inverted once every tick to create coin flashing effect.
        bool displayCoins = false;

    public:
        /**
         * Constructs a new {@link PlayGameState}.
         *
         * @param game the currently running {@link Game}.
         * @param world the mutable world to play, coins are removed when
         *              running.
         */
        explicit PlayGameState(Game *game, World *world);

        /**
         * Destructs the game state, deleting both the player and the current
         * world.
         */
        ~PlayGameState() override;

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
         * Called whenever button B is pressed, makes the player jump.
         */
        void jump();

        /**
         * {@inheritDoc}
         */
        void run() override;

        /**
         * Handles all player movements, velocities, and collisions.
         */
        void tick();

        /**
         * Handles when the player has completed the world. Waits for opponent
         * to complete when in multiplayer. Prints out the final score and
         * whether they were the winner of this game.
         */
        void handleCompletion() const;

        /**
         * Handles when the player has fallen off the map and died. Either
         * displays the game over screen by temporarily changing state and
         * resets the map, or finishes the game if the opponent has already
         * completed the game.
         */
        void handleDeath() const;

        /**
         * Renders the player and world to the LEDs in relation to the players
         * current location.
         */
        void render() const;

        /**
         * Renders a block to the screen, taking the X and Y coordinates of the
         * LED with an origin of bottom-left. Offsets where the block is
         * rendered based on where the player is rendered.
         *
         * @param offsetX the player position offset X.
         * @param offsetY the player position offset Y.
         * @param x the LED X position (origin at bottom-left).
         * @param y the LED Y position (origin at bottom-left).
         */
        void renderBlock(int offsetX, int offsetY, int x, int y) const;
    };

}

#endif //PLATFORMER_PLAY_GAME_STATE_H
