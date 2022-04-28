#include <errno.h>
#include "verbs_common.h"
#include <malloc.h>
#include <inttypes.h>

#define __STDC_FORMAT_MACROS

// #define SGE_MSG "SEND operation "
// #define SGE_MSG_SIZE (strlen(SGE_MSG) + 1)

namespace Network {
namespace Roce {

/*
*
*   Handles setup between 2 QPs.
*   Build and destroy RoCE connection per TCP session.
*
*/

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

    for (int i = 0; i < 10; ++i) {
        post_recv(memory_manager_->get_available_sge());
    }

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

void RoceConnector::post_recv(ScatterGatherElementPtr sge) {

        ibv_sge* ib_sge = sge->get().get();

        // printf("POSTING: SGE addr: %lu, Data addr: %lu, Length: %u, LKey: %u\n", reinterpret_cast<uint64_t>(ib_sge), ib_sge->addr, ib_sge->length, ib_sge->lkey);

        ibv_recv_wr rr;
        ibv_recv_wr *bad_wr;
        int rc;

        /* prepare the receive work request */
        memset(&rr, 0, sizeof(rr));
        rr.next = NULL;
        rr.wr_id = ib_sge->addr;
        rr.sg_list = ib_sge;
        rr.num_sge = 1;

        rc = ibv_post_recv(app_ctx_->get_qp()->get_ibv_qp().get(), &rr, &bad_wr);
        if (rc)
            fprintf(stderr, "failed to post RR\n");
        // else
            // DEBUG_MSG("Receive Request was posted\n");

}

void RoceConnector::send(BufferPtr buf_, uint32_t id) {
    
    printf("sending Roce with id: %u\n", id);



    ScatterGatherElementPtr sge;
    do {
        sge = memory_manager_->get_available_sge();
    } while ( sge == nullptr);

    ibv_sge* ib_sge = sge->get().get();
    auto p = reinterpret_cast<void*>(ib_sge->addr);
    memcpy(p, buf_->message, buf_->lenght);
    ib_sge->length = buf_->lenght;

    /* prepare the send work request */
    ibv_send_wr sr {0};
    ibv_send_wr *bad_wr = NULL;
    int rc;

    // memset(&sr, 0, sizeof(sr));
    sr.next = NULL;
    sr.wr_id = ib_sge->addr;
    sr.sg_list = ib_sge;
    sr.num_sge = 1;
    sr.opcode = IBV_WR_SEND_WITH_IMM;
    sr.send_flags = IBV_SEND_SIGNALED;
    
    // for remote
    sr.imm_data   = htonl(id);
    // for local SR
    connection_id_map_[ib_sge->addr] = id;

    rc = ibv_post_send(app_ctx_->get_qp()->get_ibv_qp().get(), &sr, &bad_wr);
    if (rc) {
        fprintf(stderr, "failed to post SR\n");
        exit(EXIT_FAILURE);
    }
        
    DEBUG_MSG("SR posted.");


}

void RoceConnector::poll_complition() {

    ibv_wc wc;

    while (1)
    {
        
        int status;
        do {
            status = ibv_poll_cq(app_ctx_->get_cq()->get_cq().get(), 1, &wc);
        } while (status == 0);

        if (status < 0) {
            perror("error getting WC.");
            continue;
        }

        if (status > 0) {
            // DEBUG_MSG("got new WC event.");
            if(wc.status == ibv_wc_status::IBV_WC_SUCCESS) {
                handle_wc(&wc);
            } else {
                DEBUG_MSG("got error processing WC.");
            }

        }

    }

}

void RoceConnector::handle_wc(ibv_wc* wc) {

    // DEBUG_MSG("handling WC.");

    switch (wc->opcode) {

        case IBV_WC_SEND :
            DEBUG_MSG("got SEND completion");
            handle_sr(wc);
            break;

        case IBV_WC_RECV :
            DEBUG_MSG("got RECV completion");
            if (wc->imm_data == 0) 
                handle_control(wc);
            else 
                handle_rr(wc);
            break;

        default : 
            DEBUG_MSG("got wrong WC opcode");
            break;

    }


}

BufferPtr RoceConnector::get_data(ibv_wc* wc) {

    ScatterGatherElementPtr sge_ptr = memory_manager_->get_sge(wc->wr_id);

    ibv_sge *sge = sge_ptr->get().get();

    BufferPtr input_buf = create_buffer(wc->byte_len);

    char *p = reinterpret_cast<char*>(sge->addr);
    memcpy(input_buf->message, p, wc->byte_len);

    return input_buf;

}

void RoceConnector::handle_rr(ibv_wc* wc) {
    
    DEBUG_MSG("handling RR");

    BufferPtr input_buf = get_data(wc);

    auto id = ntohl(wc->imm_data);

    auto it = roce_connection_map.find(id);
    if (it != roce_connection_map.end()) {
        // DEBUG_MSG("using existing connection");
        printf("using existing connection id: %u\n", id);
        auto roce_connection = it->second;
        roce_connection->on_read(input_buf);

    } else {

        RoceVirtualConnectionPtr roce_connection = connect(id);
        roce_connection->on_read(input_buf);
    }
         
    
    ScatterGatherElementPtr sge = memory_manager_->get_sge(wc->wr_id);
    post_recv(sge);

}

void RoceConnector::handle_sr(ibv_wc* wc) {

    uint32_t id;

    auto it = connection_id_map_.find(wc->wr_id);
    
    if (it != connection_id_map_.end()) {
        id = it->second;
    } else {
        puts("error: could not find connection id");
    }

    printf("handle_sr() for RoCE connection id %u\n", id);
    
    memory_manager_->make_available(wc->wr_id);

    auto it2 = roce_connection_map.find(id);
    if (it2 != roce_connection_map.end()) {
        auto roce_connection = it2->second;
        roce_connection->on_write_complete();

    } else {
        DEBUG_MSG("could not find RoCE connection. Probably closed.");
        
    }

}

//server side close
void RoceConnector::handle_control(ibv_wc* wc) {
    
    DEBUG_MSG("handling Control");

    // TODO: create better message struct
    // for now we are only getting connection id info

    BufferPtr input_buf = get_data(wc);

    uint32_t* tmp_id = (uint32_t*) input_buf->message;
    uint32_t id_to_close = ntohl(*tmp_id);

    printf("request to close connection id %u\n", id_to_close);

    auto it = roce_connection_map.find(id_to_close);

    if (it != roce_connection_map.end()) {
        it->second->on_close();
    } else {
        DEBUG_MSG("RoCE connection already closed.");
    }

    ScatterGatherElementPtr sge = memory_manager_->get_sge(wc->wr_id);
    post_recv(sge);
    
}

// listener side
RoceVirtualConnectionPtr RoceConnector::connect(uint32_t id) {

    DEBUG_MSG("creating new Roce Connection on listener side.");

    RoceVirtualConnectionPtr roce_connection = create_roce_connection(id, shared_from_this());    

    roce_connection_map[id] = roce_connection;

    auto connection_pair = client_->connect();
    roce_connection->set_connection_pair(connection_pair);

    connection_pair->set_connection_pair(roce_connection);


    return roce_connection;

}

// client side
Network::Connection::ConnectionBasePtr RoceConnector::connect() {
    
    DEBUG_MSG("creating new Roce Connection on client side");

    RoceVirtualConnectionPtr roce_connection = create_roce_connection(++next_connection_id_, shared_from_this());    

    roce_connection_map[next_connection_id_] = roce_connection;

    printf("new RoCE connection added with id %u\n", next_connection_id_);

    return roce_connection; 
}

// client side close
void RoceConnector::close(uint32_t id) {

    DEBUG_MSG("RoceConnector close()");

    BufferPtr buf = create_buffer(sizeof(uint32_t));
    auto id_ = htonl(id);
    memcpy(buf->message, &id_, sizeof(uint32_t));

    send(buf, 0);

    roce_connection_map.erase(id);
}

// listener side
void RoceConnector::on_close(uint32_t id) {
    DEBUG_MSG("RoceConnector on_close()");
    roce_connection_map.erase(id);
}

void RoceConnector::set_client_side(Network::ClientBasePtr client) {
    client_ = client;
}


} // namespace Network
} // namespace Roce
