#include "connection.h"


namespace Network {
namespace Connection {

Connection::Connection(int sd) {
    sd_ = sd;
};


int Connection::get_sock() const {
    return sd_;
};

void Connection::on_read() {
    puts("got read event.");
};

void Connection::on_write() {
    puts("got write event");
    
}

ConnectionPtr create_connection(int sd) {
    return std::make_shared<Connection>(sd);
};

} // namespace Connection
} // namespace Network