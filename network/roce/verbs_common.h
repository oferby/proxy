#ifndef VERBS_COMMON
#define VERBS_COMMON

#include "../../common.h"
#include <infiniband/verbs.h>

#define PORT_NUM 1
#define QKEY 0x11111111
#define GID_IDX 1
#define IB_PORT 1
#define MSG_SIZE 1500
#define GRH_SIZE 40
#define CQ_SIZE 10
#define MAX_WR 10
#define MAX_SGE 1

namespace Network {
namespace Roce {

class RoceDevice {
private:
    std::string name_;
    ibv_port_attr port_info;
    ibv_context* ctx_;

public:
    RoceDevice(std::string dev_name);
    std::string get_device_name();
    ibv_context* get_context();

};

using RoceDevicePtr = std::shared_ptr<RoceDevice>;
RoceDevicePtr create_roce_device(std::string dev_name);

class ProtectionDomain {
private:
    ibv_pd pd_;
public:
    ProtectionDomain(RoceDevicePtr device);

};

using ProtectionDomainPtr = std::shared_ptr<ProtectionDomain>;
ProtectionDomainPtr create_protection_domain(RoceDevicePtr device);

class CompletionQueue {
private:    
    ibv_cq cq_;
public:
    CompletionQueue(RoceDevicePtr device);
    ibv_cq get_cq();
};

using CompletionQueuePtr = std::shared_ptr<CompletionQueue>;
CompletionQueuePtr create_completion_queue(RoceDevicePtr device);

class QueuePair {
private:
    ibv_qp qp_;
public:
    QueuePair(RoceDevicePtr device);
};

using QueuePairPtr = std::shared_ptr<QueuePair>;
QueuePairPtr create_queue_pair(RoceDevicePtr device);


class AppContext {
private:
    RoceDevicePtr device_;
    ProtectionDomainPtr pd_;
    CompletionQueuePtr cq_;
    QueuePairPtr qp_;
    
public:
    AppContext();
    void set_device(RoceDevicePtr device);
    void set_protection_domain(ProtectionDomainPtr pd);
    void set_completion_queue(CompletionQueuePtr cq);
    void set_queue_pair(QueuePairPtr qp);

};

using AppContextPtr = std::shared_ptr<AppContext>;
AppContextPtr create_app_context();



class RoceListener : public Network::Listener {
private:
    AppContextPtr app_ctx_;

public:
    RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket();

};

using RoceListenerPtr = std::shared_ptr<RoceListener>;
RoceListenerPtr create_roce_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);


} // namespace Roce
} // namespace Network

#endif