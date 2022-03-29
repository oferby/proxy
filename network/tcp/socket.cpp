#include "socket.h"


namespace Network {

Socket::Socket(ConnectionManagerBasePtr connection_manager, bool is_reuse) {
    DEBUG_MSG("new Socket created");

    connection_manager_ = connection_manager;

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
    int status = ::listen(sd_, 5);
    if (status != 0) {
        perror("could not listen to socket");
        exit(EXIT_FAILURE);
    }
    
    printf("listener listening on port %i\n", info_.port);

    return status;
}


int Socket::connect(Network::addr_info info) {

    info_ = info;

    DEBUG_MSG("connecting to server...");

    sockaddr_in server {0};
    auto addrlen = sizeof(server);

    if (info_.ip_addr.size() == 0) {
        printf("could not read server address: %s", info_.ip_addr.c_str());
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
        // inet_aton(info_.ip_addr.c_str(), &server.sin_addr);
    hostent* host_ = gethostbyname(info_.ip_addr.c_str());

    if (host_ == nullptr) {
        puts("could not resolve hostname.");
        exit(EXIT_FAILURE);
    }

    in_addr * address = (in_addr * )host_->h_addr;
    std::string ip_address = inet_ntoa(* address);
    printf("server ip: %s\n",ip_address.c_str());
    server.sin_addr.s_addr = inet_addr(ip_address.c_str());
    
    server.sin_port = htons(info_.port);
    
    int status;
    using namespace std::chrono_literals;
    while(1) {
        status =  ::connect(sd_, (const sockaddr*) &server, sizeof(server));
        if ( status == -1) {
            perror("could not connect to server! trying ...");
            std::this_thread::sleep_for(2s);
        } else {
            break;
        }
    }
    
    return status;

}


int Socket::get() {
    return sd_;
}

void Socket::on_connect() {
    DEBUG_MSG("got new connection");

    sockaddr_in client;
    socklen_t slen = sizeof(client);
    int new_sock = ::accept(sd_, (sockaddr*) &client, &slen);
    if (new_sock == -1) {
        perror("error getting connection socket");
        return;
    }

    auto client_sock = client_->create_client_socket();
    auto client_side_connection = client_->connect(client_sock);
    
    auto connection = connection_manager_->create_connection(new_sock);
    connection->set_connection_pair(client_side_connection);
    client_side_connection->set_connection_pair(connection);
    
}

void Socket::set_client_side(Network::ClientBasePtr client) {
    client_ = client;
}

SocketPtr create_socket(ConnectionManagerBasePtr connection_manager, bool is_reuse) {
    return std::make_shared<Socket>(connection_manager, is_reuse);
};


} // namespace Network

