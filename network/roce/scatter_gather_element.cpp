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

uint64_t ScatterGatherElement::get_addr() { return sge_->addr; };
uint32_t ScatterGatherElement::get_length() { return sge_->length; }; 
uint32_t ScatterGatherElement::get_lkey() { return sge_->lkey; };
void ScatterGatherElement::set_lenght(uint32_t lenght) { sge_->length = lenght; }; 
void ScatterGatherElement::set_addr(uint64_t addr) { sge_->addr = addr; }


ScatterGatherElementPtr create_sge(uint64_t	addr, uint32_t length, uint32_t lkey) {
    return std::make_shared<ScatterGatherElement>(addr, length, lkey);
}

} // namespace Roce
} // namespace Network
