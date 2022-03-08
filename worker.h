#ifndef WORKER
#define WORKER

#include "common.h"
#include "dispatcher.h"
#include <vector>


class Worker {

    private:
        std::string name;
        pthread_t worker_thread;
        std::function<void()> thread_func_;
        Event::DispatcherPtr dispatcher_;
        void run();

    public:

        Worker(std::string name);
        void new_proxy_config(Network::ProxyConfigPtr config);
        void start();
        void join();


};


using WorkerPtr = std::unique_ptr<Worker>;
WorkerPtr get_worker(std::string name);

#endif