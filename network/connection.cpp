#include "connection.h"

namespace Network {
namespace Connection {

Connection::Connection(int sd, ConnectionManagerBasePtr connection_manager) :
    sd_(sd), connection_manager_(connection_manager) {
        buf_ = create_buffer(BUF_SIZE);
    };

int Connection::get_sock() {
    return sd_;
};

void Connection::on_read() {
    // DEBUG_MSG("got read event.");
    
    ssize_t result;
    int i;

    size_t size;
    while (1) {

        buf_->lenght = ::recv(sd_, buf_->message, BUF_SIZE, 0);
        // printf("read result: %d\n", buf_->lenght);
        if (buf_->lenght <= 0)
            break;
        connection_pair_->on_write(buf_);
    }

    if (buf_->lenght == 0) {
        DEBUG_MSG("connection closed. clearing socket events.");
        on_close();
    }

};

void Connection::on_write(BufferPtr buf) {
    // printf("writing %i bytes to client %i\n", size, sd_);

    buf->lenght = ::write(sd_, buf->message, buf->lenght);
    if (buf->lenght == -1) {

        if (errno == EAGAIN)
            return;

        perror("error writing to client");
        return;
    }

    // DEBUG_MSG("writen data to client");    
    
}

void Connection::on_close() {
    DEBUG_MSG("removing connection.");
    connection_manager_->close_connection(sd_);
}

void Connection::set_connection_pair(ConnectionBasePtr connection_pair) {
    connection_pair_ = std::static_pointer_cast<Connection>(connection_pair);
}

ConnectionBasePtr Connection::get_connection_pair() {
    if (connection_pair_ == nullptr)
        return nullptr;
    
    return std::static_pointer_cast<ConnectionBase>(connection_pair_);
}

void Connection::clear_connection_pair() {
    DEBUG_MSG("clearing connection pair.");
    connection_pair_.reset();
 }

 void Connection::close() {
    DEBUG_MSG("closing connection.");
    ::close(sd_);
 }

Connection::~Connection() {
    DEBUG_MSG("connection destroyed.");
}

ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<Connection>(sd, connection_manager);
};

} // namespace Connection
} // namespace Network