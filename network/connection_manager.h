#ifndef CONNECTION_MGR
#define CONNECTION_MGR

#include "../common.h"
#include "connection.h"
#include "../event_scheduler.h"

namespace Network {


class ConnectionManager : public ConnectionManagerBase, public std::enable_shared_from_this<ConnectionManagerBase> {
public:
    ConnectionManager(Event::EventSchedulerPtr event_scheduler);
    void create_connection(int sd) override;
    void close_connection(int sd) override;

private:
    std::map<int,Network::Connection::ConnectionPtr> sock_map;
    Event::EventSchedulerPtr event_scheduler_;

};

using ConnectionManagerPtr = std::shared_ptr<ConnectionManager>;
ConnectionManagerPtr create_connection_manager(Event::EventSchedulerPtr event_scheduler);

} // namespace Network


#endif