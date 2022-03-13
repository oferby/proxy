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
#include <thread>

#define PURE =0;


namespace Network {

    struct addr_info {
        std::string ip_addr; 
        int port;
    };

    using AddrInfoPtr = std::shared_ptr<addr_info>;

    struct proxy_config {
        addr_info source;
        addr_info destination;
    };

    using ProxyConfigPtr = std::shared_ptr<proxy_config>;

    using SockAddrInPtr = std::shared_ptr<sockaddr_in>;
    using SockAddrLen = std::shared_ptr<socklen_t>;

    namespace Tcp {
        class TcpClientBase;
        using TcpClientBasePtr = std::shared_ptr<TcpClientBase>;
    } // namespace Tcp
    
    namespace Connection {
    
    class ConnectionBase;
    using ConnectionBasePtr = std::shared_ptr<ConnectionBase>;
    
    class ConnectionBase {
    public:
        virtual int get_sock() PURE;
        virtual void on_read() PURE;
        virtual void on_write(char* buf, size_t size) PURE;
        virtual void set_connection_pair(ConnectionBasePtr connection_pair) PURE;
        
    };
    

    } // namespace Connection


    class ConnectionManagerBase {
    public:
        virtual Connection::ConnectionBasePtr create_connection(int sd) PURE;
        virtual void close_connection(int sd) PURE;
    };
    
    using ConnectionManagerBasePtr = std::shared_ptr<ConnectionManagerBase>;

    class SocketBase {
    public:
        virtual int connect(Network::addr_info info) PURE;
        virtual int get() PURE;
        virtual void on_connect() PURE;
        virtual void set_client_side(Network::Tcp::TcpClientBasePtr client) PURE;
    };

    using SocketBasePtr = std::shared_ptr<SocketBase>;

    class Listener {
    public:
        virtual Network::SocketBasePtr get_socket() PURE;
    };
    using ListenerPtr = std::shared_ptr<Listener>;



namespace Tcp {

    class TcpClientBase {
    public:
        virtual Network::SocketBasePtr get_socket() PURE;
        virtual Network::Connection::ConnectionBasePtr connect(Network::SocketBasePtr sd) PURE;
    };

} // namespace Tcp
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
        virtual Event::EventSchedulerBasePtr get_event_scheduler() PURE;
        virtual Network::ConnectionManagerBasePtr get_connection_manager() PURE;
    };

    using DispatcherBasePtr = std::shared_ptr<DispatcherBase>;

}; // namespace Event

#endif