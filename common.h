#ifndef COMMON
#define COMMON

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h> 
#include <netinet/in.h>

#include <string>
#include <functional>
#include <memory>
#include <map>
#include <vector>

#define PURE =0;



namespace Network {

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
        virtual void create_connection(int sd) PURE;
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


} // namespace Network

namespace Event {
    
    class EventSchedulerBase;
    
    // typedef void (*OnEventCallback) (int fd, short event, void* arg);
    using OnEventCallback = void (int fd, short event, void* arg); 
    using EventSchedulerBasePtr = std::shared_ptr<EventSchedulerBase>;
    
    class DispatcherBase {
    public:
        virtual void run() PURE;
        virtual void add_listener(Network::addr_info info) PURE;

    };
    using DispatcherBasePtr = std::shared_ptr<DispatcherBase>;

}; // namespace Event

#endif