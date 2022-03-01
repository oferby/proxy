#include <common.h>
#include <event_scheduler.h>


class Worker {

    private:
        std::string name;
        pthread_t worker_thread;
        std::function<void()> thread_func_;

    public:

        Worker(std::string name);
        void start(std::function<void()> thread_func);


};


using WorkerPtr = std::unique_ptr<Worker>;


WorkerPtr get_worker(std::string name) {
    return std::make_unique<Worker>(name);
}