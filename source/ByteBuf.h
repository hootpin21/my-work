#ifndef PLATFORMER_BYTE_BUF_H
#define PLATFORMER_BYTE_BUF_H

#include <vector>
#include <stdint-gcc.h>

namespace platformer {

    // Enumerates all packet types.
    // It is imperative to keep INVALID as the last element.
    // Do not reassign any of these to a different integer value.
    enum PacketType {
        BROADCAST,
        PARTNER_REQUEST,
        SELECT_WORLD,
        WORLD_COMPLETE,
        DISCONNECT,
        INVALID // Always keep INVALID at the end, its used for indexing.
    };

    /**
     * Byte buffer helper class. Provides ease-of-use methods for reading from
     * and writing to byte arrays.
     */
    class ByteBuf {
    private:
        // Offset in bytes for reading from the data buffer.
        size_t offset = 0;

        // The variable-length backing data buffer.
        std::vector<uint8_t> buffer;

    public:
        /**
         * Constructs a new empty {@link ByteBuf}.
         */
        ByteBuf();

        /**
         * Constructs a new {@link ByteBuf} using the data provided.
         *
         * @param size the size in bytes.
         * @param data the data in bytes.
         */
        ByteBuf(size_t size, uint8_t &data);

        /**
         * Destructs the current {@link ByteBuf}.
         */
        virtual ~ByteBuf();

        /**
         * Gets the size in bytes.
         *
         * @return the size in bytes.
         */
        int size();

        /**
         * Gets the raw data held in this buffer.
         *
         * @return the raw byte array.
         */
        uint8_t *data();

        /**
         * Reads one byte.
         *
         * @return the next byte to read in.
         */
        int read();

        /**
         * Reads 4-byte big endian integer.
         *
         * @return the next integer to read in.
         */
        int readInt();

        /**
         * Reads an integer and finds the associated {@link PacketType}.
         * If none match, {@code PacketType::INVALID} is provided.
         *
         * @return the next packet type.
         */
        PacketType readPacketType();

        /**
         * Writes a single byte to the buffer.
         *
         * @param b the byte to write.
         */
        void write(int b);

        /**
         * Writes a 4-byte big endian integer to the buffer.
         *
         * @param v the integer value to write.
         */
        void writeInt(int v);

        /**
         * Writes an integer that represents the provided {@link PacketType}.
         *
         * @param packetType the packet type to write.
         */
        void writePacketType(PacketType packetType);
    };
}

#endif //PLATFORMER_BYTE_BUF_H
