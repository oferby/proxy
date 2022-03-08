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

};

void Dispatcher::run() {
    event_scheduler_->run();
}

Network::ConnectionManagerPtr Dispatcher::get_connection_manager() {
    return connection_manager_;
}

Event::EventSchedulerBasePtr Dispatcher::get_event_scheduler() {
    return std::static_pointer_cast<Event::EventSchedulerBase>(event_scheduler_);
}

} // namespace Event

