#include "dispatcher.h"

namespace Event {

Dispatcher::Dispatcher() {
    DEBUG_MSG("create dispatcher");
    event_scheduler_ = create_event_scheduler();
    connection_manager_ = Network::create_connection_manager(event_scheduler_);
    roce_connection_manager_ = Network::Roce::create_roce_connection_manager();
}

void Dispatcher::new_proxy_config(Network::ProxyConfigPtr config) {

    Network::Proxy::ProxyPathPtr proxy_path = Network::Proxy::create_proxy_path(config, shared_from_this());
    proxies_.push_back(proxy_path);
};

void Dispatcher::run() {
    event_scheduler_->run();
}

Network::ConnectionManagerBasePtr Dispatcher::get_connection_manager() {
    return std::static_pointer_cast<Network::ConnectionManagerBase>(connection_manager_);
}

Event::EventSchedulerBasePtr Dispatcher::get_event_scheduler() {
    return std::static_pointer_cast<Event::EventSchedulerBase>(event_scheduler_);
}

Network::Roce::RoceConnectionManagerPtr Dispatcher::get_roce_connection_manager() {
    return roce_connection_manager_;
}
} // namespace Event

