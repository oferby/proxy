#include "verbs_common.h"
#include <malloc.h>

namespace Network {
namespace Roce {


MemoryRegion::MemoryRegion(AppContextPtr app_ctx, int num_of_sge) : app_ctx_(app_ctx),  size_(( MSG_SIZE + GRH_SIZE ) * num_of_sge) {

    int alignment = sysconf(_SC_PAGESIZE);
    buf_ = (char*) memalign(alignment, size_);

    if (!buf_) {
        perror("error creating memory buffer.");
        exit(EXIT_FAILURE);
    };

    mr_ = std::shared_ptr<ibv_mr>(ibv_reg_mr(app_ctx_->get_pd()->get_ibv_pd().get(), buf_, size_, IBV_ACCESS_LOCAL_WRITE));
    if (!mr_) {
        perror("error registering memory");
        exit(EXIT_FAILURE);
    }


    uint32_t msg_size = MSG_SIZE + GRH_SIZE;

    uint64_t mem_addr = reinterpret_cast<uint64_t>(buf_);

    for (int i = 0; i < num_of_sge; i++) {

        ScatterGatherElementPtr sge = create_sge(mem_addr, MSG_SIZE + GRH_SIZE, mr_->lkey);

        available_sge_vector.push_back(sge);
        all_sge_map[mem_addr] = sge;

        mem_addr += msg_size;

    }


    DEBUG_MSG("memory registered.");


};

ScatterGatherElementPtr MemoryRegion::get_sge(uint64_t addr) {
    auto sge_entry = all_sge_map.find(addr);   
    return sge_entry->second;
}

uint32_t MemoryRegion::get_lkey() {
    return mr_->lkey;
}

char* MemoryRegion::get_registered_buffer() {
    return buf_;
}

std::vector<ScatterGatherElementPtr> MemoryRegion::get_available_sge(int num) {
    
    std::vector<ScatterGatherElementPtr> sge_vector {};

    if (num > available_sge_vector.size()) {
        num = available_sge_vector.size();
    };

    for (int i = 0; i < num; i++) {
        ScatterGatherElementPtr sge = available_sge_vector.back();
        available_sge_vector.pop_back();
        sge_vector.push_back(sge);
        
    }

    return sge_vector;

}

std::vector<ScatterGatherElementPtr> MemoryRegion::get_all_available_sge() {
    
    std::vector<ScatterGatherElementPtr> sge_vector {};

    for (int i = 0; i < available_sge_vector.size(); i++) {
        ScatterGatherElementPtr sge = available_sge_vector.back();
        available_sge_vector.pop_back();
        sge_vector.push_back(sge);
        
    }

    return sge_vector;

}

void MemoryRegion::make_available(ScatterGatherElementPtr sge) {
    available_sge_vector.push_back(sge);
}


ScatterGatherElementPtr MemoryRegion::get_available_sge() {

    ScatterGatherElementPtr sge = available_sge_vector.back();
    available_sge_vector.pop_back();

    return sge;

}



MemoryRegionPtr create_memory_region(AppContextPtr app_ctx, int num_of_sge) {
    return std::make_shared<MemoryRegion>(app_ctx, num_of_sge);
};

    
} // namespace Roce
    
} // namespace Network
