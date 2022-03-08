#ifndef SCHEDULER
#define SCHEDULER

#include "common.h"

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/event-config.h>

namespace Event {

class EventScheduler : public EventSchedulerBase {
    private:
        EventScheduler() {

            puts("creating scheduler");

            base_ = event_base_new();
            if (!base_) {
                perror("error creating base event.");
                exit(EXIT_FAILURE);
            }

        };
        event_base* base_;
        std::map<int, event*> event_map;

    public:
        static EventScheduler& get_instance() {
            static EventScheduler instance;
            return instance;
        }
        
        void run();
        void register_for_event(int fd, OnEventCallback cb, void* arg);
        void unregister_for_event(int fd);
        void make_nonblocking(int fd);

};

void initialize(); 

using EventSchedulerPtr = std::shared_ptr<EventScheduler>;
EventSchedulerPtr create_event_scheduler();


} // namespace Event

#endif