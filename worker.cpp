#include "worker.h"
#include <pthread.h>

Worker::Worker(std::string name) {

    printf("creating worker named %s\n", name.c_str());

    this->name = name;
    
    dispatcher_ = Event::create_dispatcher();

}

void Worker::start() {

    int status = pthread_create(&worker_thread, nullptr, 
        [](void* arg) -> void* {
            static_cast<Worker*>(arg)->run();
            return nullptr;
        }, 
        this);
        
    if (status != 0) {
        perror("error starting thread");
    }

}

void Worker::run() {

    dispatcher_->run();


}


void Worker::join() {
    pthread_join(worker_thread, nullptr);
}

void Worker::add_listener(Network::Tcp::addr_info info) {
    // listeners.push_back(Network::Tcp::create_tcp_listener(info));
    dispatcher_->add_listener(info);
}

WorkerPtr get_worker(std::string name) {
    return std::make_unique<Worker>(name);
}



