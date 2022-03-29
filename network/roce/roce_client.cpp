#include "verbs_common.h"

namespace Network {
namespace Roce {         

RoceClient::RoceClient(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {

    target_ = info;
    dispatcher_ = dispatcher;

    roce_connector_ = create_roce_connector(info.dev_name);

    DEBUG_MSG("Roce client created.");
    
}

Network::Connection::ConnectionBasePtr RoceClient::connect() { return nullptr; }; // TODO


RoceClientPtr create_roce_client(Network::addr_info info, Event::DispatcherBasePtr dispatcher) {
    return std::make_shared<RoceClient>(info, dispatcher);
}


} // namespace Roce  
} // namespace Network
