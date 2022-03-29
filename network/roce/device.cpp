#include "verbs_common.h"

namespace Network {
namespace Roce {


RoceDevice::RoceDevice(std::string dev_name) :name_(dev_name) {
    
    DEBUG_MSG("creating RoCE device");

    ibv_device   **dev_list, **tmp_dev_list;
    ibv_device   *ib_dev;
    int status;


    dev_list = ibv_get_device_list(NULL);
    
    if (!dev_list) {
        perror("error getting IB device list");
        exit(EXIT_FAILURE);
    }
    
    ib_dev = *dev_list;
    if (!ib_dev) {
        perror("device list empty");
        exit(EXIT_FAILURE);
    }
    
    tmp_dev_list = dev_list;
    bool found = false;
    while (tmp_dev_list)
    {
        ib_dev = *tmp_dev_list;
        if (!ib_dev) {
            break;
        }
        if (strcmp(ib_dev->name, name_.c_str()) == 0) {
            found = true;
            break;
        }
        tmp_dev_list++;
    }

    if (!found){
        printf("could not find dev %s\n", name_.c_str());
        exit(EXIT_FAILURE);
    }
    
    printf("using dev name: %s\n", ib_dev->name);

    ctx_ = ibv_open_device(ib_dev);
    if (!ctx_) {
        perror("error creating context");
        exit(EXIT_FAILURE);
    }

    ibv_free_device_list(dev_list);

    port_info_ = {};
    status = ibv_query_port(ctx_, IB_PORT, &port_info_);
    if (status == -1) {
        perror("could not get port info");
        exit(EXIT_FAILURE);
    }

}        

std::string RoceDevice::get_device_name() { return name_; };
IbvContextPtr RoceDevice::get_context() { return ctx_; };
uint16_t RoceDevice::get_lid() { return port_info_.lid; };

RoceDevicePtr create_roce_device(std::string dev_name) {
    return std::make_shared<RoceDevice>(dev_name);
}

} // namespace Roce    
} // namespace Network


