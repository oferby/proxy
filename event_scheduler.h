#ifndef SCHEDULER
#define SCHEDULE

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/event-config.h>

#include <functional>
#include <memory>
#include <map>


namespace Event {

// typedef void (*OnEventCallback) (int fd, short event, void* arg);
using OnEventCallback = void (evutil_socket_t fd, short event, void* arg);

class EventScheduler {
    private:
        event_base* base_;
        std::map<int, event*> event_map;

    public:
        EventScheduler();
        void run();
        void register_for_event(int fd, OnEventCallback cb);
        void unregister_for_event(int fd);
       

};

void initialize(); 

using EventSchedulerPtr = ::std::unique_ptr<EventScheduler>;

EventSchedulerPtr create_event_scheduler(); 

} // namespace Event

#endif