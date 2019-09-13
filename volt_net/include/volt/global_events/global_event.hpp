#pragma once
#ifndef global_event_hpp
#define global_event_hpp

// #include "volt/global_events/observer.hpp"
#include <algorithm>
#include <functional>
#include <iostream>

namespace volt::event
{
    /**
     * @brief A templated class that allows for global events. Useful for pure
     * decoupling of many systems
     *
     * @tparam T The type of that data that will be handed to all observers
     */
    template <typename T>
    class global_event
    {
      protected:
        /**
         * @brief A list of all subscribed functions that are to be invoked when
         * this event is fired
         *
         */
        static inline std::vector<std::function<void(T const &)> *> subscribers;

      public:
        /**
         * @brief Forbids public construction of the global_event class
         *
         */
        global_event() = delete;
        /**
         * @brief Forbids public destruction of the global_event classs
         *
         */
        ~global_event() = delete;

        /**
         * @brief Invokes all registered observers
         *
         * @param inst An instance of the data of type T to share between all
         * observers
         */
        static void call_event(T const &inst)
        {
            for (auto subscriber : subscribers)
                (*subscriber)(inst);
        }

        /**
         * @brief Will subscribe an observer to this event
         *
         * @param obs The observer that will get called
         */
        static void subscribe(std::function<void(T const &)> *obs)
        {
            std::cout << "Subscribed" << std::endl;
            subscribers.push_back(obs);
            std::cout << subscribers.size() << std::endl;
        }

        /**
         * @brief Removes a registered observer from this event
         *
         * @param obs The observer to remove from the callback list
         */
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
