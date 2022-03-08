#include "dispatcher.h"


namespace Event {

Dispatcher::Dispatcher() {
    puts("create dispatcher");
    event_scheduler_ = create_event_scheduler();
    connection_manager_ = Network::create_connection_manager(event_scheduler_);
}

void Dispatcher::add_listener(Network::addr_info info) {
    
    Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(info, 
        shared_from_this(),
        connection_manager_);
    listeners_.push_back(listener);

    Network::SocketBasePtr sock = listener->get_socket();
    auto simple_sock_ptr = sock.get();

    event_scheduler_->register_for_event(sock->get(), 
        [] (evutil_socket_t fd, short event, void* arg) {
            puts("got new event");
            static_cast<Network::Socket*>(arg)->on_connect();
        }, 
        (void*) simple_sock_ptr);



    

};

void Dispatcher::run() {
    event_scheduler_->run();
}

Network::ConnectionManagerPtr Dispatcher::get_connection_manager() {
    return connection_manager_;
}

Event::EventSchedulerPtr Dispatcher::get_event_scheduler() {
    return event_scheduler_;
}


} // namespace Event

