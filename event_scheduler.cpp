#include "event_scheduler.h"


namespace Event { 

EventScheduler::EventScheduler() {
    DEBUG_MSG("creating scheduler");

    base_ = event_base_new();
    if (!base_) {
        perror("error creating base event.");
        exit(EXIT_FAILURE);
    }
}
 
void EventScheduler::register_for_event(int fd, OnEventCallback cb, void* arg, bool persist) {
    DEBUG_MSG("registering new event");

    auto new_event = event_new(base_, fd, persist ?  EV_READ | EV_PERSIST : EV_READ, cb, arg);
    event_add(new_event, nullptr);
    event_map[fd] = new_event;

};

void EventScheduler::unregister_for_event(int fd) {
    DEBUG_MSG("unregistering event");
    auto event_ = event_map[fd];
    event_free(event_);
    event_map.erase(fd);
}

void EventScheduler::make_nonblocking(int fd) {
    evutil_make_socket_nonblocking(fd);
}


void EventScheduler::run() {
    DEBUG_MSG("running event loop");
    
    int flags = EVLOOP_NO_EXIT_ON_EMPTY;
    event_base_loop(base_, flags);
}

void initialize() {
    int status = evthread_use_pthreads();
    if (status < 0) {
        puts("error setting libevent pthreads.");
        exit(EXIT_FAILURE);
    }
}

EventSchedulerPtr create_event_scheduler(){
    return std::make_shared<EventScheduler>();
}

} //namespace Event