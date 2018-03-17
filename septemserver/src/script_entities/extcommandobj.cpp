#include "stdafx.h"
#include "script_entities/extcommandobj.h"
#include "script_entities/livingentity.h"
#include "security_context.hpp"
#include "script_entities/playerobj.h"

extcommandobj::extcommandobj(sol::state_view lua_state,
                             sol::protected_function func,
                             const std::string& name,
                             sol::object userData)
    : m_lua_state(lua_state)
{
    this->func = func;
    this->user_data = userData;
    this->name = name;
}

extcommandobj::~extcommandobj()
{
    
}

bool extcommandobj::ExecuteFunc(living_entity* le, const std::string& cmd)
{
    sol::protected_function_result result = func(m_lua_state, le, cmd);
    if(!result.valid()) {
        sol::error err = result;
        if(auto p = security_context::Instance().GetPlayerEntity()) {
            if(p->isCreator()) {
                std::stringstream ss;
                ss << "Error executing command: " << err.what();
                p->SendToEntity(ss.str());
            }
        }

        return false;
    }
    return true;
}

void extcommandobj::SetCommand(const std::string& name)
{
    this->name = name;
}

const std::string& extcommandobj::GetCommand()
{
    return name;
}

void extcommandobj::SetAliases(const sol::as_table_t<std::vector<std::string> >& aliases)
{
    const auto& vex = aliases.source;
    for(auto& s : vex) {
        this->command_aliases.push_back(s);
    }
}

void extcommandobj::AddAlias(const std::string& alias)
{
    this->command_aliases.push_back(alias);
}

const std::vector<std::string>& extcommandobj::GetAliases()
{
    return command_aliases;
}

sol::protected_function extcommandobj::GetFunc()
{
    return func;
}

sol::object extcommandobj::GetUserData()
{
    return user_data;
}
