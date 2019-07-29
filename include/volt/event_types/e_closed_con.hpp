#pragma once
#ifndef e_closed_con_hpp
#define e_closed_con_hpp

#include "volt/volt_defs.hpp"

namespace volt::event
{
    class e_closed_con
    {
      private:
        const volt::con_id id;

      public:
        e_closed_con(volt::con_id closed_con_id);
        e_closed_con(e_closed_con const &other) = delete;
        ~e_closed_con();

        volt::con_id get_id() const;
    };
} // namespace volt::event

#endif
