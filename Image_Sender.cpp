//
// Created by ZhuanZ（无密码） on 2026/10/30.
//

#include "Image_Sender.h"
#include "Comm_Protocol.h"

int SendImage(Ser_Camera& camera, Ser_WebSock& server) {
    // 1. 抓取一帧并编码为 JPEG
    std::vector<uchar> jpeg;
    if (!camera.Camera_Capture(jpeg) || jpeg.empty()) {
        return -1;
    }

    // 2. 按协议打包为图像帧: magic + FT_FRAME + length + jpeg
    Frame frame;
    if (!frame.Pack(FT_FRAME, jpeg)) {
        return -1;
    }

    // 3. 通过 WebSocket 广播给所有客户端
    return server.BroadcastBinary(CTL::ByteArray(frame.Data().data(), frame.Size()));
}
