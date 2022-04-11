#include "verbs_common.h"

namespace Network {
namespace Roce {

QueuePair::QueuePair(AppContextPtr ctx) : ctx_(ctx) {

    ibv_qp_init_attr_ex init_attr_ex = {0};
    init_attr_ex.send_cq = ctx_->get_cq()->get_cq().get();
    init_attr_ex.recv_cq = ctx_->get_cq()->get_cq().get();
    // init_attr_ex.srq = ctx_->get_srq()->get_srq().get();

    init_attr_ex.cap     = {};
    init_attr_ex.cap.max_send_wr  = MAX_WR;
    init_attr_ex.cap.max_recv_wr  = MAX_WR;
    init_attr_ex.cap.max_send_sge = MAX_SGE;
    init_attr_ex.cap.max_recv_sge = MAX_SGE;
            
    init_attr_ex.qp_type = IBV_QPT_RC;
    init_attr_ex.sq_sig_all = 1;
    init_attr_ex.pd = ctx_->get_pd()->get_ibv_pd().get();
    init_attr_ex.comp_mask  = IBV_QP_INIT_ATTR_PD;

    qp_ = std::shared_ptr<ibv_qp>(ibv_create_qp_ex(ctx_->get_device()->get_context(), &init_attr_ex));
    if (!qp_) {
        perror("could not create QP");
        exit(EXIT_FAILURE);
    }

    DEBUG_MSG("QP created");

    ibv_qp_attr attr = {};
    attr.qp_state        = IBV_QPS_INIT;
    attr.pkey_index      = 0;
    attr.port_num        = PORT_NUM;
    attr.qp_access_flags = 0;

    int state = IBV_QP_STATE              |
                IBV_QP_PKEY_INDEX         |
                IBV_QP_PORT               |
                IBV_QP_ACCESS_FLAGS;  

    int status = ibv_modify_qp(qp_.get(), &attr, state);

    if (status == 0)
        DEBUG_MSG("QP changed to INIT");
    else {
        perror("could not change QP state to INIT");
        exit(EXIT_FAILURE);
    }
        
    
};

QueuePairInfoPtr QueuePair::get_qp_info() {

    QueuePairInfoPtr qp_info = std::make_shared<QueuePairInfo>();
    qp_info->lid = ctx_->get_device()->get_lid();
    qp_info->qpn = qp_->qp_num;
    qp_info->psn = 1;
    qp_info->gid = ctx_->get_device()->get_gid();
    
    return qp_info; 
};


void QueuePair::set_remote_qp_info(QueuePairInfoPtr qp_info) {

	ibv_qp_attr attr = {};
    attr.qp_state		= IBV_QPS_RTR;
    attr.path_mtu		= IBV_MTU_512; // TODO
    attr.dest_qp_num		= qp_info->qpn;
    attr.rq_psn			= qp_info->psn;
    attr.max_dest_rd_atomic	= 1;
    attr.min_rnr_timer		= 12;
    
    attr.ah_attr.dlid		= qp_info->lid;
    attr.ah_attr.sl		= 0;
    attr.ah_attr.src_path_bits	= 0;
    attr.ah_attr.port_num	= PORT_NUM;

    attr.ah_attr.is_global = 1;
    attr.ah_attr.grh.hop_limit = 1;
    attr.ah_attr.grh.dgid = *(qp_info->gid);
    attr.ah_attr.grh.sgid_index = GID_IDX;

	if (ibv_modify_qp(qp_.get(), &attr,
			  IBV_QP_STATE              |
			  IBV_QP_AV                 |
			  IBV_QP_PATH_MTU           |
			  IBV_QP_DEST_QPN           |
			  IBV_QP_RQ_PSN             |
			  IBV_QP_MAX_DEST_RD_ATOMIC |
			  IBV_QP_MIN_RNR_TIMER)) {
		fprintf(stderr, "Failed to modify QP to RTR\n");
		exit(EXIT_FAILURE);
	}

    // switch_to_rts();

    attr.qp_state	    = IBV_QPS_RTS;
	attr.timeout	    = 14;
	attr.retry_cnt	    = 7;
	attr.rnr_retry	    = 7;
	attr.sq_psn	    = 0;
	attr.max_rd_atomic  = 1;
	
    if (ibv_modify_qp(qp_.get(), &attr,
			  IBV_QP_STATE              |
			  IBV_QP_TIMEOUT            |
			  IBV_QP_RETRY_CNT          |
			  IBV_QP_RNR_RETRY          |
			  IBV_QP_SQ_PSN             |
			  IBV_QP_MAX_QP_RD_ATOMIC)) {
  
            fprintf(stderr, "Failed to modify QP to RTS\n");
            exit(EXIT_FAILURE);
	}

    DEBUG_MSG("QP ready.");    

};


IbvQp QueuePair::get_ibv_qp() {
    return qp_;
}

QueuePairPtr create_queue_pair(AppContextPtr ctx) {
    return std::make_shared<QueuePair>(ctx);
};

} // namespace Roce

} // namespace Network
