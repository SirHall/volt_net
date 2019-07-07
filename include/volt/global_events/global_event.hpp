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
        static inline std::vector<std::function<void(T const &)> *> subscribers;
        // static inline T glob_inst;

      public:
        global_event() = delete;

        ~global_event() = delete;

        // void set(T inst)
        // {
        //     ~glob_inst;
        //     glob_inst = std::move(inst);
        // }

        // T &get() { return glob_inst; }

        // T *operator->() { return &glob_inst; }

        static void call_event(T const &inst)
        {
            for (auto subscriber : subscribers)
                (*subscriber)(glob_inst);
        }

        static void subscribe(std::function<void(T const &)> *obs)
        {
            subscribers.push_back(obs);
        }

        static void unsubscribe(std::function<void(T const &)> *obs)
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
