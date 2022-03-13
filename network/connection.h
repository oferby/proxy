#ifndef CONNECTION
#define CONNECTION

#include "../common.h"

namespace Network {
namespace Connection {

class Connection;
using ConnectionPtr = std::shared_ptr<Connection>;

class Connection : public ConnectionBase {
public:
    Connection(int sd, ConnectionManagerBasePtr connection_manager);
    int get_sock() override;
    void on_read() override;
    void on_write(char* buf, size_t size) override;
    void set_connection_pair(ConnectionBasePtr connection_pair);
    ConnectionBasePtr get_connection_pair();
    void clear_connection_pair();
    ~Connection();
private:
    int sd_;
    ConnectionManagerBasePtr connection_manager_;
    void on_close();
    ConnectionPtr connection_pair_; 

};


ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager);


} // namespace Connection
} // namespace Network


#endif