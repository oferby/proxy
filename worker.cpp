#include <worker.h>
#include <pthread.h>

Worker::Worker(std::string name) {
    this->name = name;
}

void Worker::start(std::function<void()> thread_func) {

    thread_func_ = std::move(thread_func);

    int status = pthread_create(&worker_thread, nullptr, 
        [](void* arg) -> void* {
            static_cast<Worker*>(arg)->thread_func_();
        }, 
        this);
        
    if (status != 0) {
        perror("error starting thread");
    }

}