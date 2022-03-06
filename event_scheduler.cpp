#include "event_scheduler.h"


namespace Event { 

void EventScheduler::register_for_event(int fd, OnEventCallback cb, void* arg) {
    puts("registering new event");

    // auto new_event = event_new(base_, fd, EV_READ | EV_PERSIST, cb, (void*) base_);
    auto new_event = event_new(base_, fd, EV_READ | EV_PERSIST, cb, arg);
    event_add(new_event, nullptr);
    event_map[fd] = new_event;

};

void EventScheduler::unregister_for_event(int fd) {
    auto event_ = event_map[fd];
    event_free(event_);
    event_map.erase(fd);
}


void EventScheduler::run() {
    puts("running event loop");
    
    int flags = EVLOOP_NO_EXIT_ON_EMPTY;
    event_base_loop(base_, flags);
}

void initialize() {
    evthread_use_pthreads();
}

EventSchedulerPtr create_event_scheduler(){
    return std::make_shared<EventScheduler>(EventScheduler::get_instance());
}

} //namespace Event