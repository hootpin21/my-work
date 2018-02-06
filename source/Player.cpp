#include "Player.h"

namespace platformer {

    Player::~Player() {
        delete location;
        delete velocity;
    }

    void Player::jump() {
        velocity->addY(JUMP_HEIGHT);
    }

    Vector2i &Player::getLocation() const {
        return *location;
    }

    Vector2i &Player::getVelocity() const {
        return *velocity;
    }

}
