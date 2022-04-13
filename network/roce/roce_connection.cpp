#include "verbs_common.h"

namespace Network {
namespace Roce {
    

RoceVirtualConnection::RoceVirtualConnection(uint32_t id, RoceConnectorPtr roce_connector) : id_(id), roce_connector_(roce_connector) { 

    DEBUG_MSG("Roce Virtual Connection created.");
    printf("Roce connection ID: %u\n", id_);

}


int RoceVirtualConnection::get_sock() {
    return 1;
}

void RoceVirtualConnection::on_read() {
    DEBUG_MSG("Roce on_read()");
}

void RoceVirtualConnection::on_read(BufferPtr buf) {
    DEBUG_MSG("Roce on_read(buf)");
    printf("SGE message: %s, IMM: %u\n", buf->message, id_);

    connection_pair_->on_write(buf);
    
}

void RoceVirtualConnection::on_write(BufferPtr buf) {
    DEBUG_MSG("Roce on_write()");
    roce_connector_->send(buf, id_);

}

void RoceVirtualConnection::set_connection_pair(Network::Connection::ConnectionBasePtr connection_pair) {
    connection_pair_ = connection_pair;
}

void RoceVirtualConnection::close() {
    DEBUG_MSG("Roce on_close()");
}

RoceVirtualConnectionPtr create_roce_connection(uint32_t id, RoceConnectorPtr roce_connector) {
    return std::make_shared<RoceVirtualConnection>(id, roce_connector);
}


} // namespace Roce
} // namespace Network
