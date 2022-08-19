//
// Created on 2022/8/18.
//
#include <iostream>
#include <cstring>

#include "event2/bufferevent.h"
#include "event2/buffer.h"

#include "Conn.h"

Conn::~Conn() {
    bufferevent_free(buffer);
}

void Conn::OnEvent(SocketEventType events) {
    if (events & (BEV_EVENT_EOF)) {
        std::cout << "socket closing. fd: " << fd << ". id: " << id << std::endl;
        closeCallback(shared_from_this());
    } else if (events & BEV_EVENT_ERROR) {
        std::cerr << "Got an error on the connection. fd: " << fd << ". id: " << id << ". msg: " << evutil_socket_error_to_string(errno) << std::endl;
        closeCallback(shared_from_this());
    }
}

void Conn::OnReadEvent() {
    messageCallback(shared_from_this());
}

std::string Conn::Read() {
    auto input = bufferevent_get_input(buffer);
    char data[4096];
    auto length = evbuffer_remove(input, data, sizeof(data)-1);
    data[length] = '\0';
    return data;
}


void Conn::Send(const std::string &message) {
    bufferevent_write(buffer, message.c_str(), message.length());
};