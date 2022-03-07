#include "connection.h"


namespace Network {
namespace Connection {

Connection::Connection(int sd, ConnectionManagerBasePtr connection_manager) :
sd_(sd), connection_manager_(connection_manager) {};

int Connection::get_sock() const {
    return sd_;
};

void Connection::on_read() {
    puts("got read event.");

    char buf[1024];
    ssize_t result;
    int i;

    while (1) {
        result = recv(sd_, buf, sizeof(buf), 0);
        printf("result: %d\n", result);
        if (result <= 0)
            break;
    }

    if (result == 0) {
        puts("connection closed. clearing socket events.");
        on_close();
    } else if (result < 0) {
        if (errno == EAGAIN) {
            return;
        }
        perror("recv error\n");
    }    

};

void Connection::on_write() {
    puts("got write event");
    
}

void Connection::on_close() {
    puts("removing connection.");
    connection_manager_->close_connection(sd_);
}

ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<Connection>(sd, connection_manager);
};

} // namespace Connection
} // namespace Network