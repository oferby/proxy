#include "tcp_listener.h"

namespace Network {
namespace Tcp {

TcpListener::TcpListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) 
    :info_(info), dispatcher_(dispatcher) {

    puts("creating new tcp listener");

    sd_ = Network::create_socket(dispatcher_->get_connection_manager(), true);

    sd_->bind(info_);

    dispatcher_->get_event_scheduler()->register_for_event(sd_->get(), 
        [] (evutil_socket_t fd, short event, void* arg) {
            puts("got new event");
            static_cast<Network::Socket*>(arg)->on_connect();
        }, 
        (void*) sd_.get());

    sd_->listen();

}


Network::SocketBasePtr TcpListener::get_socket() {
    return std::static_pointer_cast<Network::SocketBase>(sd_);
}

TcpListenerPtr create_tcp_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<TcpListener>(info, dispatcher);
}


} // namespace TCP
} // namespace Network