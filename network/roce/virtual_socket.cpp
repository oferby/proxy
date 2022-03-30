#include "verbs_common.h"

namespace Network {
namespace Roce {

RoceVirtualSocket::RoceVirtualSocket(ConnectionManagerBasePtr connection_manager) {
    connection_manager_ = connection_manager;
    
};

RoceVirtualSocketPtr create_roce_socket(ConnectionManagerBasePtr connection_manager) {
    return std::make_shared<RoceVirtualSocket>(connection_manager);
};

int RoceVirtualSocket::connect(Network::addr_info info) { return 0; };
int RoceVirtualSocket::get() { return 0; };
void RoceVirtualSocket::on_connect() {};
void RoceVirtualSocket::set_client_side(Network::ClientBasePtr client) {};
int RoceVirtualSocket::accept(std::shared_ptr<sockaddr_in> client) { return 0; };
int RoceVirtualSocket::bind(Network::addr_info info) { return 0; };
int RoceVirtualSocket::listen() { return 0; };


} // namespace Roce
} // namespace Network
