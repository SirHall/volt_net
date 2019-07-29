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
      private:
        std::function<void(T const &)> event;

      public:
        observer(observer const &other) = delete; // No copying allowed
        observer(std::function<void(T const &)> action)
        {
            // Subscribe to global_event
            event = action;
            global_event<T>::subscribe(&event);
        }
        ~observer()
        {
            std::cout << "Before del" << std::endl;
            // Unsubscribe from global_event
            global_event<T>::unsubscribe(&event);
            std::cout << "After del" << std::endl;
        }
    };
} // namespace volt::event

#endif
