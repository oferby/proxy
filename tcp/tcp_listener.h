#ifndef TCP_LISTENER
#define TCP_LISTENER

#include "listener.h" 
#include "Socket.h"

#include <string>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <memory>

namespace Network {
namespace Tcp {

struct addr_info {
    std::string ip_addr; 
    int port;
};


class TcpListener : public Network::Listener {
private:
    addr_info info_;
    Network::SocketPtr sd_;

public:
    TcpListener(addr_info info);
    Network::SocketPtr get_socket();
    void onConnect();

};

using TcpListenerPtr = std::shared_ptr<TcpListener>;

TcpListenerPtr create_tcp_listener(addr_info info);

} // namespace Tcp
} // namespace Network

#endif