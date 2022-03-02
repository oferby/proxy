#ifndef SOCKET
#define SOCKET

#include <memory>

namespace Network {

class Socket {
public:
    Socket(int sd);
    int get();
    void on_connect();

private:
    int sd_;
};

using SocketPtr = std::shared_ptr<Socket>;
SocketPtr create_socket(int sd);

} // namespace natwork



#endif