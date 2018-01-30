#ifndef PLATFORMER_LOCATION_H
#define PLATFORMER_LOCATION_H

namespace platformer {

    class Vector2i {
    private:
        int x = 0;
        int y = 0;
    public:
        Vector2i();

        Vector2i(int x, int y);

        void set(int x, int y);

        void add(int x, int y);

        void addX(int x);

        void addY(int y);

        int getX() const;

        void setX(int x);

        int getY() const;

        void setY(int y);
    };

} // namespace platformer

#endif // PLATFORMER_LOCATION_H
