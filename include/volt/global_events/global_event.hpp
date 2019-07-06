#pragma once
#ifndef global_event_hpp
#define global_event_hpp

// #include "volt/global_events/observer.hpp"
#include <algorithm>
#include <functional>

namespace volt::event
{
    template <typename T>
    class global_event
    {
      protected:
        static inline std::vector<std::function<void(T &)> *> subscribers;
        static inline T                                       glob_inst;

        void call_subscribers()
        {
            // Fire event on object destruction
            for (auto subscriber : subscribers)
                (*subscriber)(glob_inst);
        }

      public:
        global_event() {}

        ~global_event() { call_subscribers(); }

        T &get() { return glob_inst; }

        static void subscribe(std::function<void(T &)> *obs)
        {
            subscribers.push_back(obs);
        }

        static void unsubscribe(std::function<void(T &)> *obs)
        {
            bool in_place = true; // TODO: Move this
            if (in_place)
            {
                subscribers.erase(
                    std::find(subscribers.begin(), subscribers.end(), obs));
            }
            else
            {
                // Swap with last element and remove
                std::iter_swap(
                    std::find(subscribers.begin(), subscribers.end(), obs),
                    subscribers.end());
                subscribers.pop_back();
            }
        }
    };
} // namespace volt::event

#endif
