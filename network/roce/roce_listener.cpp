#include "verbs_common.h"
#include "../tcp/socket.h"


namespace Network {
namespace Roce {
    
RoceListener::RoceListener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    
    DEBUG_MSG("creating RoCE listener");
    
    info_ = info;
    dispatcher_ = dispatcher; 

    roce_connector_ = create_roce_connector(info.dev_name);

    sd_ = create_roce_socket(dispatcher_->get_connection_manager());

    setup_comm_server();
    
};

Network::SocketBasePtr RoceListener::get_socket() {
    return sd_;
};

void RoceListener::set_client_side(Network::ClientBasePtr client) {
    sd_->set_client_side(client);
};

void RoceListener::setup_comm_server() {

    DEBUG_MSG("creating RoCE Communication Server");

    tcp_sd_ =  Network::create_socket(nullptr, true);

    addr_info info = {
        .ip_addr = "localhost",
        .port = ROCE_COMMUNICATION_MGR_PORT
    };

    int status = tcp_sd_->bind(info);

    dispatcher_->get_event_scheduler()->register_for_event(tcp_sd_->get(), 
        [] (int fd, short event, void* arg) {
            DEBUG_MSG("got new event");
            static_cast<RoceListener*>(arg)->on_connect();
        }, 
        this);
    
    tcp_sd_->listen();

    DEBUG_MSG("RoCE communication server started...");

};


void RoceListener::on_connect() {

    DEBUG_MSG("got new connection on RoCE communication server.");

    auto client = std::make_shared<sockaddr_in>();

    int new_sock = sd_->accept(client);
    if (new_sock == -1) {
        puts("error accepting new connection!");
        return;
    }

}

RoceListenerPtr create_roce_listener(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<RoceListener>(info, dispatcher);
}

} // namespace Roce
} // namespace Network




