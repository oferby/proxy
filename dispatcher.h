#ifndef DISPATCHER
#define DISPATCHER

#include "event_scheduler.h"
#include "tcp/tcp_listener.h"

#include <vector>
#include <memory>

namespace Event {


class Dispatcher {
public:
    Dispatcher();
    void run();
    void add_listener(Network::Tcp::addr_info info);

private:
    Event::EventSchedulerPtr event_scheduler_;
    std::vector<Network::Tcp::TcpListenerPtr> listeners_ = {};

};


using DispatcherPtr = std::unique_ptr<Dispatcher>;

DispatcherPtr create_dispatcher();

} // namespace Dispatcher

#endif