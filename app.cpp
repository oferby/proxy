#include "common.h"
#include "worker.h"

Network::ProxyConfigPtr get_ptr() {
    return std::make_shared<Network::proxy_config>();
};

void print_usage() {
    puts("USAGE: <source-ip> <source-port> <TCP/ROCE> [dev-name] <dest-ip> <dest-port> <TCP/ROCE> [dev-name]");
    exit(0);
}

int main(int argc, char** argv) {

    if (argc < 7)
        print_usage();

    Event::initialize();

    WorkerPtr worker1 = get_worker("worker1");

    Network::ProxyConfigPtr config = get_ptr();
    
    config->source = {
            .ip_addr = argv[1],
            .port = atoi(argv[2])
            
    };

    int arg_shift = 0;
    
    std::string s_transport = argv[3];
    if (s_transport == "TCP")
        config->source.type = Network::TCP;
    
    else if (s_transport == "ROCE")
    {

        if (argc < 8 ) {
            print_usage();
        }

        config->source.type = Network::RoCE;
        config->source.dev_name = argv[4];
        arg_shift++;
        
    } else {
        puts("unknown local transport");
        exit(EXIT_FAILURE);
    }

    config->destination = {
        .ip_addr = argv[4 + arg_shift],
        .port = atoi(argv[5 + arg_shift])
    };

    std::string t_transport = argv[6 + arg_shift];
    if (t_transport == "TCP")
        config->destination.type = Network::TCP;
    
    else if (t_transport == "ROCE")
    {
        if (argc < 8 ) {
            print_usage();
        }       
        
        config->destination.type = Network::RoCE;
        config->destination.dev_name = argv[7 + arg_shift];
        arg_shift++;

    } else {
        puts("unknown remote transport");
        exit(EXIT_FAILURE);
    }    

    worker1->new_proxy_config(config);
    worker1->start();

    DEBUG_MSG("joining worker1");
    worker1->join();    
    
}