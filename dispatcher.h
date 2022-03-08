#ifndef DISPATCHER
#define DISPATCHER

#include "common.h"

#include "network/connection_manager.h"
#include "tcp/tcp_listener.h"
#include "event_scheduler.h"
#include "proxy/proxy.h"

namespace Event {


class Dispatcher : public DispatcherBase, public std::enable_shared_from_this<Event::DispatcherBase> {
public:

    static std::shared_ptr<Dispatcher> getInstance() {
        static Dispatcher instance;
        return std::make_shared<Dispatcher>(instance);
    }
    
    void run();
    void new_proxy_config(Network::ProxyConfigPtr config);
    Network::ConnectionManagerBasePtr get_connection_manager();
    Event::EventSchedulerBasePtr get_event_scheduler() ;
    
private:
    Dispatcher();
    EventSchedulerPtr event_scheduler_;
    Network::ConnectionManagerPtr connection_manager_;
    std::vector<Network::Tcp::TcpListenerPtr> listeners_ = {};

};

using DispatcherPtr = std::shared_ptr<Dispatcher>;

} // namespace Dispatcher

#endif