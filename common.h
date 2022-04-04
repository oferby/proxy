#ifndef COMMON
#define COMMON

#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { puts(str);} while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif


#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h> 
#include <netinet/in.h>
#include <netdb.h>
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

    struct Buffer {
        size_t lenght;
        int status;
        char* message;
        Buffer(size_t len);
    };
    using BufferPtr = std::shared_ptr<Buffer>;
    BufferPtr create_buffer(size_t len);

    class ConnectionManagerBase;
    using ConnectionManagerBasePtr = std::shared_ptr<ConnectionManagerBase>;

    enum transport {TCP, RoCE};

    struct addr_info {
        std::string ip_addr; 
        int port;
        transport type;
        std::string dev_name;
    };

    struct proxy_config {
        addr_info source;
        addr_info destination;
    };

    using ProxyConfigPtr = std::shared_ptr<proxy_config>;

    using SockAddrInPtr = std::shared_ptr<sockaddr_in>;
    using SockAddrLen = std::shared_ptr<socklen_t>;

    class ClientBase;
    using ClientBasePtr = std::shared_ptr<ClientBase>;


namespace Roce{
    class RoceConnectionManager;
    using RoceConnectionManagerPtr = std::shared_ptr<RoceConnectionManager>;
    

} // namespace Roce
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
        virtual Network::Roce::RoceConnectionManagerPtr get_roce_connection_manager() PURE;
    };

    using DispatcherBasePtr = std::shared_ptr<DispatcherBase>;

}; // namespace Event

namespace Network {

namespace Connection {
    
    class ConnectionBase;
    using ConnectionBasePtr = std::shared_ptr<ConnectionBase>;
    
    class ConnectionBase {
    public:
        virtual int get_sock() PURE;
        virtual void on_read() PURE;
        virtual void on_write(char* buf, size_t size) PURE;
        virtual void set_connection_pair(ConnectionBasePtr connection_pair) PURE;
        virtual void close() PURE;
        
    };
    

} // namespace Connection


    class ConnectionManagerBase {
    public:
        virtual Connection::ConnectionBasePtr create_connection(int sd) PURE;
        virtual void close_connection(int sd) PURE;
    };
    
    class SocketBase;
    using SocketBasePtr = std::shared_ptr<SocketBase>;

    class SocketBase {
    protected:
        int sd_;
        Network::addr_info info_;
        ConnectionManagerBasePtr connection_manager_;
        Network::ClientBasePtr client_;

    public:
        virtual int connect(Network::addr_info info) PURE;
        virtual int get() PURE;
        virtual void on_connect() PURE;
        virtual void set_client_side(Network::ClientBasePtr client) PURE;
        virtual int bind(Network::addr_info info) PURE;
        virtual int listen() PURE;
        virtual SocketBasePtr accept() PURE;
        virtual BufferPtr recv() PURE;
        virtual void send(BufferPtr buf) PURE;
        virtual void close() PURE;
    };
    
    
    class Listener {
    protected:
        Network::addr_info info_;
        Network::SocketBasePtr sd_;
        Event::DispatcherBasePtr dispatcher_;
    public:
        virtual Network::SocketBasePtr get_socket() PURE;
        virtual void set_client_side(Network::ClientBasePtr client) PURE;
    };
    using ListenerPtr = std::shared_ptr<Listener>;

    class ClientBase {
    protected:
        Network::addr_info target_;
        Event::DispatcherBasePtr dispatcher_;
    public:
        virtual Network::Connection::ConnectionBasePtr connect() PURE;
    };

} // namespace Network

#endif