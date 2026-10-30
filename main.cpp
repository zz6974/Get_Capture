#include <iostream>
#include <opencv2/opencv.hpp>
#include <atomic>
#include <chrono>
#include <thread>
#include <winsock2.h>   // 必须在 windows.h 之前, 避免 winsock 版本冲突
#include <windows.h>
#include "Ser_WebSock.h"
#include "Ser_Camera.h"
#include "Image_Sender.h"

int main() {
    // 控制台按 UTF-8 显示, 避免中文日志乱码
    SetConsoleOutputCP(CP_UTF8);

    Ser_WebSock server;
    // 绑定 0.0.0.0 监听所有网卡, 客户端用本机实际 IP 连接
    if (server.Bind("0.0.0.0", 8081) != 0) {
        return -1;
    }

    Ser_Camera camera;
    if (camera.Camera_Init() < 0) {
        CTL::System::Println("[Camera] 初始化失败, 无法发送图片");
    }

    // 发送线程: 每 100ms 抓取一帧图片并广播
    std::atomic<bool> running{true};
    std::thread sender([&] {
        int last_sent = -1;
        while (running) {
            int sent = SendImage(camera, server);
            if (sent != last_sent) {   // 状态变化时才打印, 避免刷屏
                CTL::System::Println("[Image] 广播图片帧, 成功发送 {} 个客户端", sent);
                last_sent = sent;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    server.Run();

    running = false;
    sender.join();
    return 0;
}
