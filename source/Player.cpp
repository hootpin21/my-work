#include "Player.h"

namespace platformer {

    bool Player::isOnGround() {
        return location->getY() == 0;
    }

    Vector2i *Player::getLocation() const {
        return location;
    }

    void Player::setLocation(Vector2i *location) {
        Player::location = location;
    }

    void Player::jump() {
        velocity->addY(JUMP_HEIGHT);
    }

    Vector2i *Player::getVelocity() const {
        return velocity;
    }

    void Player::setVelocity(Vector2i *velocity) {
        Player::velocity = velocity;
    }

    Player::~Player() {
        delete location;
        delete velocity;
    }

} // namespace platformer
