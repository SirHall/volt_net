#include "volt/net/event_types/e_closed_con.hpp"

using namespace volt::net;

e_closed_con::e_closed_con(con_id new_con_id) : id(new_con_id) {}

e_closed_con::~e_closed_con() {}

con_id e_closed_con::get_id() const { return this->id; }