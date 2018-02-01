#ifndef PLATFORMER_WORLD_H
#define PLATFORMER_WORLD_H

#include <vector>
#include <string>
#include "Vector2i.h"

namespace platformer {

    enum BlockType : char {
        AIR = ' ',
        SOLID = '+',
        FLAG = '=',
        COIN = '.'
    };

    class World {
    private:
        const int id;
        const int minX = 0;
        const int minY = 0;
        const int maxX;
        const int maxY;
        std::vector<std::string*> map;
    public:
        World(int id, int maxX, int maxY, std::vector<std::string*> &map);

        virtual ~World();

        int getId() const;

        int getMinX() const;

        int getMinY() const;

        int getMaxX() const;

        int getMaxY() const;

        std::vector<std::string*> &getMap();

        BlockType getBlock(Vector2i location) const;

        void setBlock(Vector2i location, BlockType type);
    };

    static const int WORLD_COUNT = 5;

    World *createWorld(int id);
}

#endif //PLATFORMER_WORLD_H
