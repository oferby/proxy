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
    void on_write(BufferPtr buf) override;
    void set_connection_pair(ConnectionBasePtr connection_pair);
    ConnectionBasePtr get_connection_pair();
    void clear_connection_pair();
    void close();
    void on_close() override;
    ConnectionManagerBasePtr get_connection_manager();
    ~Connection();
private:
    int sd_;
    ConnectionManagerBasePtr connection_manager_;
    
    
    BufferPtr buf_;
    

};


ConnectionPtr create_connection(int sd, ConnectionManagerBasePtr connection_manager);


} // namespace Connection
} // namespace Network


#endif