#ifndef VERBS_COMMON
#define VERBS_COMMON

#include "../../common.h"
#include <infiniband/verbs.h>

#define PORT_NUM 1
#define QKEY 0x11111111
#define GID_IDX 3
#define IB_PORT 1
#define GRH_SIZE 40
#define MSG_SIZE (2048 - GRH_SIZE)
#define CQ_SIZE 10
#define MAX_WR 10
#define MAX_SGE 5
#define HELLO_MSG_SIZE (sizeof "0000:000000:000000:00000000000000000000000000000000")

#define ROCE_COMMUNICATION_MGR_PORT 9000

namespace Network {
namespace Roce {

/* structure to exchange data which is needed to connect the QPs */
struct cm_con_data_t
{
    // uint64_t addr; /* Buffer address */
    uint32_t rkey; /* Remote key */
    uint32_t qp_num; /* QP number */
    uint16_t lid; /* LID of the IB port */
    uint8_t gid[16]; /* gid */
} __attribute__ ((packed));

// using CmConDataPtr = std::shared_ptr<cm_con_data_t>;

struct QueuePairInfo {
	uint16_t lid;
	uint32_t qpn;
	int psn;
	union ibv_gid *gid;
	ibv_ah *ah;
};
using QueuePairInfoPtr = std::shared_ptr<QueuePairInfo>;

using IbvContextPtr = ibv_context*;
using IbvProtectionDomainPtr = std::shared_ptr<ibv_pd>;
using IbvCompletionQueuePtr = std::shared_ptr<ibv_cq>;
using IbvSharedReceiveQueuePtr = std::shared_ptr<ibv_srq>;
using IbvScatterGatherElementPtr = std::shared_ptr<ibv_sge>;
using IbvMemoryRegionPtr = std::shared_ptr<ibv_mr>;
using IbvSgePtr = std::shared_ptr<ibv_sge>;
using IbvQp = std::shared_ptr<ibv_qp>;

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



class ProtectionDomain {
private:
    IbvProtectionDomainPtr pd_;
public:
    ProtectionDomain(RoceDevicePtr device);
    IbvProtectionDomainPtr get_ibv_pd();
};

using ProtectionDomainPtr = std::shared_ptr<ProtectionDomain>;
ProtectionDomainPtr create_protection_domain(RoceDevicePtr device);



class CompletionQueue {
private:    
    IbvCompletionQueuePtr cq_;
public:
    CompletionQueue(RoceDevicePtr device);
    IbvCompletionQueuePtr get_cq();
};

using CompletionQueuePtr = std::shared_ptr<CompletionQueue>;
CompletionQueuePtr create_completion_queue(RoceDevicePtr device);


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
    IbvQp qp_;
    AppContextPtr ctx_;
public:
    QueuePair(AppContextPtr ctx);
    QueuePairInfoPtr get_qp_info();
    void set_remote_qp_info(QueuePairInfoPtr qp_info);
    IbvQp get_ibv_qp();
    
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


class ScatterGatherElement {
private:    
    IbvSgePtr sge_;
public:
    ScatterGatherElement(uint64_t addr, uint32_t length, uint32_t lkey);
    IbvSgePtr get();
    uint64_t get_addr();
    uint32_t get_length(); 
    uint32_t get_lkey();
    void set_lenght(uint32_t lenght);
    void set_addr(uint64_t addr);
};
using ScatterGatherElementPtr = std::shared_ptr<ScatterGatherElement>;
ScatterGatherElementPtr create_sge(uint64_t	addr, uint32_t length, uint32_t lkey);

class MemoryRegion {
private:
    const size_t size_;
    char* buf_;
    std::vector<ScatterGatherElementPtr> available_sge_vector;
    std::map<uint64_t, ScatterGatherElementPtr> all_sge_map;
    IbvMemoryRegionPtr mr_;
    AppContextPtr app_ctx_;
public:
    MemoryRegion(AppContextPtr app_ctx, int num_of_sge);
    std::vector<ScatterGatherElementPtr> get_available_sge(int num);
    std::vector<ScatterGatherElementPtr> get_all_available_sge();
    void make_available(ScatterGatherElementPtr sge);
    ScatterGatherElementPtr get_available_sge();
    ScatterGatherElementPtr get_sge(uint64_t addr);
    uint32_t get_lkey();
    char* get_registered_buffer();
};
using MemoryRegionPtr = std::shared_ptr<MemoryRegion>;
MemoryRegionPtr create_memory_region(AppContextPtr app_ctx, int num_of_sge);


class MemoryManager {
private:
    AppContextPtr app_ctx_;
    MemoryRegionPtr mr_;
public:
    MemoryManager(AppContextPtr app_ctx);
    int register_memory_block();
    ScatterGatherElementPtr get_available_sge();
    ScatterGatherElementPtr get_sge(uint64_t addr);
    MemoryRegionPtr get_memory_region();
};

using MemoryManagerPtr = std::shared_ptr<MemoryManager>;
MemoryManagerPtr create_memory_manager(AppContextPtr app_ctx);



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
    MemoryManagerPtr memory_manager_;
    pthread_t polling_thread;

    void poll_complition();
    void handle_wc(std::shared_ptr<ibv_wc> wc);
    void handle_sr(std::shared_ptr<ibv_wc> wc);
    void handle_rr(std::shared_ptr<ibv_wc> wc);


public:
    RoceConnector(std::string dev_name);
    QueuePairInfoPtr get_qp_info();
    BufferPtr get_qp_info_msg();
    void set_pair_qp_info(BufferPtr msg);
    void send(BufferPtr buf, uint32_t id);

};
using RoceConnectorPtr = std::shared_ptr<RoceConnector>;
RoceConnectorPtr create_roce_connector(std::string dev_name);



class RoceVirtualConnection : public Network::Connection::ConnectionBase {
private:
    uint32_t id_;
    RoceConnectorPtr roce_connector_;
public:
    RoceVirtualConnection(uint32_t id, RoceConnectorPtr roce_connector);
    int get_sock() override;
    void on_read() override;
    void on_write(BufferPtr buf) override;
    void set_connection_pair(Network::Connection::ConnectionBasePtr connection_pair) override;
    void close() override;
    
};

using RoceVirtualConnectionPtr = std::shared_ptr<RoceVirtualConnection>;
RoceVirtualConnectionPtr create_roce_connection(uint32_t id, RoceConnectorPtr roce_connector);







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
    uint32_t next_connection_id_ = 0;
    std::map<uint32_t, RoceVirtualConnectionPtr> roce_connection_map;
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