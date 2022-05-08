#ifndef SOCKET
#define SOCKET

#include "../../common.h"

namespace Network {

class Socket : public SocketBase {

public:
    Socket(ConnectionManagerBasePtr connection_manager, bool is_reuse);
    Socket(int sd, ConnectionManagerBasePtr connection_manager);
    int get();
    int connect(Network::addr_info info);
    int bind(Network::addr_info info);
    int listen();
    SocketBasePtr accept();
    virtual void on_connect();
    void set_client_side(Network::ClientBasePtr client) override;
    BufferPtr recv();
    void send(BufferPtr buf);
    void close();

};

using SocketPtr = std::shared_ptr<Socket>;
SocketPtr create_socket(ConnectionManagerBasePtr connection_manager, bool is_reuse);
SocketPtr create_socket(int sd, ConnectionManagerBasePtr connection_manager);

} // namespace natwork



#endif