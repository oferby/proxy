#include "socket.h"


namespace Network {

Socket::Socket(int sd, ConnectionManagerBasePtr connection_manager) {
    
    sd_ = sd;
    connection_manager_ = connection_manager;

    puts("new Socket created");
}

int Socket::get() {
    return sd_;
}

void Socket::on_connect() {
    puts("got new connection");

    sockaddr_in client;
    socklen_t slen = sizeof(client);
    int new_sock = accept(sd_, (sockaddr*) &client, &slen);
    if (new_sock == -1) {
        perror("error getting connection socket");
        return;
    }

    connection_manager_->create_connection(new_sock);
    
}

SocketPtr create_socket(int sd, ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<Socket>(sd, connection_manager);
};


} // namespace Network

