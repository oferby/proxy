#include "worker.h"
// #include <pthread.h>

Worker::Worker(std::string name) {

    printf("creating worker named %s\n", name.c_str());

    this->name = name;
    
    dispatcher_ = Event::get_dispatcher();
    dispatcher_->get_roce_connection_manager()->set_dispatcher(dispatcher_);

}

void Worker::start() {

    worker_thread = std::thread(run, dispatcher_, &worker_thread);

}

void Worker::run(Event::DispatcherPtr dispatcher, std::thread* worker_thread) {
    
    printf("Running thread id: %u\n", worker_thread->get_id());
    dispatcher->run();
}


void Worker::join() {
    worker_thread.join();
}

void Worker::new_proxy_config(Network::ProxyConfigPtr config){
    dispatcher_->new_proxy_config(config);
}

WorkerPtr get_worker(std::string name) {
    return std::make_shared<Worker>(name);
}



