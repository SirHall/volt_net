#pragma once
#ifndef global_event_hpp
#define global_event_hpp

// #include "volt/global_events/observer.hpp"
#include <algorithm>
#include <functional>
#include <iostream>

namespace volt::event
{
    template <typename T>
    class global_event
    {
      protected:
        static inline std::vector<std::function<void(T const &)> *> subscribers;

      public:
        global_event()  = delete;
        ~global_event() = delete;

        static void call_event(T const &inst)
        {
            for (auto subscriber : subscribers)
                (*subscriber)(inst);
        }

        static void subscribe(std::function<void(T const &)> *obs)
        {
            std::cout << "Subscribed" << std::endl;
            subscribers.push_back(obs);
            std::cout << subscribers.size() << std::endl;
        }

        static void unsubscribe(std::function<void(T const &)> *obs)
        {
            std::cout << "Unsubbing" << std::endl;
            std::cout << subscribers.size() << std::endl;
            bool in_place = true; // TODO: Move this
            auto it = std::find(subscribers.begin(), subscribers.end(), obs);
            if (it != subscribers.end())
            {
                if (in_place)
                {
                    subscribers.erase(it);
                }
                else
                {
                    // Swap with last element and remove
                    std::iter_swap(it, subscribers.end());
                    subscribers.pop_back();
                }
            }
            else
            {
                std::cout << "\t\tNothing deleted" << std::endl;
            }
        }
    };
} // namespace volt::event

#endif
