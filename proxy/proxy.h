#include "../network/tcp/tcp_listener.h"
#include "../network/tcp/tcp_client.h"
#include "../network/roce/verbs_common.h"

namespace Network {
namespace Proxy {

class ProxyPath : public std::enable_shared_from_this<ProxyPath> {
private:
    Network::ProxyConfigPtr config_;
    Network::ListenerPtr listener_;
    Network::ClientBasePtr client_;
    Event::DispatcherBasePtr dispatcher_;
    Network::ListenerPtr get_listener(Network::addr_info info);
    Network::ClientBasePtr get_client(Network::addr_info info, Network::ListenerPtr listener);

public:
    ProxyPath(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher);
    Network::ListenerPtr get_listener();
    Network::ClientBasePtr get_client();
    Network::ProxyConfigPtr get_config();
};

using ProxyPathPtr = std::shared_ptr<ProxyPath>;
ProxyPathPtr create_proxy_path(Network::ProxyConfigPtr config, Event::DispatcherBasePtr dispatcher);

} // namespace Proxy
} // namespace Network