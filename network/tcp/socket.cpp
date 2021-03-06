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

Socket::Socket(int sd, ConnectionManagerBasePtr connection_manager) {
    sd_ = sd;
    connection_manager_ = connection_manager;
};

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
    int status = ::listen(sd_, 500);
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

    if (info_.ip_addr.size() == 0) {
        printf("could not read server address: %s", info_.ip_addr.c_str());
        exit(EXIT_FAILURE);
    }

    addrinfo hints {};
    addrinfo *result;
    int status;

    hints.ai_family = AF_INET;    
    hints.ai_socktype = SOCK_STREAM; 
    hints.ai_flags = 0;
    hints.ai_protocol = 0;        

    status = getaddrinfo(info_.ip_addr.c_str(), info_.port_char, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }    

    using namespace std::chrono_literals;
    while(1) {
        status =  ::connect(sd_, result->ai_addr, result->ai_addrlen);
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
    
    pid_t x = syscall(__NR_gettid);
    // printf("pthread id: %u\n", x);
    
    sockaddr_in client;
    socklen_t slen = sizeof(client);
    int new_sock = ::accept(sd_, (sockaddr*) &client, &slen);
    if (new_sock == -1) {
        perror("error getting connection socket");
        return;
    }

    auto client_side_connection = client_->connect();
    
    auto connection = connection_manager_->create_connection(new_sock);
    connection->set_connection_pair(client_side_connection);
    client_side_connection->set_connection_pair(connection);
    
}

SocketBasePtr Socket::accept() {

    DEBUG_MSG("accepting new connection");
    
    sockaddr_in client_addr;
    socklen_t slen;
    int new_sock = ::accept(sd_, (sockaddr*) &client_addr, &slen);
    if (new_sock == -1) {
        perror("error getting connection socket");
        return create_socket(-1, connection_manager_);
    }

    return create_socket(new_sock, connection_manager_);

}

void Socket::set_client_side(Network::ClientBasePtr client) {
    client_ = client;
}

BufferPtr Socket::recv() {

    BufferPtr buf = create_buffer(BUF_SIZE);

    buf->lenght = ::recv(sd_, buf->message, buf->lenght, 0);

    return buf;

};

void Socket::send(BufferPtr buf) {

    buf->lenght = ::send(sd_, buf->message, buf->lenght, 0);

};

void Socket::close() {
    DEBUG_MSG("closing socket");
    ::close(sd_);
}


SocketPtr create_socket(ConnectionManagerBasePtr connection_manager, bool is_reuse) {
    return std::make_shared<Socket>(connection_manager, is_reuse);
};

SocketPtr create_socket(int sd, ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<Socket>(sd, connection_manager);
};


} // namespace Network

