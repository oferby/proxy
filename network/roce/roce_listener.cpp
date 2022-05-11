#include "verbs_common.h"
#include "../tcp/socket.h"


namespace Network {
namespace Roce {
    
RoceListener::RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    
    DEBUG_MSG("creating RoCE listener");
    
    info_ = info;
    dispatcher_ = dispatcher; 

    roce_connector_ = create_roce_connector(info.dev_name, true);

    setup_comm_server();
    
};

Network::SocketBasePtr RoceListener::get_socket() {
    return std::make_shared<RoceVirtualSocket>(roce_connector_);
};

void RoceListener::set_client_side(Network::ClientBasePtr client) {
    roce_connector_->set_client_side(client);
};

void RoceListener::setup_comm_server() {

    DEBUG_MSG("creating RoCE Communication Server");

    tcp_sd_ =  Network::create_socket(nullptr, false);

    addr_info info = {
        .ip_addr = "localhost",
        .port = ROCE_COMMUNICATION_MGR_PORT + info_.worker_id
    };

    int status = tcp_sd_->bind(info);

    dispatcher_->get_event_scheduler()->register_for_event(tcp_sd_->get(), 
        [] (int fd, short event, void* arg) {
            DEBUG_MSG("got new event");
            static_cast<RoceListener*>(arg)->on_connect();
        }, 
        this, false);
    
    tcp_sd_->listen();

    DEBUG_MSG("RoCE communication server started...");

};


void RoceListener::on_connect() {

    DEBUG_MSG("got new connection on RoCE communication server.");

    auto new_sock = tcp_sd_->accept();
    if (new_sock->get() == -1) {
        DEBUG_MSG("error accepting new connection!");
        return;
    }

    BufferPtr qp_info_buf = roce_connector_->get_qp_info_msg();

    printf("sending msg to client: %s\n", qp_info_buf->message);

    new_sock->send(qp_info_buf);

    if (qp_info_buf->lenght == -1) {
        perror("could not sent QP infor to client");
        return;
    }

    BufferPtr client_qp_info_buf = new_sock->recv();

    printf("got msg from client: %s\n", client_qp_info_buf->message);

    if (client_qp_info_buf->lenght <= 0) {
        perror("could not get QP info from client");
        return;
    }

    roce_connector_->set_pair_qp_info(client_qp_info_buf);

    new_sock->close();

    DEBUG_MSG("finished syncing QP info with pair");

}

RoceListenerPtr create_roce_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<RoceListener>(info, dispatcher);
}

} // namespace Roce
} // namespace Network




