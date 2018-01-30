#ifndef PLATFORMER_PLAYER_H
#define PLATFORMER_PLAYER_H

#import "Vector2i.h"

namespace platformer {

    class Player {
    private:
        static const int JUMP_HEIGHT = 3;

        Vector2i *location = new Vector2i();
        Vector2i *velocity = new Vector2i();
    public:
        virtual ~Player();

        bool isOnGround();

        void jump();

        Vector2i *getLocation() const;

        void setLocation(Vector2i *location);

        Vector2i *getVelocity() const;

        void setVelocity(Vector2i *velocity);
    };

} // namespace platformer

#endif // PLATFORMER_PLAYER_H
