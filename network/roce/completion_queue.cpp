#include "verbs_common.h"

namespace Network {
namespace Roce {

CompletionQueue::CompletionQueue(RoceDevicePtr device) {

}

CompletionQueuePtr create_completion_queue(RoceDevicePtr device) {
    return std::make_shared<CompletionQueue>(device);
};

} // namespace Roce
} // namespace Network
