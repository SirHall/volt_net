#pragma once
#ifndef e_new_con_hpp
#define e_new_con_hpp

// #include "volt/net_con.hpp"
#include "volt/net/volt_net_defs.hpp"

namespace volt::net
{
    /**
     * @brief The data for a global event that fires when any new connection is
     * opened
     *
     */
    class e_new_con
    {
    private:
        const con_id id;

    public:
        /**
         * @brief Construct a new e_new_con object with the connection id of the
         * new connection
         *
         * @param new_con_id The connection id of this new connection
         */
        e_new_con(con_id new_con_id);
        /**
         * @brief Forbids object copying
         *
         * @param other The instance of the object to copy
         */
        e_new_con(e_new_con const &other) = delete;
        /**
         * @brief Destroy the e_new_con object
         *
         */
        ~e_new_con();

        /**
         * @brief Returns the connection id of the new connection
         *
         * @return volt::con_id The connection id of the new connection
         */
        con_id get_id() const;
    };
} // namespace volt::net

#endif
