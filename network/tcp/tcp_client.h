#ifndef TCP_CLIENT
#define TCP_CLIENT

#include "../connection_manager.h"
#include "socket.h"

namespace Network {
namespace Tcp {

class TcpClient : public ClientBase {
public:
    TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher);
    Network::Connection::ConnectionBasePtr connect();
};

using TcpClientPtr = std::shared_ptr<TcpClient>;
TcpClientPtr create_tcp_client(Network::addr_info target, Event::DispatcherBasePtr dispatcher);

} // namespace Tcp
} // namespace Network

#endif