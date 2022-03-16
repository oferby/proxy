#include "verbs_common.h"

namespace Network {
namespace Roce {

QueuePair::QueuePair(RoceDevicePtr device) {

};

QueuePairPtr create_queue_pair(RoceDevicePtr device) {
    return std::make_shared<QueuePair>(device);
};

} // namespace Roce

} // namespace Network
