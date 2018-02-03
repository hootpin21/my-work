#ifndef PLATFORMER_BYTEBUF_H
#define PLATFORMER_BYTEBUF_H

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

    class ByteBuf {
    private:
        size_t offset = 0;
        std::vector<uint8_t> buffer;
    public:
        ByteBuf();

        ByteBuf(size_t size, uint8_t &data);

        virtual ~ByteBuf();

        int size();

        uint8_t *data();

        int read();

        int readInt();

        PacketType readPacketType();

        void write(int b);

        void writeInt(int v);

        void writePacketType(PacketType packetType);
    };
}

#endif //PLATFORMER_BYTEBUF_H
