#include "volt/volt_defs.hpp"

volt::message_ptr volt::make_message()
{
    return std::make_unique<std::vector<net_word>>();
}