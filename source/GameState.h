#ifndef PLATFORMER_GAME_STATE_H
#define PLATFORMER_GAME_STATE_H

#include "ByteBuf.h"

namespace platformer {

    /**
     * An interface for game states. Handles both the mechanics and the
     * screen rendering, taking full control until the state must change.
     */
    class GameState {
    public:
        virtual ~GameState() = default;

        /**
         * Handler for when button A is pressed.
         */
        virtual void onButtonAPress() = 0;

        /**
         * Handler for when button B is pressed.
         */
        virtual void onButtonBPress() = 0;

        /**
         * Handler for when both buttons A and B are pressed.
         */
        virtual void onButtonABPress() = 0;

        /**
         * Handler for when a radio message has been received.
         *
         * @param in the input buffer.
         */
        virtual void onMessage(ByteBuf &in) = 0;

        /**
         * Runs the current game state.
         */
        virtual void run() = 0;
    };

}

#endif //PLATFORMER_GAME_STATE_H
