#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

class Conn;
class EventLoop;
using ConnPtr = std::shared_ptr<Conn>;
using SocketEventType = short;
using EventLoopPtr = EventLoop*;
using ListenerType = struct evconnlistener *;
using EventBaseType = struct event_base *;
using FdType = int;

using ConnectedCallback = std::function<void(const ConnPtr &)>;
using MessaageCallback = std::function<void(const ConnPtr &)>;
using CloseCallback = std::function<void(const ConnPtr &)>;


using ConnMaps = std::unordered_map<uint16_t, ConnPtr>;