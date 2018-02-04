#ifndef PLATFORMER_VECTOR2I_H
#define PLATFORMER_VECTOR2I_H

namespace platformer {

    /**
     * A vector of two integers. Used for locations and velocities.
     */
    class Vector2i {
    private:
        // The X and Y values. Not much else to it really. :)
        int x = 0;
        int y = 0;

    public:
        /**
         * Constructs a new {@link Vector2i} of values [0, 0].
         */
        Vector2i();

        /**
         * Constructs a new {@link Vector2i}.
         *
         * @param x the X value.
         * @param y the Y value.
         */
        Vector2i(int x, int y);

        /**
         * Sets both the X and Y values.
         *
         * @param x the new X value.
         * @param y the new Y value.
         * @return this.
         */
        Vector2i &set(int x, int y);

        /**
         * Adds to both the X and Y values.
         *
         * @param x the amount to add X by.
         * @param y the amount to add Y by.
         * @return this.
         */
        Vector2i &add(int x, int y);

        /**
         * Adds to the X value.
         *
         * @param x the amount to add X by.
         * @return this.
         */
        Vector2i &addX(int x);

        /**
         * Adds to the Y value.
         *
         * @param y the amount to add Y by.
         * @return this.
         */
        Vector2i &addY(int y);

        /**
         * Gets the X value.
         *
         * @return the value of X.
         */
        int getX() const;

        /**
         * Sets the value of X.
         *
         * @param x the new value of X.
         * @return this.
         */
        Vector2i &setX(int x);

        /**
         * Gets the value of Y.
         *
         * @return the value of Y.
         */
        int getY() const;

        /**
         * Sets the value of Y.
         *
         * @param y the new value of Y.
         * @return this.
         */
        Vector2i &setY(int y);

        /**
         * Clones the current {@link Vector2i}.
         *
         * @return the clone.
         */
        Vector2i clone();

        /**
         * Clones the current {@link Vector2i} and adds the two offsets,
         * returning the final result.
         *
         * @param offsetX the X offset.
         * @param offsetY the Y offset.
         * @return the cloned and modified result.
         */
        Vector2i getRelative(int offsetX, int offsetY);
    };

}

#endif // PLATFORMER_VECTOR2I_H
