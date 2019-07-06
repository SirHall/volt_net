#pragma once
#ifndef observer_hpp
#define observer_hpp

#include "volt/global_events/global_event.hpp"
#include <functional>
#include <type_traits>

namespace volt::event
{
    template <typename T>
    class observer
    {
        friend class global_event<T>;

      private:
        std::function<void(T &)> event;
        // void                     call_event(T &obj_ref) { event(obj_ref); }

      public:
        observer(observer &other) = delete; // No copying allowed
        observer(std::function<void(T &)> action)
        {
            // Subscribe to global_event
            event = action;
            global_event<T>::subscribe(&event);
        }
        ~observer()
        {
            // Unsubscribe from global_event
            global_event<T>::unsubscribe(&event);
        }
    };
} // namespace volt::event

#endif
