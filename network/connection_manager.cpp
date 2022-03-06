#include "connection_manager.h"

namespace Network {

ConnectionManager::ConnectionManager(Event::EventSchedulerPtr event_scheduler) : event_scheduler_(event_scheduler) {};

Connection::ConnectionBasePtr ConnectionManager::create_connection(int sd) {
    
    Connection::ConnectionPtr conn = Connection::create_connection(sd);
    sock_map[sd] = conn;

    return std::static_pointer_cast<Connection::ConnectionBase>(conn);
}

ConnectionManagerPtr create_connection_manager(Event::EventSchedulerPtr event_scheduler) {
    return std::make_shared<ConnectionManager>(event_scheduler);
}


} // namespace Network