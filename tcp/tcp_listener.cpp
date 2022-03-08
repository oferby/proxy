#include "tcp_listener.h"

namespace Network {
namespace Tcp {

TcpListener::TcpListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher, Network::ConnectionManagerPtr connection_manager) 
    :info_(info), dispatcher_(dispatcher), connection_manager_(connection_manager) {

    puts("creating new tcp listener");

    sd_ = Network::create_socket(std::static_pointer_cast<ConnectionManagerBase>(connection_manager_), true);

    sd_->bind(info_);

    sd_->listen();

}


Network::SocketBasePtr TcpListener::get_socket() {
    return std::static_pointer_cast<Network::SocketBase>(sd_);
}

TcpListenerPtr create_tcp_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher,
    Network::ConnectionManagerPtr connection_manager) {
    return std::make_shared<TcpListener>(info, dispatcher, connection_manager);
}


} // namespace TCP
} // namespace Network