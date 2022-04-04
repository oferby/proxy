#include "verbs_common.h"

namespace Network {
namespace Roce {

RoceVirtualSocket::RoceVirtualSocket(ConnectionManagerBasePtr connection_manager) {
    connection_manager_ = connection_manager;
    
};

RoceVirtualSocketPtr create_roce_socket(ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<RoceVirtualSocket>(connection_manager);
};


// TODO
int RoceVirtualSocket::connect(Network::addr_info info) { return 0; };
int RoceVirtualSocket::get() { return 0; };
void RoceVirtualSocket::on_connect() {};
void RoceVirtualSocket::set_client_side(Network::ClientBasePtr client) {};
SocketBasePtr RoceVirtualSocket::accept() { return nullptr; };
int RoceVirtualSocket::bind(Network::addr_info info) { return 0; };
int RoceVirtualSocket::listen() { return 0; };
BufferPtr RoceVirtualSocket::recv() { return nullptr; };
void RoceVirtualSocket::send(BufferPtr buf) {};
void RoceVirtualSocket::close() {};


} // namespace Roce
} // namespace Network
