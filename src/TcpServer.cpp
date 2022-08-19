//
// Created on 2022/8/18.
//
#include <atomic>
#include <cassert>
#ifndef _WIN32
//#include <netinet/in.h>
#endif

#include "event2/listener.h"
#include "event2/bufferevent.h"

#include "EventLoop.h"
#include "TcpServer.h"
#include "Conn.h"

namespace {
    std::atomic<uint64_t> uniqueId{1};

    void OnNewSocket(struct evconnlistener *, evutil_socket_t fd,
                     struct sockaddr *sa, int socklen, void *userData) {
        auto tcpServer = static_cast<TcpServer*>(userData);
        tcpServer->OnAccept(static_cast<int>(fd));
    }

    void ConnReadCallback(struct bufferevent *, void *userData) {
        auto conn = static_cast<Conn*>(userData);
        conn->OnReadEvent();
    }

    void ConnEventCallback(struct bufferevent *, SocketEventType events, void *userData) {
        auto conn = static_cast<Conn*>(userData);
        conn->OnEvent(events);
    }
}

TcpServer::~TcpServer() {
    if (listener) {
        evconnlistener_free(listener);
    }
    event_base_free(loop->EventBase());
}

void TcpServer::Run() {
    auto eventBase = EventBase();
    assert(eventBase);
    assert(messageCallback);

    struct sockaddr_in sin = {0};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);

    listener = evconnlistener_new_bind(eventBase, OnNewSocket, static_cast<void *>(this),
                                       LEV_OPT_REUSEABLE|LEV_OPT_CLOSE_ON_FREE, -1,
                                       (struct sockaddr*)&sin,
                                       sizeof(sin));

    if (!listener) {
        std::cerr << "Could not create a listener!" << std::endl;
        return;
    }

    event_base_dispatch(eventBase);
}

void TcpServer::OnDisconnect(const ConnPtr &conn) {
    auto fd = conn->Fd();
    connMaps.erase(fd);
}

void TcpServer::OnAccept(int fd) {
    auto buffer = bufferevent_socket_new(EventBase(), fd, BEV_OPT_CLOSE_ON_FREE);
    if (!buffer) {
        std::cerr << "Error constructing bufferevent!" << std::endl;
        return;
    }

    auto conn = std::make_shared<Conn>(fd, uniqueId++, Conn::ConnType::CLIENT, buffer);
    bufferevent_setcb(buffer, ConnReadCallback, nullptr, ConnEventCallback, conn.get());
    bufferevent_enable(buffer, EV_READ|EV_WRITE);
    conn->SetMessageCallback(messageCallback);
    conn->SetCloseCallback([this](const ConnPtr &targetConn) {
        this->OnDisconnect(targetConn);
    });

    connMaps.insert({fd, conn});

    if (connectedCallback) {
        connectedCallback(conn);
    }
}