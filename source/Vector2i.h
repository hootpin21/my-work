#ifndef PLATFORMER_VECTOR2I_H
#define PLATFORMER_VECTOR2I_H

namespace platformer {

    class Vector2i {
    private:
        int x = 0;
        int y = 0;
    public:
        Vector2i();

        Vector2i(int x, int y);

        Vector2i &set(int x, int y);

        Vector2i &add(int x, int y);

        Vector2i &addX(int x);

        Vector2i &addY(int y);

        int getX() const;

        Vector2i &setX(int x);

        int getY() const;

        Vector2i &setY(int y);

        Vector2i clone();

        Vector2i getRelative(int offsetX, int offsetY);
    };

}

#endif // PLATFORMER_VECTOR2I_H
