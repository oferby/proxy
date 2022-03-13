#include "tcp_client.h"

namespace Network {
namespace Tcp {

TcpClient::TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher) : target_(target), 
    dispatcher_(dispatcher) {

    puts("creating new tcp listener");

    sd_ = Network::create_socket(dispatcher_->get_connection_manager(), false);

};

Network::SocketBasePtr TcpClient::get_socket() {
    return std::static_pointer_cast<SocketBase>(sd_);
};

Connection::ConnectionBasePtr TcpClient::connect() {

    sd_->connect(target_);
    
    puts("connected to upstream server.");

    return dispatcher_->get_connection_manager()->create_connection(sd_->get());

};

void TcpClient::close() { 
    ::close(sd_->get());     
};

TcpClientPtr create_tcp_client(Network::addr_info target, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<TcpClient>(target, dispatcher);
}
} // namespace Tcp
} // namespace Network