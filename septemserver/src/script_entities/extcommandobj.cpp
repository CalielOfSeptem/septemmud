#include "stdafx.h"
#include "script_entities/extcommandobj.h"
#include "security_context.hpp"
#include "script_entities/playerobj.h"
bool extcommandobj::ExecuteFunc(living_entity * le, const std::string& cmd)
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
