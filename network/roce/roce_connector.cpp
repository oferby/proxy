#include "verbs_common.h"


namespace Network {
namespace Roce {

RoceConnector::RoceConnector(std::string dev_name) {

    RoceDevicePtr device = create_roce_device(dev_name);
    
    ProtectionDomainPtr pd =  create_protection_domain(device);

    CompletionQueuePtr cq = create_completion_queue(device);

    SharedReceiveQueuePtr srq = create_srq(pd);

    app_ctx_ = create_app_context(device, pd, cq, srq);

    QueuePairPtr qp = create_queue_pair(app_ctx_);

    app_ctx_->set_qp(qp);

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

    BufferPtr hello_msg = create_buffer(64);
    
    hello_msg->lenght = HELLO_MSG_SIZE;

    char gid[33];
    gid_to_wire_gid(qp_info->gid, gid);
    
    sprintf(hello_msg->message, "%04x:%06x:%06x:%s", qp_info->lid, qp_info->qpn,
                qp_info->psn, gid);

    return hello_msg;
}

void RoceConnector::set_pair_qp_info(BufferPtr msg) {

        QueuePairInfoPtr remote_qp_info = std::make_shared<QueuePairInfo>();

        char tmp_gid[33];
        sscanf(msg->message, "%x:%x:%x:%s", &remote_qp_info->lid, &remote_qp_info->qpn,
                                &remote_qp_info->psn, tmp_gid);

        remote_qp_info->gid = new ibv_gid;
        wire_gid_to_gid(tmp_gid, remote_qp_info->gid);

        // char gid[33];
        // inet_ntop(AF_INET6, remote_qp_info->gid, gid, INET6_ADDRSTRLEN);
	    // printf("GID %s\n", gid);

        app_ctx_->get_qp()->set_remote_qp_info(remote_qp_info);

        DEBUG_MSG("QP info set");

};


} // namespace Network
} // namespace Roce