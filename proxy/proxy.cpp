#include "proxy.h"

namespace Network {
namespace Proxy {

ProxyPath::ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) : dispatcher_(dispatcher) {
    
    Network::ListenerPtr listener = get_listener(config->source);
    
    if (config->destination.port != 0) {
        // there is destination
        client_ = get_client(config->destination);
        auto listen_sock = listener->get_socket();
        listen_sock->set_client_side(client_);
    }

}


Network::ListenerPtr ProxyPath::get_listener(Network::addr_info info) {

        if (info.type == Network::TCP) {
        
            Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(info, 
            dispatcher_);

            return std::static_pointer_cast<Listener>(listener);
        }

        // Roce

        Network::Roce::RoceListenerPtr listener = Network::Roce::create_roce_listener(info, dispatcher_);
        return std::static_pointer_cast<Listener>(listener);

}



Network::ClientBasePtr ProxyPath::get_client(Network::addr_info info) {
    
    return std::static_pointer_cast<ClientBase>(Network::Tcp::create_tcp_client(info, dispatcher_));

}



ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<ProxyPath>(config, dispatcher);
}


}
}


