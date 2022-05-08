#include "verbs_common.h"
#include "../tcp/socket.h"

namespace Network {
namespace Roce {         

RoceClient::RoceClient(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {

    target_ = info;
    dispatcher_ = dispatcher;

    roce_connector_ = create_roce_connector(info.dev_name);

    DEBUG_MSG("Roce client created.");

    setup_pair_connection();
    
}

Network::Connection::ConnectionBasePtr RoceClient::connect() { 

    return roce_connector_->connect();
};



void RoceClient::setup_pair_connection() {

    DEBUG_MSG("exchangin QP info with remote");

    auto fd = Network::create_socket(nullptr, false);

    addr_info remote_info = {
        .ip_addr = target_.ip_addr,
        .port = ROCE_COMMUNICATION_MGR_PORT
    };

    using namespace std::chrono_literals;
    while (1) {
        
        int status = fd->connect(remote_info);
        
        if (status != -1) 
            break;
        
        puts("could not connect to remote RoCE communication server. trying ...");
        std::this_thread::sleep_for(2s);
            
    }

    BufferPtr from_server = fd->recv();

    roce_connector_->set_pair_qp_info(from_server);

    BufferPtr buf = roce_connector_->get_qp_info_msg();

    fd->send(buf);

    if (buf->lenght == -1) {
        perror("error sending qp info to server.");

    };

    fd->close();

    DEBUG_MSG("finished syncing QP info with pair");

};

RoceClientPtr create_roce_client(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<RoceClient>(info, dispatcher);
}


} // namespace Roce  
} // namespace Network
