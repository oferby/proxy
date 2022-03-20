#include "verbs_common.h"


namespace Network {
namespace Roce {
    
RoceListener::RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    
    DEBUG_MSG("creating RoCE listener");
    
    info_ = info;
    dispatcher_ = dispatcher; 

    sd_ = std::static_pointer_cast<SocketBase>(create_roce_socket(dispatcher_->get_connection_manager()));

    app_ctx_ = create_app_context();
    RoceDevicePtr device = create_roce_device(info.dev_name);
    app_ctx_->set_device(device);
    // app_ctx_->set_protection_domain(create_protection_domain(device));
    // app_ctx_->set_completion_queue(create_completion_queue(device));

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




