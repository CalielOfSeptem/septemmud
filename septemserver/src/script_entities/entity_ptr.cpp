#include "stdafx.h"
#include "script_entities/entity_ptr.h"
	

	
entity_ptr::entity_ptr(sol::state_view lua_state, script_entity* se)
 : m_lua_state(lua_state)
{
	set_alive(true);
	m_se = se;
	m_c = se->on_destroy.connect(
		[this]
		{
			set_alive(false);
		});
}

void entity_ptr::set_alive(bool b)
{
	m_bAlive = b;
	if( !b )
		m_c.disconnect();
}

bool entity_ptr::get_alive()
{
	return m_bAlive;
}

entity_ptr::~entity_ptr()
{
}
script_entity* entity_ptr::get_entity()
{
	if( !get_alive() )
		return NULL;
	else return m_se;
}
