#pragma once
#ifndef e_closed_con_hpp
#define e_closed_con_hpp

#include "volt/net/volt_net_defs.hpp"

namespace volt::net
{
    /**
     * @brief The data for a global event that fires when any connection is
     * closed
     *
     */
    class e_closed_con
    {
    private:
        const con_id id;

    public:
        /**
         * @brief Construct a new e_closed_con object with the connection id of
         * the closed connection
         *
         * @param closed_con_id The connection id of the closed connection
         */
        e_closed_con(con_id closed_con_id);
        /**
         * @brief Forbids copying
         *
         * @param other A reference to the instance to copy from
         */
        e_closed_con(e_closed_con const &other) = delete;
        /**
         * @brief Destroy the e_closed_con object
         *
         */
        ~e_closed_con();

        /**
         * @brief Returns the connection id of the closed connection
         *
         * @return volt::con_id The connection id of the closed connection
         */
        con_id get_id() const;
    };
} // namespace volt::net

#endif
