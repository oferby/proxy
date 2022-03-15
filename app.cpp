#include "common.h"
#include "worker.h"

Network::ProxyConfigPtr get_ptr() {
    return std::make_shared<Network::proxy_config>();
};

void print_usage() {
    puts("USAGE: <source-ip> <source-port> <dest-ip> <dest-port>");
    exit(0);
}

int main(int argc, char** argv) {

    if (argc != 5)
        print_usage();

    WorkerPtr worker1 = get_worker("worker1");

    Network::ProxyConfigPtr config = get_ptr();
    
    config->source = {
            .ip_addr = argv[1],
            .port = atoi(argv[2])
    };

    config->destination = {
        .ip_addr = argv[3],
        .port = atoi(argv[4])
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

    DEBUG_MSG("joining worker1");
    worker1->join();    
    
}