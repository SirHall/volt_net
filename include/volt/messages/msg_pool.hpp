#pragma once
#ifndef msg_pool_hpp
#define msg_pool_hpp

#include "volt/defs.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace volt
{
    class msg_pool
    {
      private:
        static std::vector<volt::message_ptr> msg_vec;
        static std::mutex                     _mut;

      public:
        static volt::message_ptr get_message()
        {
            if (msg_vec.size() > 0)
            {
                volt::message_ptr msg;
                {
                    std::lock_guard guard(_mut);
                    msg = std::move(msg_vec.back());
                    msg_vec.pop_back;
                }

                return std::move(msg);
            }
            return std::move(volt::make_message());
        }
        static void return_message(volt::message_ptr msg)
        {
            std::lock_guard guard(_mut);
            msg_vec.push_back(std::move(msg));
        }
    };
} // namespace volt

#endif
