#ifndef TCP_CLIENT
#define TCP_CLIENT

#include "../../common.h"
#include "../connection_manager.h"
#include "socket.h"


namespace Network {
namespace Tcp {

class TcpClient : public ClientBase {
private:
    Network::addr_info target_;
    Event::DispatcherBasePtr dispatcher_;

public:
    TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket();
    Network::Connection::ConnectionBasePtr connect(Network::SocketBasePtr sd);
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
TcpClientPtr create_tcp_client(Network::addr_info target, Event::DispatcherBasePtr dispatcher);

} // namespace Tcp
} // namespace Network

#endif