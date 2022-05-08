#include "verbs_common.h"

namespace Network {
namespace Roce {

SharedReceiveQueue::SharedReceiveQueue(ProtectionDomainPtr pd) {

    ibv_srq_init_attr attr = {
        .attr = {
            .max_wr = MAX_WR,
            .max_sge = MAX_SGE
        }
    };

    srq_ = std::shared_ptr<ibv_srq>(ibv_create_srq(pd->get_ibv_pd().get(), &attr));
    

}

SharedReceiveQueuePtr create_srq(ProtectionDomainPtr pd) {
    return std::make_shared<SharedReceiveQueue>(pd);
}

IbvSharedReceiveQueuePtr SharedReceiveQueue::get_srq() {
    return srq_;
}


} // namespace Roce
} // namespace Network
