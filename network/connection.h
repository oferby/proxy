#ifndef CONNECTION
#define CONNECTION

#include "../common.h"

namespace Network {
namespace Connection {

class Connection : public ConnectionBase {
public:
    Connection(int sd, ConnectionManagerBasePtr connection_manager);
    int get_sock() const override;
    void on_read() override;
    void on_write() override;
private:
    int sd_;
    ConnectionManagerBasePtr connection_manager_;
    void on_close(); 

};

using ConnectionPtr = std::shared_ptr<Connection>;
ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager);


} // namespace Connection
} // namespace Network


#endif