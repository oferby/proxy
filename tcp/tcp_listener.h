#ifndef TCP_LISTENER
#define TCP_LISTENER

#include <string>
#include <sys/socket.h> 
#include <netinet/in.h>
#include "listener.h" 
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
    int sd_;

public:
    TcpListener(addr_info info);
    int get_socket();
    void onConnect();

};

using TcpListenerPtr = std::unique_ptr<TcpListener>;

TcpListenerPtr create_tcp_listener(addr_info info);

} // namespace Tcp
} // namespace Network

#endif