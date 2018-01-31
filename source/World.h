#ifndef PLATFORMER_WORLD_H
#define PLATFORMER_WORLD_H

#include <vector>
#include <string>
#include "Vector2i.h"

namespace platformer {

    enum BlockType : char {
        AIR = '-',
        FOREGROUND = '+',
        BACKGROUND = '=',
        COIN_ON = '.',
        COIN_OFF = ','
    };

    class World {
    private:
        const int minX = 0;
        const int minY = 0;
        const int maxX;
        const int maxY;
        std::vector<std::string> map;
    public:
        World(int maxX, int maxY, std::vector<std::string> &map);

        int getMinX() const;

        int getMinY() const;

        int getMaxX() const;

        int getMaxY() const;

        std::vector<std::string> &getMap();

        Vector2i getRelativeLocation(Vector2i location, int offsetX, int offsetY) const;

        BlockType getRelativeBlock(Vector2i location, int offsetX, int offsetY) const;

        BlockType getBlock(Vector2i location) const;

        void setBlock(Vector2i location, BlockType type);
    };

    World *createWorld1();
}

#endif //PLATFORMER_WORLD_H
