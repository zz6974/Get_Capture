//
// Created by ZhuanZ（无密码） on 2026/10/29.
//

#ifndef CV_TEST_SER_WEBSOCK_H
#define CV_TEST_SER_WEBSOCK_H

#include "CCWebSocket.h"

class Ser_WebSock {
public:
    //二进制消息回调：上层（业务路由）通过它订阅原始字节流
    using OnBinaryMessage = std::function<void(CTL::ByteArray&)>;
    void SetOnBinary(OnBinaryMessage cb) { onBinary_ = std::move(cb); }

    //向所有客户端广播二进制数据，返回成功发送的客户端数量
    int BroadcastBinary(const CTL::ByteArray& data);
    //向指定客户端发送二进制数据，客户端不存在返回 false
    bool SendBinaryTo(const SOCKET& id, const CTL::ByteArray& data);

    void OnOpen(CTL::WebClient& client);
    void OnMessage(CTL::WebClient& client);
    void OnClose(CTL::WebClient& client);
    void OnError(CTL::WebClient& client);
    void OnPing(CTL::WebClient& client);
    void OnPong(CTL::WebClient& client);

    //绑定监听（非阻塞，仅 Init + InitBind）
    int Bind(std::string ip, int port);
    //启动事件循环（阻塞）
    int Run();
    //兼容旧接口：绑定后立即阻塞运行
    int Server_Bind_Demo(std::string ip, int port);
private:
    CTL::WebSocketServer server;
    OnBinaryMessage onBinary_;
};



#endif //CV_TEST_SER_WEBSOCK_H
