#include "verbs_common.h"

namespace Network {
namespace Roce {
        
ProtectionDomain::ProtectionDomain(RoceDevicePtr device) {

    pd_ = std::shared_ptr<ibv_pd>(ibv_alloc_pd(device->get_context()));
    if (!pd_.get()) {
        perror("could not allocated protection domain.");
        exit(EXIT_FAILURE);      
    }

    DEBUG_MSG("RoCE protection domain created.");

    
}

ProtectionDomainPtr create_protection_domain(RoceDevicePtr device) {
    return std::make_shared<ProtectionDomain>(device);
}

IbvProtectionDomainPtr ProtectionDomain::get_ibv_pd() {
    return pd_;
}


} // namespace Roce
} // namespace Network
