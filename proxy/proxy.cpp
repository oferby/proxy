#include "proxy.h"

namespace Network {
namespace Proxy {

ProxyPath::ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) : dispatcher_(dispatcher) {
    
    Network::ListenerPtr listener = get_listener(config);

    
    if (config->destination.port != 0) {
        // there is destination
        client_ = Network::Tcp::create_tcp_client(config->destination, dispatcher_);
        auto listen_sock = listener->get_socket();
        listen_sock->set_client_side(std::static_pointer_cast<Network::ClientBase>(client_));
    }

}


Network::ListenerPtr ProxyPath::get_listener(Network::ProxyConfigPtr config) {

        Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(config->source, 
        dispatcher_);

        return std::static_pointer_cast<Listener>(listener);

}











ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<ProxyPath>(config, dispatcher);
}


}
}


