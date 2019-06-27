#pragma once
#ifndef msg_send_hpp
#define msg_send_hpp

#include "volt/defs.hpp"
#include <vector>

namespace volt::net::msg
{
    class msg_send : std::vector<volt::net_word>
    {
      private:
        msg_send() {}
        ~msg_send() {}
    }
} // namespace volt::net::msg

#endif
