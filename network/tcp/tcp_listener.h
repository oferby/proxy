#ifndef TCP_LISTENER
#define TCP_LISTENER

#include "../connection_manager.h"
#include "socket.h"



namespace Network {
namespace Tcp {

class TcpListener : public Network::Listener {

    
public:
    TcpListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket() override;
    void set_client_side(Network::ClientBasePtr client) override;
};

using TcpListenerPtr = std::shared_ptr<TcpListener>;
TcpListenerPtr create_tcp_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);

} // namespace Tcp
} // namespace Network

#endif