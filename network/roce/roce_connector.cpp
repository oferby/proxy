#include <errno.h>
#include "verbs_common.h"
#include <malloc.h>

#define SGE_MSG "SEND operation "
#define SGE_MSG_SIZE (strlen(SGE_MSG) + 1)

namespace Network {
namespace Roce {

RoceConnector::RoceConnector(std::string dev_name) {

    RoceDevicePtr device = create_roce_device(dev_name);
    
    ProtectionDomainPtr pd =  create_protection_domain(device);

    CompletionQueuePtr cq = create_completion_queue(device);

    // SharedReceiveQueuePtr srq = create_srq(pd);

    app_ctx_ = create_app_context(device, pd, cq, nullptr);

    QueuePairPtr qp = create_queue_pair(app_ctx_);

    app_ctx_->set_qp(qp);

    memory_manager_ = create_memory_manager(app_ctx_);

};

RoceConnectorPtr create_roce_connector(std::string dev_name) {
    return std::make_shared<RoceConnector>(dev_name);
};

QueuePairInfoPtr RoceConnector::get_qp_info() {
    return app_ctx_->get_qp()->get_qp_info();
};

BufferPtr RoceConnector::get_qp_info_msg() {

    DEBUG_MSG("got request for QP info.");
    
    auto qp_info =  app_ctx_->get_qp()->get_qp_info();

    cm_con_data_t* local_con_data = new cm_con_data_t;

    /* exchange using TCP sockets info required to connect QPs */
    
    // local_con_data.addr = htonll((uintptr_t)res->buf);   TODO for READ / WRITE
    // local_con_data.rkey = htonl(res->mr->rkey);

    local_con_data->rkey = htonl(memory_manager_->get_memory_region()->get_lkey());
    local_con_data->qp_num = htonl(qp_info->qpn);
    local_con_data->lid = htons(qp_info->lid);
    memcpy(local_con_data->gid, qp_info->gid, 16);
    
    fprintf(stdout, "\nLocal LID = 0x%x\n", qp_info->lid);
    fprintf(stdout, "Local rkey = 0x%x\n", local_con_data->rkey);
    fprintf(stdout, "Local QP number = 0x%x\n", local_con_data->qp_num);
    uint8_t *pl = local_con_data->gid;
    fprintf(stdout, "Local GID = %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n", 
        pl[0], pl[1], pl[2], pl[3], pl[4], pl[5], pl[6], pl[7], pl[8], pl[9], pl[10], pl[11], pl[12], pl[13], pl[14], pl[15]);  
       

    BufferPtr hello_msg = create_buffer(sizeof(struct cm_con_data_t));
    memcpy(hello_msg->message,local_con_data, sizeof(struct cm_con_data_t));



    return hello_msg;
}

void RoceConnector::set_pair_qp_info(BufferPtr msg) {

    QueuePairInfoPtr remote_qp_info = std::make_shared<QueuePairInfo>();

    cm_con_data_t* tmp_con_data = (cm_con_data_t*) msg->message;
    cm_con_data_t remote_con_data;
    
    // tmp_con_data.addr = ntohll(tmp_con_data.addr);
    remote_con_data.rkey = ntohl(tmp_con_data->rkey);
    remote_con_data.qp_num = ntohl(tmp_con_data->qp_num);
    remote_con_data.lid = ntohs(tmp_con_data->lid);
    memcpy(remote_con_data.gid, tmp_con_data->gid, 16);

    // fprintf(stdout, "Remote address = 0x%"PRIx64"\n", remote_con_data.addr);
    fprintf(stdout, "Remote rkey = 0x%x\n", remote_con_data.rkey);
    fprintf(stdout, "Remote QP number = 0x%x\n", remote_con_data.qp_num);
    fprintf(stdout, "Remote LID = 0x%x\n", remote_con_data.lid);

    uint8_t *p = remote_con_data.gid;
    fprintf(stdout, "Remote GID = %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x\n", 
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);

    remote_qp_info->gid = new ibv_gid;
    memcpy(remote_qp_info->gid, remote_con_data.gid,16);
    remote_qp_info->lid = remote_con_data.lid;
    remote_qp_info->qpn = remote_con_data.qp_num;

    app_ctx_->get_qp()->set_remote_qp_info(remote_qp_info);

    DEBUG_MSG("QP info set");





    // memory_manager_->register_memory_block();

    char* reg_buf = memory_manager_->get_memory_region()->get_registered_buffer();

    ibv_sge ib_sge;
    ib_sge.addr = (uintptr_t) ( reg_buf + SGE_MSG_SIZE );
    ib_sge.length = SGE_MSG_SIZE;
    ib_sge.lkey = memory_manager_->get_memory_region()->get_lkey();

    ibv_recv_wr rr;
    ibv_recv_wr *bad_wr;
    int rc;
    
    /* prepare the receive work request */
    memset(&rr, 0, sizeof(rr));
    rr.next = NULL;
    rr.wr_id = ib_sge.addr;
    rr.sg_list = &ib_sge;
    rr.num_sge = 1;

    /* post the Receive Request to the RQ */
    rc = ibv_post_recv(app_ctx_->get_qp()->get_ibv_qp().get(), &rr, &bad_wr);
    if (rc)
        fprintf(stderr, "failed to post RR\n");
    else
        fprintf(stdout, "Receive Request was posted\n");


    DEBUG_MSG("starting polling thread.");
    
    int status = pthread_create(&polling_thread, nullptr, 
    [](void* arg) -> void* {
        static_cast<RoceConnector*>(arg)->poll_complition();
        return nullptr;
    }, 
    this);
    
    if (status != 0) {
        perror("error starting thread");
    }

};

void RoceConnector::send(BufferPtr buf_, uint32_t id) {
    
    char* buf = (char*) malloc(SGE_MSG_SIZE);
    strcpy(buf, SGE_MSG);





    struct ibv_send_wr sr;
    struct ibv_send_wr *bad_wr = NULL;
    int rc;

    
    char* reg_buf = memory_manager_->get_memory_region()->get_registered_buffer();

    ibv_sge ib_sge;
    ib_sge.addr = (uintptr_t) ( reg_buf + SGE_MSG_SIZE );
    ib_sge.length = SGE_MSG_SIZE;
    ib_sge.lkey = memory_manager_->get_memory_region()->get_lkey();

    /* prepare the send work request */
    memset(&sr, 0, sizeof(sr));
    sr.next = NULL;
    sr.wr_id = ib_sge.addr;
    sr.sg_list = &ib_sge;
    sr.num_sge = 1;
    sr.opcode = IBV_WR_SEND;
    sr.send_flags = IBV_SEND_SIGNALED;

    /* there is a Receive Request in the responder side, so we won't get any into RNR flow */
    rc = ibv_post_send(app_ctx_->get_qp()->get_ibv_qp().get(), &sr, &bad_wr);
    if (rc) {
        fprintf(stderr, "failed to post SR\n");
        exit(EXIT_FAILURE);
    }
        
    DEBUG_MSG("SR posted.");


}

void RoceConnector::poll_complition() {

    auto wc = std::make_shared<ibv_wc>(); 

    while (1)
    {

        int status = ibv_poll_cq(app_ctx_->get_cq()->get_cq().get(), 1, wc.get());
        if (status < 0) {
            perror("error getting WC.");
            break;;
        }

        if (status > 0) {
            DEBUG_MSG("got new WC event.");
            if(wc->status == ibv_wc_status::IBV_WC_SUCCESS) {
                printf("wid: " PRId64 ", opcode: %i\n", wc->wr_id, wc->opcode);
                handle_wc(wc);
                break;
            } else {
                DEBUG_MSG("got error processing WC.");
                break;
            }

        }

    }

}

void RoceConnector::handle_wc(std::shared_ptr<ibv_wc> wc) {

    DEBUG_MSG("handling WC.");

    switch (wc->opcode) {

        case IBV_WC_SEND :
            DEBUG_MSG("got SEND completion");
            // handle_sr();
            break;

        case IBV_WC_RECV :
            DEBUG_MSG("got RECV completion");
            // handle_rr();
            break;

        default : 
            DEBUG_MSG("got wrong WC opcode");
            break;

    }


}

} // namespace Network
} // namespace Roce
