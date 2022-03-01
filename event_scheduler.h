#include <event2/event.h>
#include "event2/thread.h"
#include <functional>
#include <memory>
#include <map>


namespace Events{

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


void initialize() {
    evthread_use_pthreads();
}


using EventSchedulerPtr = ::std::unique_ptr<EventScheduler>;

EventSchedulerPtr create_event_scheduler() {
    ::std::unique_ptr<EventScheduler>(new EventScheduler);
    return ::std::make_unique<EventScheduler>();

}

} // namespace Events
