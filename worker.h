#ifndef WORKER
#define WORKER

#include "common.h"
#include "dispatcher.h"
#include <vector>


class Worker {

    private:
        std::string name;
        std::thread worker_thread;
        std::function<void()> thread_func_;
        Event::DispatcherPtr dispatcher_;
        static void run(Event::DispatcherPtr dispatcher, std::thread* worker_thread);

    public:

        Worker(std::string name);
        void new_proxy_config(Network::ProxyConfigPtr config);
        void start();
        void join();


};


using WorkerPtr = std::shared_ptr<Worker>;
WorkerPtr get_worker(std::string name);

#endif