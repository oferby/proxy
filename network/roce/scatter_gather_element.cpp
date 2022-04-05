#include "verbs_common.h"

namespace Network {
namespace Roce {
    

ScatterGatherElement::ScatterGatherElement(uint64_t	addr, uint32_t length, uint32_t lkey) {
    
    sge_ = std::make_shared<ibv_sge>();

    sge_->addr = addr;
    sge_->length = length;
    sge_->lkey = lkey;

}

IbvSgePtr ScatterGatherElement::get() {
    return sge_;
}


ScatterGatherElementPtr create_sge(uint64_t	addr, uint32_t length, uint32_t lkey) {
    return std::make_shared<ScatterGatherElement>(addr, length, lkey);
}

} // namespace Roce
} // namespace Network
