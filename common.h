#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <vector>

#define PURE =0;



namespace Network {

    using SockAddrInPtr = std::shared_ptr<sockaddr_in>;
    using SockAddrLen = std::shared_ptr<socklen_t>;
    
    namespace Connection {
    
    class ConnectionBase {
    public:
        virtual int get_sock() const PURE;
        virtual void on_read() PURE;
        virtual void on_write() PURE;
    };

    using ConnectionBasePtr = std::shared_ptr<ConnectionBase>;

    } // namespace Connection

    class ConnectionManagerBase {
    public:
        virtual Connection::ConnectionBasePtr create_connection(int sd) PURE;
        virtual void close_connection(int sd) PURE;
    };
    using ConnectionManagerBasePtr = std::shared_ptr<ConnectionManagerBase>;

    class SocketBase {
    public:
        virtual int get() PURE;
        virtual void on_connect() PURE;
    };

    using SocketBasePtr = std::shared_ptr<SocketBase>;

    class Listener {
    public:
        virtual Network::SocketBasePtr get_socket() PURE;
    };
    using ListenerPtr = std::shared_ptr<Listener>;

    struct addr_info {
        std::string ip_addr; 
        int port;
    };

    struct proxy_config {
        addr_info source;
        addr_info destination;
    };


} // namespace Network

namespace Event {
    
    // typedef void (*OnEventCallback) (int fd, short event, void* arg);
    using OnEventCallback = void (int fd, short event, void* arg); 
    

    class EventSchedulerBase {
    public:
        virtual void run() PURE;
        virtual void register_for_event(int fd, OnEventCallback cb, void* arg) PURE;
        virtual void unregister_for_event(int fd) PURE;
        virtual void make_nonblocking(int fd) PURE;
    };

    using EventSchedulerBasePtr = std::shared_ptr<EventSchedulerBase>;   
    
    class DispatcherBase {
    public:
        virtual void run() PURE;
        virtual void add_listener(Network::addr_info info) PURE;
        virtual Event::EventSchedulerBasePtr get_event_scheduler()  PURE;

    };
    using DispatcherBasePtr = std::shared_ptr<DispatcherBase>;

}; // namespace Event

#endif