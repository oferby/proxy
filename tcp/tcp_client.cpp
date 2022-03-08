#include "tcp_client.h"

namespace Network {
namespace Tcp {

TcpClient::TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher, 
    Network::ConnectionManagerPtr connection_manager) : target_(target), 
    dispatcher_(dispatcher), connection_manager_(connection_manager) {

    puts("creating new tcp listener");

    sd_ = Network::create_socket(std::static_pointer_cast<ConnectionManagerBase>(connection_manager_), false);

};

Network::SocketBasePtr TcpClient::get_socket() {
    return std::static_pointer_cast<SocketBase>(sd_);
}

Connection::ConnectionBasePtr TcpClient::connect() {

    sd_->connect();

    return connection_manager_->create_connection(sd_->get());

};

void TcpClient::close() { 
    ::close(sd_->get());     
};

} // namespace Tcp
} // namespace Network