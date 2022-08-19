//
// Created on 2022/8/18.
//

#include "event2/event.h"
#include "EventLoop.h"

EventLoop::EventLoop():
    eventBase{nullptr} {

#ifdef _WIN32
    WSADATA wsa_data;
    WSAStartup(0x101, &wsa_data);
#endif

    eventBase = event_base_new();
}