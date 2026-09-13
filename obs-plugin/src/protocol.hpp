#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace idraiscam {
constexpr uint32_t MAGIC = 0x49445243;
constexpr uint16_t VERSION = 1;

enum PacketType : uint16_t {
    HELLO = 1,
    VIDEO_CONFIG = 2,
    VIDEO = 3,
    AUDIO_CONFIG = 4,
    AUDIO = 5,
    CONTROL = 6,
    PING = 7,
    PONG = 8,
};

enum PacketFlags : uint32_t {
    KEYFRAME = 1,
    CODEC_CONFIG = 2,
};

struct Packet {
    uint16_t type = 0;
    uint32_t flags = 0;
    uint64_t timestamp_ns = 0;
    std::vector<uint8_t> payload;
};

struct DeviceInfo {
    std::string name;
    std::string host;
    uint16_t port = 48484;
};
}
