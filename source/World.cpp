#include "World.h"

namespace platformer {

    World::World(int id, int maxX, int maxY, std::vector<std::string *> &map) : id(id), maxX(maxX), maxY(maxY),
                                                                                map(map) {}

    World::~World() {
        for (std::string *row : map) {
            delete row;
        }
    }

    int World::getId() const {
        return id;
    }

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

    std::vector<std::string *> &World::getMap() {
        return map;
    }

    BlockType World::getBlock(Vector2i location) const {
        int x = location.getX();
        int y = (maxY - 1) - (location.getY());

        if (x < 0 || x >= maxX ||
            y < 0 || y >= maxY) {
            return AIR;
        }

        return (BlockType) map.at((size_t) y)->at((size_t) x);
    }

    void World::setBlock(Vector2i location, BlockType type) {
        int x = location.getX();
        int y = (maxY - 1) - (location.getY());

        if (x < 0 || x >= maxX ||
            y < 0 || y >= maxY) {
            return;
        }

        std::string *row = map.at((size_t) y);
        row->at((size_t) x) = type;
        map.at((size_t) y) = row;
    }

    World *createWorld1() {
        std::vector<std::string *> map = {
                new std::string("                    "),
                new std::string(" .       .          "),
                new std::string("+++    .   .      = "),
                new std::string("                 == "),
                new std::string("      +++++       = "),
                new std::string("   .      ++++    = "),
                new std::string("+++++        +++++++")
        };
        return new World(1, 20, 7, map);
    }

    World *createWorld2() {
        std::vector<std::string *> map = {
                new std::string("         .          "),
                new std::string("             .      "),
                new std::string("                  = "),
                new std::string("    .   +++      == "),
                new std::string("     ++           = "),
                new std::string("  .         ++    = "),
                new std::string("++++            ++++")
        };
        return new World(2, 20, 7, map);
    }

    World *createWorld3() {
        std::vector<std::string *> map = {
                new std::string("         ....+                            "),
                new std::string("         +++++..                          "),
                new std::string("++++         +++++    +++                 "),
                new std::string("    . .      +..                          "),
                new std::string("             +..       .  ++              "),
                new std::string("     +++ . . +++     .                  = "),
                new std::string("  ++++                  ++ .     .     == "),
                new std::string("    .+   ++++     ++       .            = "),
                new std::string("     + ..        ++        .   +++++    = "),
                new std::string("+++++++++    +++++        ++++       +++++")
        };
        return new World(3, 42, 10, map);
    }

    World *createWorld(int id) {
        switch (id) {
            case 1:
                return createWorld1();
            case 2:
                return createWorld2();
            case 3:
                return createWorld3();
            default: // Invalid world selected, send back default.
                return createWorld1();
        }
    }
}
