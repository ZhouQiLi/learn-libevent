//
// Created on 2022/8/19.
//

#include "EventLoop.h"
#include "TcpServer.h"
#include "Conn.h"

int main() {
    EventLoop loop;
    TcpServer server(&loop, 8755, "EchoServer");
    server.SetMessageCallback([](const ConnPtr &conn) {
        auto msg = conn->Read();
        std::cout << "on message: " << msg << std::endl;
        conn->Send(msg);
    });
    server.Run();
}