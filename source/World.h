#ifndef PLATFORMER_WORLD_H
#define PLATFORMER_WORLD_H

#include <vector>
#include <string>
#include "Vector2i.h"

namespace platformer {

    /**
     * All types of blocks, read friendly characters used for easy plotting of
     * the map.
     */
    enum BlockType : char {
        AIR = ' ',
        SOLID = '+',
        FLAG = '=',
        COIN = '.'
    };

    /**
     * Represents a world that is played in the "play" game state.
     */
    class World {
    private:
        // The unique ID for this world type.
        const int id;

        // Map dimensions.
        const int minX = 0;
        const int minY = 0;
        const int maxX;
        const int maxY;

        // Backing map, stores all block types and positions. Uses pointers of
        // strings as without this, some maps require up to 500 bytes of
        // contiguous memory. Since memory fragments quickly on micro:bit
        // devices and we only have 16kb to play with, it makes sense to split
        // up our allocations.
        std::vector<std::string*> map;

    public:
        /**
         * Constructs a new {@link World}.
         *
         * @param id the uniquie ID of this world type.
         * @param maxX the maximum X coordinate.
         * @param maxY the maximum Y coordinate.
         * @param map the backing map of all block types and positions.
         */
        World(int id, int maxX, int maxY, std::vector<std::string*> &map);

        /**
         * Destructs this world, deleting all contents of the map.
         */
        virtual ~World();

        /**
         * Gets the unique identification for this world type.
         *
         * @return the ID.
         */
        int getId() const;

        /**
         * Gets the minimum X coordinate.
         *
         * @return the minimum X coordinate.
         */
        int getMinX() const;

        /**
         * Gets the minimum Y coordinate.
         *
         * @return the minimum Y coordinate.
         */
        int getMinY() const;

        /**
         * Gets the maximum X coordinate.
         *
         * @return the maximum X coordinate.
         */
        int getMaxX() const;

        /**
         * Gets the maximum Y coordinate.
         *
         * @return the maximum Y coordinate.
         */
        int getMaxY() const;

        std::vector<std::string*> &getMap();

        /**
         * Gets the block type, given a specific location.
         *
         * @param location the location of the block.
         * @return the block type at the provided location.
         */
        BlockType getBlock(Vector2i location) const;

        /**
         * Sets the block type of a given location.
         *
         * @param location the location of the block.
         * @param type the new block type to set.
         */
        void setBlock(Vector2i location, BlockType type);
    };

    // The total number of worlds.
    static const int WORLD_COUNT = 5;

    /**
     * Creates a new world, given the one-indexed unique ID of the world type.
     * World IDs are from 1 to WORLD_COUNT.
     *
     * @param id the world ID to create.
     * @return the newly created world.
     */
    World *createWorld(int id);
}

#endif //PLATFORMER_WORLD_H
