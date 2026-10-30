//
// 定义数据格式
//+--------+--------+---------+--------+----------+
// | magic  |  type  | length  |  payload ...     |
// |  2B    |  1B    |  4B     |  length 字节      |
// +--------+--------+---------+--------+----------+

#ifndef CV_TEST_COMM_PROTOCOL_H
#define CV_TEST_COMM_PROTOCOL_H
#include <cstdint>
#include <vector>

constexpr uint16_t FRAME_MAGIC = 0xCCDD;          // 帧头定位符
constexpr uint32_t MAX_LEN = 10 * 1024 * 1024;    // 最大数据长度(10MB)

enum FrameType {
    FT_UNKNOW = 0,
    FT_FRAME = 1,   // 图像帧
    FT_DATA = 2,    // 通用数据帧
};

// 帧头: magic(2B) + type(1B) + length(4B), 按 1 字节对齐
#pragma pack(push, 1)
struct FrameHeader {
    uint16_t magic = FRAME_MAGIC;
    uint8_t type = FT_UNKNOW;
    uint32_t length = 0;
};
#pragma pack(pop)

constexpr uint32_t FRAME_HEADER_SIZE = sizeof(FrameHeader);

class Frame {
public:
    Frame() = default;
    ~Frame() = default;

    // 打包协议帧: 帧头 + payload, payload 为空指针时按空负载处理
    bool Pack(FrameType type, const void* payload, uint32_t length);
    // 打包协议帧: 字节流负载版本
    bool Pack(FrameType type, const std::vector<uint8_t>& payload);
    void Clear();   // 清除帧内容

    // 打包后的完整帧字节流(magic + type + length + payload)
    const std::vector<uint8_t>& Data() const { return buffer_; }
    uint32_t Size() const { return static_cast<uint32_t>(buffer_.size()); }

private:
    std::vector<uint8_t> buffer_;   // 打包后的帧数据
};

#endif //CV_TEST_COMM_PROTOCOL_H
