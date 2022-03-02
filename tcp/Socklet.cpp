#include "Socket.h"

namespace Network {

Socket::Socket(int sd) {
    puts("new Socket created");
    sd_ = sd;
}

int Socket::get() {
    return sd_;
}

void Socket::on_connect() {
    puts("got new connection");
    
}

SocketPtr create_socket(int sd) {
    return std::make_shared<Socket>(sd);
};


} // namespace Network

