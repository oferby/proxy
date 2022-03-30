#include "verbs_common.h"
#include "../tcp/socket.h"

namespace Network {
namespace Roce {

RoceConnectionManager::RoceConnectionManager() {};

void RoceConnectionManager::set_dispatcher(Event::DispatcherBasePtr dispatcher) {
    dispatcher_ = dispatcher;
}
 
RoceListenerPtr RoceConnectionManager::create_roce_listener(Network::addr_info info) {
    
    if (listener_ == nullptr) {
        listener_ = Network::Roce::create_roce_listener(info, dispatcher_);
    }
    
    return listener_;
}

RoceClientPtr RoceConnectionManager::create_roce_client(Network::addr_info info) {

    auto iter = roce_clients_.find(info.ip_addr);
    
    if ( iter == roce_clients_.end()) {
        
        RoceClientPtr client = Network::Roce::create_roce_client(info, dispatcher_);
        roce_clients_[info.ip_addr] = client;
        return client;
    };

    return iter->second;

}

RoceConnectionManagerPtr create_roce_connection_manager() {
    return std::make_shared<RoceConnectionManager>();
};

} // namespace Roce 
} // namespace Network



