#include "verbs_common.h"

namespace Network {
namespace Roce {
        
ProtectionDomain::ProtectionDomain(RoceDevicePtr device) {

}

ProtectionDomainPtr create_protection_domain(RoceDevicePtr device) {
    return std::make_shared<ProtectionDomain>(device);
}


} // namespace Roce
} // namespace Network
