#include "tcp_listener.h"

namespace Network {
namespace Tcp {

TcpListener::TcpListener(addr_info info) {

    puts("creating new tcp listener");
    
    info_ = info;

    sd_ = socket(AF_INET, SOCK_STREAM, 0);
    if ( sd_ == -1) {
        perror("could not create socket");
        exit(EXIT_FAILURE);

    }
    
    int optval = 1;
    setsockopt(sd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    sockaddr_in server {0};
    auto addrlen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(info.port);

    int status = bind(sd_, (sockaddr*) &server, addrlen);
    if (status != 0) {
        perror("could not bind to socket");
        exit(EXIT_FAILURE);
    }

    status = listen(sd_, 1);
    if (status != 0) {
        perror("could not listen to socket");
        exit(EXIT_FAILURE);
    }

    puts("listener listening");

}


int TcpListener::get_socket() {
    return sd_;
}

void TcpListener::onConnect() {
    puts("got new connection.");
}

TcpListenerPtr create_tcp_listener(addr_info info) {
    return std::make_unique<TcpListener>(info);
}


} // namespace TCP
} // namespace Network