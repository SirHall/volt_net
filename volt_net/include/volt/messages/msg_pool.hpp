#pragma once
#ifndef msg_pool_hpp
#define msg_pool_hpp

#include "volt/volt_defs.hpp"
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace volt::msg_pool
{

    volt::message_ptr get_message();

    void return_message(volt::message_ptr msg);
} // namespace volt::msg_pool

#endif
