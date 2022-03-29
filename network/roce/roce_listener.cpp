#include "verbs_common.h"


namespace Network {
namespace Roce {
    
RoceListener::RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    
    DEBUG_MSG("creating RoCE listener");
    
    info_ = info;
    dispatcher_ = dispatcher; 

    RoceDevicePtr device = create_roce_device(info.dev_name);
    
    ProtectionDomainPtr pd =  create_protection_domain(device);

    CompletionQueuePtr cq = create_completion_queue(device);

    SharedReceiveQueuePtr srq = create_srq(pd);

    app_ctx_ = create_app_context(device, pd, cq, srq);

    QueuePairPtr qp = create_queue_pair(app_ctx_);

    app_ctx_->set_qp(qp);

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




