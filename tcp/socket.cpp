#include "socket.h"


namespace Network {

Socket::Socket(ConnectionManagerBasePtr connection_manager, bool is_reuse) : connection_manager_(connection_manager) {
    puts("new Socket created");

    sd_ = socket(AF_INET, SOCK_STREAM, 0);
    if ( sd_ == -1) {
        perror("could not create socket");
        exit(EXIT_FAILURE);

    }
    
    if (is_reuse) {
        int optval = 1;
        setsockopt(sd_, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
    }

}

int Socket::bind(Network::addr_info info) {
    info_ = info;
    sockaddr_in server {0};
    auto addrlen = sizeof(server);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(info.port);

    int status = ::bind(sd_, (sockaddr*) &server, addrlen);
    if (status != 0) {
        perror("could not bind to socket");
        exit(EXIT_FAILURE);
    }

    return status;
}

int Socket::listen() {
    int status = ::listen(sd_, 1);
    if (status != 0) {
        perror("could not listen to socket");
        exit(EXIT_FAILURE);
    }
    
    printf("listener listening on port %i\n", info_.port);

    return status;
}


int Socket::connect() {

    puts("connecting to server...");

    sockaddr_in server {0};
    auto addrlen = sizeof(server);

    if (info_.ip_addr.size() == 0) {
        server.sin_addr.s_addr = INADDR_ANY;
    } else {
        int result = inet_aton(info_.ip_addr.c_str(), &server.sin_addr);
        if (!result) {
            perror("invalid server address");
            exit(EXIT_FAILURE);

        }
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(info_.port);
    
    int status =  ::connect(sd_, (const sockaddr*) &server, sizeof(server));
    if ( status == -1) {
        perror("could not connect to server!");
        exit(EXIT_FAILURE);
    }

    return status;

}


int Socket::get() {
    return sd_;
}

void Socket::on_connect() {
    puts("got new connection");

    sockaddr_in client;
    socklen_t slen = sizeof(client);
    int new_sock = ::accept(sd_, (sockaddr*) &client, &slen);
    if (new_sock == -1) {
        perror("error getting connection socket");
        return;
    }

    connection_manager_->create_connection(new_sock);
    
}

SocketPtr create_socket(ConnectionManagerBasePtr connection_manager, bool is_reuse) {
    return std::make_shared<Socket>(connection_manager, is_reuse);
};


} // namespace Network

