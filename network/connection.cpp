#include "connection.h"

namespace Network {
namespace Connection {

Connection::Connection(int sd, ConnectionManagerBasePtr connection_manager) :
    sd_(sd), connection_manager_(connection_manager) {
        buf_ = create_buffer(BUF_SIZE);
    printf("new connection created with socket id: %u\n", sd_);
};

ConnectionManagerBasePtr Connection::get_connection_manager() {
    return connection_manager_;
}

int Connection::get_sock() {
    return sd_;
};

void Connection::on_read() {
    
    DEBUG_MSG("got read event.");
    
    int result;
    size_t left = buf_->size;
    buf_->lenght = 0;
    auto msg = buf_->message;

    while (1) {
        
        result = ::recv(sd_, msg, left, 0);
        // printf("read result: %d\n", buf_->lenght);
        if (result <= 0)
            break;
        
        buf_->lenght += result;
        msg += result;
        left = buf_->size - buf_->lenght;

        if (left == 0)
            break;

    }

    if (buf_->lenght > 0)
        connection_pair_->on_write(buf_);

    if (result == 0) {
        DEBUG_MSG("connection closed. clearing socket events.");
        on_close();
    } else if (result < 0 & errno != EAGAIN) {
        DEBUG_MSG("error on_read()");
    }

};

void Connection::on_write(BufferPtr buf) {

    DEBUG_MSG("TCP on_write(buf)");
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

// local close
void Connection::on_close() {
    DEBUG_MSG("TCP on_close()");
    printf("socket id: %u\n", sd_);
    connection_manager_->close_connection(sd_);
    connection_pair_->close();
    clear_connection_pair();
    int status = ::close(sd_);
    if (status < 0) {
        DEBUG_MSG("error trying to close socket");
    }
    else
        DEBUG_MSG("TCP on_close() - socket closed.");
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

// remote close
void Connection::close() {
    DEBUG_MSG("TCP close()");
    connection_pair_.reset();
    connection_manager_->close_connection(sd_);
    ::close(sd_);
}

Connection::~Connection() {
    DEBUG_MSG("~Connection()");
}

ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<Connection>(sd, connection_manager);
};

} // namespace Connection
} // namespace Network