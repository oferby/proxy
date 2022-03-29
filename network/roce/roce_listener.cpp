#include "verbs_common.h"


namespace Network {
namespace Roce {
    
RoceListener::RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    
    DEBUG_MSG("creating RoCE listener");
    
    info_ = info;
    dispatcher_ = dispatcher; 

    roce_connector_ = create_roce_connector(info.dev_name);

    sd_ = create_roce_socket(dispatcher_->get_connection_manager());
    
};

Network::SocketBasePtr RoceListener::get_socket() {
    return sd_;
};

void RoceListener::set_client_side(Network::ClientBasePtr client) {
    sd_->set_client_side(client);
};

RoceListenerPtr create_roce_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<RoceListener>(info, dispatcher);
}

} // namespace Roce
} // namespace Network




