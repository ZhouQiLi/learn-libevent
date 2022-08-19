#pragma once

#include "Type.h"

class EventLoop {
public:
    EventLoop();
    EventLoop(const EventLoop &) = delete;
    EventLoop &operator=(const EventLoop &) = delete;
    struct event_base *EventBase() {
        return eventBase;
    }

private:
    EventBaseType eventBase;
};