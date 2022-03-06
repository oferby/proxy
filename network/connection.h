#ifndef CONNECTION
#define CONNECTION

#include "../common.h"

namespace Network {
namespace Connection {

class Connection : public ConnectionBase {
public:
    Connection(int sd);
    int get_sock() const override;
    void on_read() override;
    void on_write() override;

private:
    int sd_;

};

using ConnectionPtr = std::shared_ptr<Connection>;
ConnectionPtr create_connection(int sd);


} // namespace Connection
} // namespace Network


#endif