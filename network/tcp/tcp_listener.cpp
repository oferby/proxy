#include "tcp_listener.h"

namespace Network {
namespace Tcp {

TcpListener::TcpListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) 
    {

    DEBUG_MSG("creating new tcp listener");

    info_ = info;
    dispatcher_ = dispatcher; 

    sd_ = Network::create_socket(dispatcher_->get_connection_manager(), true);

    sd_->bind(info_);
    
    dispatcher_->get_event_scheduler()->register_for_event(sd_->get(), 
        [] (evutil_socket_t fd, short event, void* arg) {
            DEBUG_MSG("got new event");
            // static_cast<Network::Socket*>(arg)->on_connect();

            Network::Socket* ss = static_cast<Network::Socket*>(arg);
            ss->on_connect();

        }, 
        static_cast<void*>(sd_.get()));

    
    // void* v = (void*)sd_.get();
    // auto ss = static_cast<Network::Socket*>(v);
    // ss->on_connect();
    sd_->listen();

};


Network::SocketBasePtr TcpListener::get_socket() {
    return std::static_pointer_cast<Network::SocketBase>(sd_);
};

void TcpListener::set_client_side(Network::ClientBasePtr client) {
    sd_->set_client_side(client);
}   

TcpListenerPtr create_tcp_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<TcpListener>(info, dispatcher);
};


} // namespace TCP
} // namespace Network