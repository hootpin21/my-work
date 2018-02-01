#include "World.h"

namespace platformer {

    World::World(const int maxX, const int maxY, std::vector<std::string> &map) : maxX(maxX), maxY(maxY),
                                                                                  map(map) {}

    int World::getMinX() const {
        return minX;
    }

    int World::getMinY() const {
        return minY;
    }

    int World::getMaxX() const {
        return maxX;
    }

    int World::getMaxY() const {
        return maxY;
    }

    std::vector<std::string> &World::getMap() {
        return map;
    }

    Vector2i World::getRelativeLocation(Vector2i location, int offsetX, int offsetY) const {
        int relativeX = offsetX + location.getX();
        int relativeY = offsetY + ((maxY - 1) - location.getY());
        return {relativeX, relativeY};
    }

    BlockType World::getRelativeBlock(Vector2i location, int offsetX, int offsetY) const {
        Vector2i blockLocation = getRelativeLocation(location, offsetX, offsetY);
        BlockType blockType = getBlock(blockLocation);
        return blockType;
    }

    BlockType World::getBlock(const Vector2i location) const {
        if (location.getX() < 0 || location.getX() >= (maxX - 1) ||
            location.getY() < 0 || location.getY() >= maxY) {
            return AIR;
        }
        return (BlockType) map[location.getY()][location.getX()];
    }

    void World::setBlock(const Vector2i location, BlockType type) {
        std::string row = map.at((size_t) location.getY());
        row.at((size_t) location.getX()) = type;
        map.at((size_t) location.getY()) = row;
    }

    World *createWorld(int id) {
        std::vector<std::string> map;

        switch (id) {
            case 1:
                 map = {
                        "--------------------",
                        "---------.----------",
                        "-------.---.------=-",
                        "-----------------==-",
                        "-------+++++------=-",
                        "---.--------------=-",
                        "+++++---------+++++++"
                };
                return new World(21, 7, map);
            case 2:
                map = {
                        "---------.----------",
                        "-------------.------",
                        "------------------=-",
                        "----.---+++------==-",
                        "-----++-----------=-",
                        "--.---------++----=-",
                        "++++------------++++"
                };
                return new World(21, 7, map);
            default: // Invalid world selected, send back default.
                return createWorld(1);
        }
    }
}
