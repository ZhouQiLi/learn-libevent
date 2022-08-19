#pragma once
#include <memory>

#include "Type.h"

class Conn : public std::enable_shared_from_this<Conn> {
public:
    enum class ConnType {
        SERVER,
        CLIENT,
    };

    Conn(FdType fd, uint64_t id, ConnType t, struct bufferevent *b):
        fd(fd),
        id(id),
        connType(t),
        buffer{b} {}

    ~Conn();

    FdType Fd() const {return fd;}
    uint64_t Id() const {return id;}
    ConnType Type() const {return connType;}

    void SetMessageCallback(const MessaageCallback &cb) {
        messageCallback = cb;
    }

    void SetCloseCallback(const CloseCallback &cb) {
        closeCallback = cb;
    }

    void OnEvent(SocketEventType);

    void OnReadEvent();

    std::string Read();
    void Send(const std::string &);

private:
    FdType fd;
    uint64_t id;
    ConnType connType;
    struct bufferevent *buffer;
    MessaageCallback messageCallback{nullptr};
    CloseCallback  closeCallback{nullptr};
};