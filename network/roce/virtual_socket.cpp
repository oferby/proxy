#include "verbs_common.h"

namespace Network {
namespace Roce {

// RoceVirtualSocket::RoceVirtualSocket(ConnectionManagerBasePtr connection_manager) {
//     connection_manager_ = connection_manager;
    
// };

// RoceVirtualSocketPtr create_roce_socket(ConnectionManagerBasePtr connection_manager) {
//     return std::make_shared<RoceVirtualSocket>(connection_manager);
// };

RoceVirtualSocket::RoceVirtualSocket(RoceConnectorPtr roce_connector) : roce_connector_(roce_connector) {};

RoceVirtualSocketPtr create_roce_socket(RoceConnectorPtr roce_connector) {
    return std::make_shared<RoceVirtualSocket>(roce_connector);
};

void RoceVirtualSocket::set_client_side(Network::ClientBasePtr client) {
    roce_connector_->set_client_side(client);
};

// TODO
int RoceVirtualSocket::connect(Network::addr_info info) { return 0; };
int RoceVirtualSocket::get() { return 0; };
void RoceVirtualSocket::on_connect() {};

SocketBasePtr RoceVirtualSocket::accept() { return nullptr; };
int RoceVirtualSocket::bind(Network::addr_info info) { return 0; };
int RoceVirtualSocket::listen() { return 0; };
BufferPtr RoceVirtualSocket::recv() { return nullptr; };
void RoceVirtualSocket::send(BufferPtr buf) {};
void RoceVirtualSocket::close() {};


} // namespace Roce
} // namespace Network
