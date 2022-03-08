#include "proxy.h"

namespace Network {
namespace Proxy {

ProxyPath::ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) : dispatcher_(dispatcher) {
    
    Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(config->source, 
        dispatcher_);

    if (config->destination.port != 0) {
        // there is destination
        client_ = Network::Tcp::create_tcp_client(config->destination, dispatcher_);
        client_->connect();
    }

}

ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<ProxyPath>(config, dispatcher);
}


}
}


