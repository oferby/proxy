#include "connection_manager.h"

namespace Network {

ConnectionManager::ConnectionManager(Event::EventSchedulerPtr event_scheduler) : event_scheduler_(event_scheduler) {};

Connection::ConnectionBasePtr ConnectionManager::create_connection(int sd) {
    
    Connection::ConnectionPtr conn = Connection::create_connection(sd, shared_from_this());
    sock_map[sd] = conn;

    event_scheduler_->make_nonblocking(sd);

    event_scheduler_->register_for_event(sd, [](evutil_socket_t fd, short event, void* arg){
        static_cast<Connection::Connection*>(arg)->on_read();
    }, (void*) conn.get());

    return std::static_pointer_cast<Connection::ConnectionBase>(conn);

}

void ConnectionManager::close_connection(int sd) {
    puts("releasing connections");
    event_scheduler_->unregister_for_event(sd);
    auto sock = sock_map[sd];
        
    auto connection_pair = std::static_pointer_cast<Connection::Connection>(sock->get_connection_pair());
    auto client_sock = sock_map[connection_pair->get_sock()];
    event_scheduler_->unregister_for_event(connection_pair->get_sock());
    sock_map.erase(client_sock->get_sock());
    client_sock.reset();
    connection_pair->clear_connection_pair();

    sock_map.erase(sd);
    sock.reset();
}

ConnectionManagerPtr create_connection_manager(Event::EventSchedulerPtr event_scheduler) {
    return std::make_shared<ConnectionManager>(event_scheduler);
}


} // namespace Network