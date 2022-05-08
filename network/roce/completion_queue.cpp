#include "verbs_common.h"

namespace Network {
namespace Roce {

CompletionQueue::CompletionQueue(RoceDevicePtr device) {

    ibv_cq* cq = ibv_create_cq(device->get_context(), CQ_SIZE, nullptr, nullptr, 0);

    if (!cq) {
        perror("could not create CQ\n");
        exit(EXIT_FAILURE);
    }
        
    cq_ = std::shared_ptr<ibv_cq>(cq);

    DEBUG_MSG("RoCE completion queue created.");
}

CompletionQueuePtr create_completion_queue(RoceDevicePtr device) {
    return std::make_shared<CompletionQueue>(device);
};

IbvCompletionQueuePtr CompletionQueue::get_cq() {
    return cq_;
};

} // namespace Roce
} // namespace Network
