#include "ByteBuf.h"

namespace platformer {

    ByteBuf::ByteBuf() = default;

    ByteBuf::ByteBuf(size_t size, uint8_t &data) {
        buffer.assign(size, data);
    }

    ByteBuf::~ByteBuf() = default;

    int ByteBuf::size() {
        return (int) buffer.size();
    }

    uint8_t *ByteBuf::data() {
        return buffer.data();
    }

    int ByteBuf::read() {
        if (offset >= size()) {
            return -1;
        }
        return buffer.at(offset++);
    }

    int ByteBuf::readInt() {
        int ch1 = read();
        int ch2 = read();
        int ch3 = read();
        int ch4 = read();

        if ((ch1 | ch2 | ch3 | ch4) < 0) {
            return -1;
        }

        return ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
    }

    PacketType ByteBuf::readPacketType() {
        int id = readInt();

        if (id < 0 || id > PacketType::INVALID) {
            return PacketType::INVALID;
        }

        return static_cast<PacketType>(id);
    }

    void ByteBuf::write(int b) {
        buffer.push_back((uint8_t) b);
    }

    void ByteBuf::writeInt(int v) {
        auto uv = (uint32_t) v;
        write((uv >> 24) & 0xFF);
        write((uv >> 16) & 0xFF);
        write((uv >> 8) & 0xFF);
        write((uv >> 0) & 0xFF);
    }

    void ByteBuf::writePacketType(PacketType packetType) {
        writeInt(packetType);
    }

}
