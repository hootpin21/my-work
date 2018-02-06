#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#include "Vector2i.h"

namespace platformer {

    /**
     * Represents a player, used in the play game state.
     */
    class Player {
    private:
        // Velocity to add when player jumps.
        static const int JUMP_HEIGHT = 3;

        // The players current location and velocity. Coordinates in this game
        // have an origin (0,0) of the bottom left LED to make more sense when
        // working on game logic. Increasing Y should make the player move up.
        Vector2i *location = new Vector2i();
        Vector2i *velocity = new Vector2i();
    public:
        /**
         * Destructs the player, deleting their location and velocity.
         */
        virtual ~Player();

        /**
         * Increases the players Y velocity by JUMP_HEIGHT. Causing them to
         * appear to be jumping in game.
         */
        void jump();

        /**
         * Gets the players location.
         *
         * @return the location.
         */
        Vector2i &getLocation() const;

        /**
         * Gets the players velocity.
         *
         * @return the velocity.
         */
        Vector2i &getVelocity() const;
    };

}

#endif // PLATFORMER_PLAYER_H
