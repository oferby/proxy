#include "tcp_client.h"

namespace Network {
namespace Tcp {

TcpClient::TcpClient(Network::addr_info target, Event::DispatcherBasePtr dispatcher) {

    DEBUG_MSG("creating new tcp listener");

    dispatcher_ = dispatcher;
    target_ = target;

};

Network::SocketBasePtr TcpClient::create_client_socket() {
    auto sd = Network::create_socket(dispatcher_->get_connection_manager(), false);
    return std::static_pointer_cast<SocketBase>(sd);
};

Connection::ConnectionBasePtr TcpClient::connect(Network::SocketBasePtr sd) {

    sd->connect(target_);
    
    DEBUG_MSG("connected to upstream server.");

    return dispatcher_->get_connection_manager()->create_connection(sd->get());

};

TcpClientPtr create_tcp_client(Network::addr_info target, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<TcpClient>(target, dispatcher);
}
} // namespace Tcp
} // namespace Network