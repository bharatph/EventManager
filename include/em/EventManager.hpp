#ifndef _EVENT_MANAGER
#define _EVENT_MANAGER
#include <iostream>
#include <functional>
#include <map>

namespace em
{
enum class event {
  ANY
};

template <class Event, typename... EventCallbackType>
class EventManager
{
  public:
    typedef std::function<void(EventCallbackType...)> EventCallback;

  private:
    std::map<Event, EventCallback> events;
    EventCallback anyCallback = nullptr;

  public:
    EventManager()
    {
    }

    EventManager &on(Event event, EventCallback callback)
    {
        events[event] = callback;
	return *this;
    }

    template <class T>
    EventManager &on(Event event, T *const object, void (T::*callback)(EventCallbackType...))
    {
        using namespace std::placeholders;
        events[event] = std::bind(callback, object, _1);
        return *this;
    }

    template <class T>
    EventManager &on(em::event, T *const object, void (T::*callback)(EventCallbackType...))
    {
        using namespace std::placeholders;
        anyCallback = std::bind(callback, object, _1);
        return *this;
    }

    EventManager &on(em::event, EventCallback callback){
      using namespace std::placeholders;
      anyCallback = callback;
      return *this;
    }

    void fireEvent(Event event, EventCallbackType... t)
    {
        if(anyCallback != nullptr){
            anyCallback(t...);
        }
        if (events[event] == nullptr)
            return;
        EventCallback callback = events[event];
        callback(t...);
    }

    void fireEvent(em::event event, EventCallbackType... t){
      if(anyCallback == nullptr)
        return;
      anyCallback(t...);
    }
};
}
#endif
