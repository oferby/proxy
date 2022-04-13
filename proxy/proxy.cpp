#include "proxy.h"

namespace Network {
namespace Proxy {

ProxyPath::ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) : config_(config), dispatcher_(dispatcher) {
    
    listener_ = get_listener(config->source);
    
    client_ = get_client(config->destination, listener_);

}


Network::ListenerPtr ProxyPath::get_listener(Network::addr_info info) {

        if (info.type == Network::TCP) {
        
            Network::Tcp::TcpListenerPtr listener = Network::Tcp::create_tcp_listener(info, 
            dispatcher_);

            return listener;
        } 

        // Roce

        Network::Roce::RoceListenerPtr listener = dispatcher_->get_roce_connection_manager()->create_roce_listener(info);
        return listener;

}


Network::ClientBasePtr ProxyPath::get_client(Network::addr_info info, Network::ListenerPtr listener) {
    
    if (info.port != 0) {
        // there is destination

        Network::ClientBasePtr client;

        if (info.type == Network::TCP) {
            
            client = std::static_pointer_cast<ClientBase>(Network::Tcp::create_tcp_client(info, dispatcher_));
 
        } else {
            
            client = std::static_pointer_cast<ClientBase>(dispatcher_->get_roce_connection_manager()->create_roce_client(info));
        }

        auto listen_sock = listener->get_socket();
        listen_sock->set_client_side(client);
        return client;
        
    };

    return nullptr;

}


Network::ProxyConfigPtr  ProxyPath::get_config() {
    return config_;
}

ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<ProxyPath>(config, dispatcher);
}


} // namespace Proxy
} // namespace Network


