#include "common.h"
#include "worker.h"

Network::ProxyConfigPtr get_ptr() {
    return std::make_shared<Network::proxy_config>();
};

void print_usage() {
    puts("USAGE: <source-ip> <source-port> <TCP/ROCE> <dest-ip> <dest-port> <TCP/ROCE>");
    exit(0);
}

int main(int argc, char** argv) {

    if (argc != 7)
        print_usage();

    WorkerPtr worker1 = get_worker("worker1");

    Network::ProxyConfigPtr config = get_ptr();
    
    config->source = {
            .ip_addr = argv[1],
            .port = atoi(argv[2])
            
    };
    
    std::string s_transport = argv[3];
    if (s_transport == "TCP")
        config->source.type = Network::transport::TCP;
    else if (s_transport == "ROCE")
    {
        config->source.type = Network::transport::RoCE;
    } else {
        puts("unknown transport");
        exit(EXIT_FAILURE);
    }

    config->destination = {
        .ip_addr = argv[4],
        .port = atoi(argv[5])
    };

    std::string t_transport = argv[6];
    if (t_transport == "TCP")
        config->source.type = Network::transport::TCP;
    else if (t_transport == "ROCE")
    {
        config->source.type = Network::transport::RoCE;
    } else {
        puts("unknown transport");
        exit(EXIT_FAILURE);
    }    

    worker1->new_proxy_config(config);
    worker1->start();

    DEBUG_MSG("joining worker1");
    worker1->join();    
    
}