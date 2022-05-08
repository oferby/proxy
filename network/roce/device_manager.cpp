#include "verbs_common.h"

namespace Network {
namespace Roce {


RoceDevicePtr RoceDeviceManager::create_or_get_device(std::string dev_name) {

    auto it = device_map.find(dev_name);

    if (it == device_map.end()) {
        RoceDevicePtr device =  std::make_shared<RoceDevice>(dev_name);
        device_map[dev_name] = device;
        return device;
    }

    return it->second;
}

} // Roce
} // Network