#ifndef DISPATCHER
#define DISPATCHER

#include "network/connection_manager.h"
#include "network/tcp/tcp_listener.h"
#include "event_scheduler.h"
#include "proxy/proxy.h"

namespace Event {

class Dispatcher : public DispatcherBase, public std::enable_shared_from_this<Event::DispatcherBase> {
public:

    Dispatcher();    
    void run();
    void new_proxy_config(Network::ProxyConfigPtr config);
    Network::ConnectionManagerBasePtr get_connection_manager();
    Event::EventSchedulerBasePtr get_event_scheduler() ;
    Network::Roce::RoceConnectionManagerPtr get_roce_connection_manager();
    
private:
    EventSchedulerPtr event_scheduler_;
    Network::ConnectionManagerPtr connection_manager_;
    std::vector<Network::Proxy::ProxyPathPtr> proxies_ {};
    
    Network::Roce::RoceConnectionManagerPtr roce_connection_manager_;

};

using DispatcherPtr = std::shared_ptr<Dispatcher>;
DispatcherPtr get_dispatcher();


} // namespace Dispatcher

#endif