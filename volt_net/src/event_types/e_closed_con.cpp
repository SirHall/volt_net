#include "volt/event_types/e_closed_con.hpp"

using namespace volt::event;

e_closed_con::e_closed_con(volt::con_id new_con_id) : id(new_con_id) {}

e_closed_con::~e_closed_con() {}

volt::con_id e_closed_con::get_id() const { return id; }