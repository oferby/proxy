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
    listeners_.push_back(Network::Tcp::create_tcp_listener(info));
};

void Dispatcher::run() {
    event_scheduler_->run();
}

} // namespace Event

