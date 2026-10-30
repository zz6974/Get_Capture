//
// Created by ZhuanZ（无密码） on 2026/10/29.
//

#include "Ser_WebSock.h"

int Ser_WebSock::BroadcastBinary(const CTL::ByteArray& data) {
    int sent = 0;
    for (const auto fd : server.getClientFds()) {
        auto c = server.getClient(fd);
        if (c && c->SendBinary(data)) {
            sent++;
        }
    }
    return sent;
}

bool Ser_WebSock::SendBinaryTo(const SOCKET& id, const CTL::ByteArray& data) {
    auto c = server.getClient(id);
    return c && c->SendBinary(data);
}

void Ser_WebSock::OnOpen(CTL::WebClient &client) {
    CTL::System::Println("OnOpen ID:{}" , client.GetID());

    const auto fds = server.getClientFds();

    for (const auto fd : fds) {
        auto c = server.getClient(fd);
        if (c && c->id != client.id) {
            c->SendText("New client joined");
        }
    }
}

void Ser_WebSock::OnMessage(CTL::WebClient &client) {
    auto packet = client.GetPacket();
    if (packet.get_opcode() == WebSocketPacket::WSOpcode_Text) {
        CTL::System::Println("OnMessage ID:{} {}", client.GetID(), "Text Message");
        const auto text = client.GetBuffer().toString();
        //回显给发送者
        client.SendText(text);
        //向客户端广播
        const auto fds = server.getClientFds();
        for (const auto fd : fds) {
            auto c = server.getClient(fd);
            if (c && c->id != client.id) {
                c->SendText(text);
            }
        }
    }else if (packet.get_opcode() == WebSocketPacket::WSOpcode_Binary) {
        CTL::System::Println("OnMessage ID:{} {}", client.GetID(), "Binary Message");
        CTL::ByteArray binary = client.GetBuffer();
        //二进制流量交给上层订阅者处理，未订阅则回显保持兼容
        if (onBinary_) {
            onBinary_(binary);
        } else {
            client.SendBinary(binary);
        }
    }
}

void Ser_WebSock::OnClose(CTL::WebClient &client) {
    CTL::System::Println("OnClose ID:{}" , client.GetID());
}

void Ser_WebSock::OnError(CTL::WebClient &client) {
    CTL::System::Println("OnError ID:{}" , client.GetID());
}

void Ser_WebSock::OnPing(CTL::WebClient &client) {
    CTL::System::Println("OnPing ID:{}" , client.GetID());
}

void Ser_WebSock::OnPong(CTL::WebClient &client) {
    CTL::System::Println("OnPong ID:{}" , client.GetID());
}

int Ser_WebSock::Bind(std::string ip, int port) {
    // 去除首尾空白, 避免带空格的 IP 导致 inet_addr 解析失败(10049)
    const auto begin = ip.find_first_not_of(" \t\r\n");
    const auto end = ip.find_last_not_of(" \t\r\n");
    ip = (begin == std::string::npos) ? "" : ip.substr(begin, end - begin + 1);
    if (ip.empty()) {
        CTL::System::Println("[Server] 无效的绑定地址");
        return -1;
    }
    if (!server.Init(ip, port)) {
        CTL::System::Println("[Server] Init failed");
        return -1;
    }
    CTL::WSBind bind;
    bind.URL = "/ws";
    bind.OnCloseFun = [this](CTL::WebClient& client) { OnClose(client); };
    bind.OnErrorFun = [this](CTL::WebClient& client) { OnError(client); };
    bind.OnMessageFun = [this](CTL::WebClient& client) { OnMessage(client); };
    bind.OnOpenFun = [this](CTL::WebClient& client) { OnOpen(client); };
    bind.OnPingFun = [this](CTL::WebClient& client) { OnPing(client); };
    bind.OnPongFun = [this](CTL::WebClient& client) { OnPong(client); };
    server.InitBind(bind);
    return 0;
}

int Ser_WebSock::Run() {
    return server.Running();
}

int Ser_WebSock::Server_Bind_Demo(std::string ip, int port) {
    if (Bind(ip, port) != 0) {
        return -1;
    }
    return Run();
}
