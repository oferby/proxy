#include "verbs_common.h"

namespace Network {
namespace Roce {
    
MemoryManager::MemoryManager(AppContextPtr app_ctx) : app_ctx_(app_ctx) {

    mr_ = create_memory_region(app_ctx_, 20);

}


int MemoryManager::register_memory_block() {
    DEBUG_MSG("registering memory block");

    std::vector<ScatterGatherElementPtr> sge_vector =  mr_->get_available_sge(10);

    for (int i = 0; i < sge_vector.size(); i++ ) {

        auto sge = sge_vector.back();
        sge_vector.pop_back();

        ibv_recv_wr rec_wr = {};
        rec_wr.wr_id = reinterpret_cast<uint64_t>(sge->get().get());
        rec_wr.sg_list = sge->get().get();
        rec_wr.num_sge = 1;

        ibv_recv_wr *bad_wr;

        if (ibv_post_recv(app_ctx_->get_qp()->get_ibv_qp().get(), &rec_wr, &bad_wr)) {
            perror("error posting RR.");
            exit(EXIT_FAILURE);    
        }  

    }

    DEBUG_MSG("memory block registered.");

    return 1;

};

ScatterGatherElementPtr MemoryManager::get_available_sge() {

    return mr_->get_available_sge();

};


MemoryManagerPtr create_memory_manager(AppContextPtr app_ctx) {
    return std::make_shared<MemoryManager>(app_ctx);
};


} // namespace Roce
} // namespace Network
