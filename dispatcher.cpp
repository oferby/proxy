#include "dispatcher.h"


namespace Event {

Dispatcher::Dispatcher() {
    puts("create dispatcher");
    event_scheduler_ = Event::create_event_scheduler();
}

DispatcherPtr create_dispatcher() {
    return std::make_unique<Dispatcher>();
};

void Dispatcher::add_listener(Network::Tcp::addr_info info) {
    
    Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(info);
    listeners_.push_back(listener);

    Network::SocketPtr sock = listener->get_socket();
    auto simple_sock_ptr = sock.get();

    event_scheduler_->register_for_event(sock->get(), 
        [] (evutil_socket_t fd, short event, void* arg) {
            static_cast<Network::Socket*>(arg)->on_connect();
        }, 
        (void*) simple_sock_ptr);
};

void Dispatcher::run() {
    event_scheduler_->run();
}

} // namespace Event

