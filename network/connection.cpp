#include "connection.h"


namespace Network {
namespace Connection {

Connection::Connection(int sd, ConnectionManagerBasePtr connection_manager) :
sd_(sd), connection_manager_(connection_manager) {};

int Connection::get_sock() {
    return sd_;
};

void Connection::on_read() {
    puts("got read event.");

    char buf[1024];
    ssize_t result;
    int i;

    size_t size = 0;
    char* buf_ = buf;
    while (1) {
        result = ::recv(sd_, buf_, sizeof(buf), 0);
        printf("result: %d\n", result);
        if (result <= 0)
            break;
        size+=result;
        buf_+=result;

    }

    if (result == 0) {
        puts("connection closed. clearing socket events.");
        on_close();
    } else if (result < 0) {
        if (errno == EAGAIN && size > 0 ) {
            connection_pair_->on_write(buf, size);  
            return;
        }
        perror("recv error\n");
    }

      

};

void Connection::on_write(char* buf, size_t size) {
    puts("writing to client");

    int result = ::write(sd_, buf, size);
    if (result == -1) {
        perror("error writing to client");
        return;
    }

    puts("writen data to client");    
    
}

void Connection::on_close() {
    puts("removing connection.");
    connection_manager_->close_connection(sd_);
}


void Connection::set_connection_pair(ConnectionBasePtr connection_pair) {
    connection_pair_ = std::static_pointer_cast<Connection>(connection_pair);
}

ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<Connection>(sd, connection_manager);
};



} // namespace Connection
} // namespace Network