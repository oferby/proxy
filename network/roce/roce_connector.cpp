#include "verbs_common.h"


namespace Network {
namespace Roce {

RoceConnector::RoceConnector(std::string dev_name) {

    RoceDevicePtr device = create_roce_device(dev_name);
    
    ProtectionDomainPtr pd =  create_protection_domain(device);

    CompletionQueuePtr cq = create_completion_queue(device);

    SharedReceiveQueuePtr srq = create_srq(pd);

    app_ctx_ = create_app_context(device, pd, cq, srq);

    QueuePairPtr qp = create_queue_pair(app_ctx_);

    app_ctx_->set_qp(qp);

};

RoceConnectorPtr create_roce_connector(std::string dev_name) {
    return std::make_shared<RoceConnector>(dev_name);
};


} // namespace Network
} // namespace Roce