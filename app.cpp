#include "common.h"
#include "worker.h"
#include "tcp/tcp_listener.h"


int main(int argc, char** argv) {

    WorkerPtr worker1 = get_worker("worker1");

    Network::addr_info info = {
        .ip_addr = "localhost",
        .port = 8585
    };
    
    worker1->add_listener(info);

    info.port = 8000;
    worker1->add_listener(info);

    worker1->start();

    // WorkerPtr worker2 = get_worker("worker1");
    // worker2->add_listener(info);
    // worker2->start();

    puts("joining worker1");
    worker1->join();    
    
}