#include "verbs_common.h"

namespace Network {
namespace Roce {
    

ScatterGatherElement::ScatterGatherElement(uint64_t	addr, uint32_t length, uint32_t lkey) : 
    addr_(addr), length_(length), lkey_(lkey)  {}

uint64_t ScatterGatherElement::get_length() {
    return length_;
};

uint64_t ScatterGatherElement::get_addr() {
    return addr_;
};

uint64_t ScatterGatherElement::get_lkey() {
    return lkey_;
};



ScatterGatherElementPtr create_sge(uint64_t	addr, uint32_t length, uint32_t lkey) {
    return std::make_shared<ScatterGatherElement>(addr, length, lkey);
}

} // namespace Roce
} // namespace Network
