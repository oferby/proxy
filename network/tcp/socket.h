#ifndef SOCKET
#define SOCKET

#include "../../common.h"

namespace Network {

class Socket : public SocketBase {

public:
    Socket(ConnectionManagerBasePtr connection_manager, bool is_reuse);
    int get();
    int connect(Network::addr_info info);
    int bind(Network::addr_info info);
    int listen();
    int accept(std::shared_ptr<sockaddr_in> client);
    virtual void on_connect();
    void set_client_side(Network::ClientBasePtr client) override;
    // int accept();
 

};

using SocketPtr = std::shared_ptr<Socket>;
SocketPtr create_socket(ConnectionManagerBasePtr connection_manager, bool is_reuse);

} // namespace natwork



#endif