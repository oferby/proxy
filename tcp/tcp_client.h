#ifndef TCP_CLIENT
#define TCP_CLIENT

#include "../common.h"
#include "../network/connection_manager.h"
#include "socket.h"


namespace Network {
namespace Tcp {

class TcpClient {
private:
    Network::addr_info target_;
    Network::SocketPtr sd_;
    Event::DispatcherBasePtr dispatcher_;

public:
    TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket();
    Connection::ConnectionBasePtr connect();
    void close();
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
TcpClientPtr create_tcp_client(Network::addr_info target, Event::DispatcherBasePtr dispatcher);

} // namespace Tcp
} // namespace Network

#endif