//
// 图片发送: 抓取摄像头帧 -> 协议打包 -> WebSocket 广播
//

#ifndef CV_TEST_IMAGE_SENDER_H
#define CV_TEST_IMAGE_SENDER_H

#include "Ser_Camera.h"
#include "Ser_WebSock.h"

// 抓取一帧图片(JPEG),按协议打包为图像帧(FT_FRAME)后广播给所有客户端
// 返回成功发送的客户端数量;抓帧或打包失败返回 -1
int SendImage(Ser_Camera& camera, Ser_WebSock& server);

#endif //CV_TEST_IMAGE_SENDER_H
