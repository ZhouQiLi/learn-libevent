#pragma once
#include <iostream>

#include "Type.h"

class EventLoop;
class TcpServer {
public:
    TcpServer(EventLoopPtr eventLoop, uint16_t port, std::string name):
        port(port),
        listener(nullptr),
        loop(eventLoop),
        name(std::move(name)),
        connectedCallback(nullptr),
        messageCallback(nullptr) {};

    TcpServer(const TcpServer &) = delete;
    TcpServer &operator=(const TcpServer &) = default;
    ~TcpServer();
    void Run();

    void OnAccept(int);
    void OnDisconnect(const ConnPtr &);

    void SetConnectedCallback(const ConnectedCallback &cb) {
        connectedCallback = cb;
    }

    void SetMessageCallback(const MessaageCallback &cb) {
        messageCallback = cb;
    }

    EventBaseType EventBase() {
        return loop->EventBase();
    }

private:
    uint16_t port;
    ListenerType listener;
    EventLoopPtr loop;
    std::string name;
    ConnectedCallback connectedCallback;
    MessaageCallback messageCallback;
    ConnMaps connMaps;
};