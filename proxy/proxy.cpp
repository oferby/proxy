#include "proxy.h"

namespace Network {
namespace Proxy {

ProxyPath::ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) : dispatcher_(dispatcher) {
    
    listener_ = get_listener(config->source);
    
    client_ = get_client(config->destination, listener_);

}


Network::ListenerPtr ProxyPath::get_listener(Network::addr_info info) {

        if (info.type == Network::TCP) {
        
            Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(info, 
            dispatcher_);

            // return std::static_pointer_cast<Listener>(listener);
            return listener;
        } 

        // Roce

        Network::Roce::RoceListenerPtr listener = Network::Roce::create_roce_listener(info, dispatcher_);
        // return std::static_pointer_cast<Listener>(listener);
        return listener;

}


Network::ClientBasePtr ProxyPath::get_client(Network::addr_info info, Network::ListenerPtr listener) {
    
    if (info.port != 0) {
        // there is destination

        Network::ClientBasePtr client;

        if (info.type == Network::TCP) {
            
            client = std::static_pointer_cast<ClientBase>(Network::Tcp::create_tcp_client(info, dispatcher_));
 
        } else {

            client = std::static_pointer_cast<ClientBase>(Network::Roce::create_roce_client(info, dispatcher_));
        }

        auto listen_sock = listener->get_socket();
        listen_sock->set_client_side(client);
        return client;
        
    };

    return nullptr;

}


ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<ProxyPath>(config, dispatcher);
}


} // namespace Proxy
} // namespace Network


