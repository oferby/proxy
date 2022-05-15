#include <errno.h>
#include "verbs_common.h"

namespace Network {
namespace Roce {
    
MemoryManager::MemoryManager(AppContextPtr app_ctx) : app_ctx_(app_ctx) {

    mr_ = create_memory_region(app_ctx_, NUM_OF_TOTAL_SGE);

}

MemoryRegionPtr MemoryManager::get_memory_region() {
    return mr_;
}

ScatterGatherElementPtr MemoryManager::get_sge(uint64_t addr) {
    return mr_->get_sge(addr);
}

int MemoryManager::post_receive() {

    for (int i = 0; i < (NUM_OF_TOTAL_SGE / 2); i++ ) {
        
        auto sge = mr_->get_available_sge();

        ibv_sge* ib_sge =  new ibv_sge;
        ib_sge->addr = sge->get_addr(),
        ib_sge->length = sge->get_length(),
        ib_sge->lkey = sge->get_lkey();

        ibv_recv_wr rec_wr = {};
        rec_wr.wr_id = reinterpret_cast<uint64_t>(sge->get_addr());
        rec_wr.sg_list = ib_sge;
        rec_wr.num_sge = 1;

        ibv_recv_wr* bad_wr;

        ibv_qp* qp = app_ctx_->get_qp()->get_ibv_qp().get();
        
        errno = 0;
        int ret;
        if (ret = ibv_post_recv(qp, &rec_wr, &bad_wr)) {
            perror("error posting RR.");
            exit(EXIT_FAILURE);    
        }  

    }

    DEBUG_MSG("post_recv done.");

    return 1;

};

ScatterGatherElementPtr MemoryManager::get_available_sge() {
    // DEBUG_MSG("Roce get_available_sge");
    return mr_->get_available_sge();

};

void MemoryManager::make_available(uint64_t addr) {
    // DEBUG_MSG("Roce make_available");
    mr_->make_available(addr);
}


MemoryManagerPtr create_memory_manager(AppContextPtr app_ctx) {
    return std::make_shared<MemoryManager>(app_ctx);
};


} // namespace Roce
} // namespace Network
