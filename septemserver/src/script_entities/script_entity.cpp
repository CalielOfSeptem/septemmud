#include "script_entities/script_entity.h"
#include "entity_manager.h"

script_entity::script_entity(sol::this_state ts, EntityType myType, std::string name)
    : m_type(myType)
    , environment_(NULL), name(name)
{
    lua_State* L = ts;
    // references the object that called this function
    // in constructors:

    m_userdata = std::shared_ptr<_sol_userdata_>(new _sol_userdata_);
    m_userdata->selfobj = sol::userdata(L, -1);

    // the -1 (NEGATIVE one) above
    // means "off the top fo the stack"
    // (-1 is the top, -2 is one below, etc...)

    // definitely the same
    //    script_entity& self = selfobj.as<script_entity>();
    // assert(&self == this);
    entity_manager::Instance().register_entity(this, myType);
}

script_entity::~script_entity()
{
    LOG_DEBUG << "Destroyed object, script path= " << script_path;
    entity_manager::Instance().deregister_entity(this, m_type);
}

void script_entity::debug(const std::string& msg)
{
    std::stringstream ss;
    ss << this->GetInstancePath() << " " << msg;
    std::string d = ss.str();
    entity_manager::Instance().debug(d);
}
