#include "verbs_common.h"

namespace Network {
namespace Roce {

AppContext::AppContext(RoceDevicePtr device, ProtectionDomainPtr pd, CompletionQueuePtr cq, SharedReceiveQueuePtr srq) :
    device_(device), pd_(pd), cq_(cq), srq_(srq) {};

AppContextPtr create_app_context(RoceDevicePtr device, ProtectionDomainPtr pd, CompletionQueuePtr cq,  SharedReceiveQueuePtr srq) {
    return std::make_shared<AppContext>(device, pd, cq, srq);
};

RoceDevicePtr AppContext::get_device() {return device_; };
ProtectionDomainPtr AppContext::get_pd() { return pd_; };
CompletionQueuePtr AppContext::get_cq() { return cq_; };
QueuePairPtr AppContext::get_qp() { return qp_; };
SharedReceiveQueuePtr AppContext::get_srq() { return srq_; };
void AppContext::set_qp(QueuePairPtr qp) { qp_ = qp; };


} // namespace Roce 
} // namespace Network
