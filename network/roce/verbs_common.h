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
#define HELLO_MSG_SIZE (sizeof "0000:000000:000000:00000000000000000000000000000000")

#define ROCE_COMMUNICATION_MGR_PORT 9000

namespace Network {
namespace Roce {

struct QueuePairInfo {
	uint16_t lid;
	uint32_t qpn;
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
    ibv_gid gid_ {0};
    IbvContextPtr ctx_;

public:
    RoceDevice(std::string dev_name);
    std::string get_device_name();
    IbvContextPtr get_context();
    uint16_t get_lid();
    ibv_gid* get_gid();

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
    SocketBasePtr accept();
    BufferPtr recv();
    void send(BufferPtr buf);
    void close();

};
using RoceVirtualSocketPtr = std::shared_ptr<RoceVirtualSocket>;
RoceVirtualSocketPtr create_roce_socket(ConnectionManagerBasePtr connection_manager);


class RoceConnector {
private:
    AppContextPtr app_ctx_;

public:
    RoceConnector(std::string dev_name);
    QueuePairInfoPtr get_qp_info();
    BufferPtr get_qp_info_msg();
    void set_pair_qp_info(BufferPtr msg);

};
using RoceConnectorPtr = std::shared_ptr<RoceConnector>;
RoceConnectorPtr create_roce_connector(std::string dev_name);

class RoceListener : public Network::Listener {
private:
    RoceConnectorPtr roce_connector_;
    void setup_comm_server();
    Network::SocketBasePtr tcp_sd_;
public:
    RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::SocketBasePtr get_socket();
    void set_client_side(Network::ClientBasePtr client);
    void on_connect();

};

using RoceListenerPtr = std::shared_ptr<RoceListener>;
RoceListenerPtr create_roce_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher);

class RoceClient : public Network::ClientBase {
private:
    RoceConnectorPtr roce_connector_;
    void setup_pair_connection();
public:
    RoceClient(Network::addr_info info, Event::DispatcherBasePtr dispatcher);
    Network::Connection::ConnectionBasePtr connect();

};

using RoceClientPtr = std::shared_ptr<RoceClient>;
RoceClientPtr create_roce_client(Network::addr_info info, Event::DispatcherBasePtr dispatcher);


class RoceConnectionManager {
private:
    Event::DispatcherBasePtr dispatcher_;
    std::map<std::string, RoceClientPtr> roce_clients_ {};
    RoceListenerPtr listener_ = nullptr;
public:
    RoceConnectionManager();
    void set_dispatcher(Event::DispatcherBasePtr dispatcher);
    RoceListenerPtr create_roce_listener(Network::addr_info info);
    RoceClientPtr create_roce_client(Network::addr_info info);
    

};

RoceConnectionManagerPtr create_roce_connection_manager();



static void wire_gid_to_gid(const char *wgid, union ibv_gid *gid) { 

	char tmp[9];
	__be32 v32;
	int i;
	uint32_t tmp_gid[4];

	for (tmp[8] = 0, i = 0; i < 4; ++i) {
		memcpy(tmp, wgid + i * 8, 8);
		sscanf(tmp, "%x", &v32);
		tmp_gid[i] = be32toh(v32);
	}
	memcpy(gid, tmp_gid, sizeof(*gid));

}

static void gid_to_wire_gid(const union ibv_gid *gid, char wgid[]) {

	uint32_t tmp_gid[4];
	int i;

	memcpy(tmp_gid, gid, sizeof(tmp_gid));
	for (i = 0; i < 4; ++i)
		sprintf(&wgid[i * 8], "%08x", htobe32(tmp_gid[i]));

}

} // namespace Roce
} // namespace Network

#endif