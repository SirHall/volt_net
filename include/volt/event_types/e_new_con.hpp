#pragma once
#ifndef e_new_con_hpp
#define e_new_con_hpp

#include "volt/global_events/global_event.hpp"
#include "volt/net_con.hpp"
#include "volt/volt_defs.hpp"

namespace volt::event
{
    class e_new_con
    {
      private:
        const volt::con_id id;

      public:
        e_new_con(volt::con_id new_con_id);
        e_new_con(e_new_con const &other) = delete;
        ~e_new_con();

        volt::con_id get_id() const;
    };
} // namespace volt::event

#endif
