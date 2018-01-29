#include "Vector2i.h"

namespace platformer {

    void Vector2i::set(int x, int y) {
        Vector2i::x = x;
        Vector2i::y = y;
    }

    void Vector2i::add(int x, int y) {
        Vector2i::x += x;
        Vector2i::y += y;
    }

    int Vector2i::getX() const {
        return x;
    }

    void Vector2i::setX(int x) {
        Vector2i::x = x;
    }

    void Vector2i::addX(int x) {
        Vector2i::x += x;
    }

    int Vector2i::getY() const {
        return y;
    }

    void Vector2i::setY(int y) {
        Vector2i::y = y;
    }

    void Vector2i::addY(int y) {
        Vector2i::y += y;
    }

} // namespace platformer
