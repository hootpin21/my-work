#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#include "Vector2i.h"
#include "World.h"

namespace platformer {

    class Player {
    private:
        static const int JUMP_HEIGHT = 3;

        Vector2i *location = new Vector2i();
        Vector2i *velocity = new Vector2i();
    public:
        virtual ~Player();

        void jump();

        Vector2i &getLocation() const;

        Vector2i &getVelocity() const;
    };

}

#endif // PLATFORMER_PLAYER_H
