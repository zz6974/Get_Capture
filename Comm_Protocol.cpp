//
// Created by ZhuanZ（无密码） on 2026/10/28.
//

#include "Comm_Protocol.h"
#include <cstring>


bool Frame::Pack(FrameType type, const void *payload, uint32_t length) {
    if (type == FT_UNKNOW) return false;
    if (length > MAX_LEN) return false;
    if (payload == nullptr && length > 0) return false;

    FrameHeader header;
    header.magic = FRAME_MAGIC;
    header.type = static_cast<uint8_t>(type);
    header.length = length;

    buffer_.resize(FRAME_HEADER_SIZE + length);
    std::memcpy(buffer_.data(), &header, FRAME_HEADER_SIZE);
    if (length > 0) {
        std::memcpy(buffer_.data() + FRAME_HEADER_SIZE, payload, length);
    }
    return true;
}

bool Frame::Pack(FrameType type, const std::vector<uint8_t> &payload) {
    return Pack(type, payload.data(), static_cast<uint32_t>(payload.size()));
}

void Frame::Clear() {
    buffer_.clear();
}
