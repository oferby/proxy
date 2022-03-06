#ifndef SOCKET
#define SOCKET

#include "../common.h"

namespace Network {

class Socket : public SocketBase {
public:
    Socket(int sd, ConnectionManagerBasePtr connection_manager);
    int get();
    void on_connect();
 
private:
    int sd_;
    ConnectionManagerBasePtr connection_manager_;
};

using SocketPtr = std::shared_ptr<Socket>;
SocketPtr create_socket(int sd, ConnectionManagerBasePtr connection_manager);

} // namespace natwork



#endif