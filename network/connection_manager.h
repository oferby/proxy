#ifndef CONNECTION_MGR
#define CONNECTION_MGR

#include "../common.h"
#include "connection.h"
#include "../event_scheduler.h"

namespace Network {


class ConnectionManager : public ConnectionManagerBase {
public:
    ConnectionManager(Event::EventSchedulerPtr event_scheduler);
    Connection::ConnectionBasePtr create_connection(int sd);
    // void close_connection(Connection::ConnectionBasePtr connection) override;

private:
    std::map<int,Network::Connection::ConnectionPtr> sock_map;
    Event::EventSchedulerPtr event_scheduler_;

};

using ConnectionManagerPtr = std::shared_ptr<ConnectionManager>;
ConnectionManagerPtr create_connection_manager(Event::EventSchedulerPtr event_scheduler);

} // namespace Network


#endif