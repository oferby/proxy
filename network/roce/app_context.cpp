#include "verbs_common.h"

namespace Network {
namespace Roce {

AppContext::AppContext() {
    
};

AppContextPtr create_app_context() {
    return std::make_shared<AppContext>();
};

void AppContext::set_device(RoceDevicePtr device) {

};

void AppContext::set_protection_domain(ProtectionDomainPtr pd) {

};

void AppContext::set_completion_queue(CompletionQueuePtr cq) {

};

void AppContext::set_queue_pair(QueuePairPtr qp) {

};

} // namespace Roce 
} // namespace Network
