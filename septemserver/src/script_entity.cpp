#include "script_entity.h"
#include "entity_manager.h"

script_entity::script_entity(sol::this_state ts, EntityType myType)
{
    m_type = myType;
    lua_State* L = ts;
    // references the object that called this function
    // in constructors:
    sol::userdata selfobj = sol::userdata(L, -1);

    // the -1 (NEGATIVE one) above
    // means "off the top fo the stack"
    // (-1 is the top, -2 is one below, etc...)

    // definitely the same
    script_entity& self = selfobj.as<script_entity>();
    assert(&self == this);
    entity_manager::Instance().register_entity(*this, myType);
}
