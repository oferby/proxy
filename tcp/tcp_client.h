#ifndef TCP_LISTENER
#define TCP_LISTENER

#include "../common.h"
#include "../network/connection_manager.h"
#include "socket.h"


namespace Network {
namespace Tcp {

class TcpClient {
private:
    Network::addr_info target_;
    Network::SocketPtr sd_;
    Network::ConnectionManagerPtr connection_manager_;
    Event::DispatcherBasePtr dispatcher_;

public:
    TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher, Network::ConnectionManagerPtr connection_manager);
    Network::SocketBasePtr get_socket();
    Connection::ConnectionBasePtr connect();
    void close();
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
TcpClientPtr create_tcp_client(Network::addr_info target, Event::DispatcherBasePtr dispatcher, Network::ConnectionManagerPtr connection_manager);

} // namespace Tcp
} // namespace Network

#endif