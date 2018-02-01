#include "Vector2i.h"

namespace platformer {

    Vector2i::Vector2i() = default;

    Vector2i::Vector2i(int x, int y) : x(x), y(y) {}

    Vector2i &Vector2i::set(int x, int y) {
        Vector2i::x = x;
        Vector2i::y = y;
        return *this;
    }

    Vector2i &Vector2i::add(int x, int y) {
        Vector2i::x += x;
        Vector2i::y += y;
        return *this;
    }

    int Vector2i::getX() const {
        return x;
    }

    Vector2i &Vector2i::setX(int x) {
        Vector2i::x = x;
        return *this;
    }

    Vector2i &Vector2i::addX(int x) {
        Vector2i::x += x;
        return *this;
    }

    int Vector2i::getY() const {
        return y;
    }

    Vector2i &Vector2i::setY(int y) {
        Vector2i::y = y;
        return *this;
    }

    Vector2i &Vector2i::addY(int y) {
        Vector2i::y += y;
        return *this;
    }

    Vector2i Vector2i::clone() {
        return {x, y};
    }

    Vector2i Vector2i::getRelative(int offsetX, int offsetY) {
        return {x + offsetX, y + offsetY};
    }

}
