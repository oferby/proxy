#include "verbs_common.h"

namespace Network {
namespace Roce {
    

RoceVirtualConnection::RoceVirtualConnection(uint32_t id, RoceConnectorPtr roce_connector) : id_(id), roce_connector_(roce_connector) { 

    DEBUG_MSG("Roce Virtual Connection created.");
    printf("Roce connection ID: %u\n", id_);

}

RoceVirtualConnection::~RoceVirtualConnection() {
    DEBUG_MSG("~RoceVirtualConnection()");
}


int RoceVirtualConnection::get_sock() {
    return 1;
}

void RoceVirtualConnection::on_read() {
    DEBUG_MSG("Roce on_read()");
}

void RoceVirtualConnection::on_read(BufferPtr buf) {
    DEBUG_MSG("Roce on_read(buf)");
    // printf("SGE message: %s, IMM: %u\n", buf->message, id_);

    
    connection_pair_->on_write(buf);

}

void RoceVirtualConnection::on_write(BufferPtr buf) {
    
    ++sending_;
    
    DEBUG_MSG("Roce on_write()");
    
    roce_connector_->send(buf, id_);

}

void RoceVirtualConnection::on_write_complete() {
    
    DEBUG_MSG("Roce on_write_complete()");

    --sending_;

    if (pending_close_) {
        close();
    }
}

void RoceVirtualConnection::set_connection_pair(Network::Connection::ConnectionBasePtr connection_pair) {
    connection_pair_ = connection_pair;
}

// on client side
void RoceVirtualConnection::close() {

    DEBUG_MSG("RoCE close()");

    if (sending_ > 0) {
        DEBUG_MSG("RoCE close pending");
        return;
    }

    roce_connector_->close(id_);
        
}

// on listener side
void RoceVirtualConnection::on_close() {
    
    DEBUG_MSG("Roce on_close");
    
    if (pending_close_)
        return;

    connection_pair_->close();
    connection_pair_.reset();
    roce_connector_->on_close(id_);
}

RoceVirtualConnectionPtr create_roce_connection(uint32_t id, RoceConnectorPtr roce_connector) {
    return std::make_shared<RoceVirtualConnection>(id, roce_connector);
}


} // namespace Roce
} // namespace Network
