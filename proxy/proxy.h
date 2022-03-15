#include "../common.h"
#include "../network/tcp/tcp_listener.h"
#include "../network/tcp/tcp_client.h"

namespace Network {
namespace Proxy {

class ProxyPath {
private:
    Network::Tcp::TcpListenerPtr listener_;
    Network::Tcp::TcpClientPtr client_;
    Event::DispatcherBasePtr dispatcher_;

public:
    ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher);
    Network::Tcp::TcpListenerPtr get_listener();
    Network::Tcp::TcpClientPtr get_client();
};

using ProxyPathPtr = std::shared_ptr<ProxyPath>;
ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher);

} // namespace Proxy
} // namespace Network