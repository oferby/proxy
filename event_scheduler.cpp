#include "event_scheduler.h"


namespace Event { 

EventScheduler::EventScheduler() {

    puts("creating scheduler");

    base_ = event_base_new();
    if (!base_) {
        perror("error creating base event.");
        exit(EXIT_FAILURE);
    }


};

void EventScheduler::register_for_event(int fd, OnEventCallback cb) {

    auto new_event = event_new(base_, fd, EV_READ | EV_PERSIST, cb, (void*) base_);
    event_add(new_event, nullptr);
    event_map[fd] = new_event;

};

void EventScheduler::unregister_for_event(int fd) {
    auto event_ = event_map[fd];
    event_del(event_);
    event_map.erase(fd);
}


void EventScheduler::run() {
    int flags = EVLOOP_NO_EXIT_ON_EMPTY;
    event_base_loop(base_, flags);
}

void initialize() {
    evthread_use_pthreads();
}

EventSchedulerPtr create_event_scheduler() {
    return ::std::make_unique<EventScheduler>();
}


} //namespace Event