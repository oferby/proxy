#ifndef TCP_LISTENER
#define TCP_LISTENER

#include "../../common.h"
#include "../connection_manager.h"
#include "socket.h"

#include <sys/socket.h> 
#include <netinet/in.h>

namespace Network {
namespace Tcp {

class TcpListener : public Network::Listener {
private:
    Network::addr_info info_;
    Network::SocketPtr sd_;
    Event::DispatcherBasePtr dispatcher_;
    
public:
    TcpListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket();
};

using TcpListenerPtr = std::shared_ptr<TcpListener>;
TcpListenerPtr create_tcp_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);

} // namespace Tcp
} // namespace Network

#endif