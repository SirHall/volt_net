#include "volt/event_types/e_new_con.hpp"

using namespace volt::net;

e_new_con::e_new_con(con_id new_con_id) : id(new_con_id) {}

e_new_con::~e_new_con() {}

con_id e_new_con::get_id() const { return id; }