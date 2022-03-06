#include "tcp_listener.h"

namespace Network {
namespace Tcp {

TcpListener::TcpListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher, Network::ConnectionManagerPtr connection_manager) 
    :info_(info), dispatcher_(dispatcher), connection_manager_(connection_manager) {

    puts("creating new tcp listener");

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sd == -1) {
        perror("could not create socket");
        exit(EXIT_FAILURE);

    }
    
    int optval = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

    sockaddr_in server {0};
    auto addrlen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(info.port);

    int status = bind(sd, (sockaddr*) &server, addrlen);
    if (status != 0) {
        perror("could not bind to socket");
        exit(EXIT_FAILURE);
    }

    status = listen(sd, 1);
    if (status != 0) {
        perror("could not listen to socket");
        exit(EXIT_FAILURE);
    }

    sd_ = Network::create_socket(sd, std::static_pointer_cast<ConnectionManagerBase>(connection_manager_));
    
    printf("listener listening on port %i\n", info.port);

}


Network::SocketBasePtr TcpListener::get_socket() {
    return std::static_pointer_cast<Network::SocketBase>(sd_);
}

TcpListenerPtr create_tcp_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher,
    Network::ConnectionManagerPtr connection_manager) {
    return std::make_shared<TcpListener>(info, dispatcher, connection_manager);
}


} // namespace TCP
} // namespace Network