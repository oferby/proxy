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

struct QueuePairInfo {
	uint16_t lid;
	int qpn;
	int psn;
	union ibv_gid *gid;
	ibv_ah *ah;
};

using QueuePairInfoPtr = std::shared_ptr<QueuePairInfo>;

using IbvContextPtr = ibv_context*;

class AppContext;
using AppContextPtr = std::shared_ptr<AppContext>;

class RoceDevice {
private:
    std::string name_;
    ibv_port_attr port_info_;
    IbvContextPtr ctx_;

public:
    RoceDevice(std::string dev_name);
    std::string get_device_name();
    IbvContextPtr get_context();
    uint16_t get_lid();

};

using RoceDevicePtr = std::shared_ptr<RoceDevice>;
RoceDevicePtr create_roce_device(std::string dev_name);

using IbvProtectionDomainPtr = std::shared_ptr<ibv_pd>;

class ProtectionDomain {
private:
    IbvProtectionDomainPtr pd_;
public:
    ProtectionDomain(RoceDevicePtr device);
    IbvProtectionDomainPtr get_ibv_pd();
};

using ProtectionDomainPtr = std::shared_ptr<ProtectionDomain>;
ProtectionDomainPtr create_protection_domain(RoceDevicePtr device);

using IbvCompletionQueuePtr = std::shared_ptr<ibv_cq>;

class CompletionQueue {
private:    
    IbvCompletionQueuePtr cq_;
public:
    CompletionQueue(RoceDevicePtr device);
    IbvCompletionQueuePtr get_cq();
};

using CompletionQueuePtr = std::shared_ptr<CompletionQueue>;
CompletionQueuePtr create_completion_queue(RoceDevicePtr device);


using IbvSharedReceiveQueuePtr = std::shared_ptr<ibv_srq>;

class SharedReceiveQueue {
private:
    IbvSharedReceiveQueuePtr srq_;
public:
    SharedReceiveQueue(ProtectionDomainPtr pd);
    IbvSharedReceiveQueuePtr get_srq();

};

using SharedReceiveQueuePtr = std::shared_ptr<SharedReceiveQueue>;
SharedReceiveQueuePtr create_srq(ProtectionDomainPtr pd);


class QueuePair {
private:
    ibv_qp* qp_;
    AppContextPtr ctx_;
public:
    QueuePair(AppContextPtr ctx);
    QueuePairInfoPtr get_qp_info();
    void set_remote_qp_info(QueuePairInfoPtr qp_info);
};

using QueuePairPtr = std::shared_ptr<QueuePair>;
QueuePairPtr create_queue_pair(AppContextPtr ctx);


class AppContext {
private:
    RoceDevicePtr device_;
    ProtectionDomainPtr pd_;
    CompletionQueuePtr cq_;
    QueuePairPtr qp_;
    SharedReceiveQueuePtr srq_;
    
public:
    AppContext(RoceDevicePtr device, ProtectionDomainPtr pd, CompletionQueuePtr cq, SharedReceiveQueuePtr srq);
    RoceDevicePtr get_device();
    ProtectionDomainPtr get_pd();
    CompletionQueuePtr get_cq();
    QueuePairPtr get_qp();
    SharedReceiveQueuePtr get_srq();
    void set_qp(QueuePairPtr qp);
};

AppContextPtr create_app_context(RoceDevicePtr device, ProtectionDomainPtr pd, CompletionQueuePtr cq, SharedReceiveQueuePtr srq);


class RoceVirtualSocket : public SocketBase {

public:
    RoceVirtualSocket(ConnectionManagerBasePtr connection_manager);
    int connect(Network::addr_info info);
    int get();
    void on_connect();
    void set_client_side(Network::ClientBasePtr client);
    int bind(Network::addr_info info);
    int listen();
    // int accept();

};
using RoceVirtualSocketPtr = std::shared_ptr<RoceVirtualSocket>;
RoceVirtualSocketPtr create_roce_socket(ConnectionManagerBasePtr connection_manager);


class RoceConnector {
private:
    AppContextPtr app_ctx_;

public:
    RoceConnector(std::string dev_name);

};
using RoceConnectorPtr = std::shared_ptr<RoceConnector>;
RoceConnectorPtr create_roce_connector(std::string dev_name);

class RoceListener : public Network::Listener {
private:
    RoceConnectorPtr roce_connector_;
public:
    RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket();
    void set_client_side(Network::ClientBasePtr client);

};

using RoceListenerPtr = std::shared_ptr<RoceListener>;
RoceListenerPtr create_roce_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);

class RoceClient : public Network::ClientBase {
private:
    RoceConnectorPtr roce_connector_;
public:
    RoceClient(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr create_client_socket();
    Network::Connection::ConnectionBasePtr connect(Network::SocketBasePtr sd);

};

using RoceClientPtr = std::shared_ptr<RoceClient>;
RoceClientPtr create_roce_client(Network::addr_info info, Event::DispatcherBasePtr dispatcher);


} // namespace Roce
} // namespace Network

#endif