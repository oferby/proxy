#include "common.h"
#include "worker.h"
#include "tcp/tcp_listener.h"


Network::ProxyConfigPtr get_ptr() {
    return std::make_shared<Network::proxy_config>();
};


int main(int argc, char** argv) {

    WorkerPtr worker1 = get_worker("worker1");

    Network::ProxyConfigPtr config = get_ptr();
    
    config->source = {
            .ip_addr = "localhost",
            .port = 8585
    };
    config->destination = {
        .ip_addr = "localhost",
        .port = 8586
    };
    worker1->new_proxy_config(config);

    // config->source = {
    //         .ip_addr = "localhost",
    //         .port = 8586
    // };
    // config->destination = {
    //     .ip_addr = "localhost",
    //     .port = 5000
    // };
    // worker1->new_proxy_config(config);


    worker1->start();


    puts("joining worker1");
    worker1->join();    
    
}