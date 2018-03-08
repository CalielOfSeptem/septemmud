#include "stdafx.h"
#include "security_context.hpp"
#include "script_entities/playerobj.h"

bool security_context::isArch()
{
    script_entity* se = GetCurrentEntity();
    if(auto p = dynamic_cast<playerobj*>(se)) {
        if(p->isArch()) {
            return true;
        }
    }
    return false;
}

bool security_context::isCreator()
{
    script_entity* se = GetCurrentEntity();
    if(auto p = dynamic_cast<playerobj*>(se)) {
        if(p->isCreator()) {
            return true;
        }
    }
    return false;
}

bool security_context::isPlayer()
{
    script_entity* se = GetCurrentEntity();
    if(auto p = dynamic_cast<playerobj*>(se)) {
        return true;
    }
    return false;
}

playerobj* security_context::GetPlayerEntity()
{
    return dynamic_cast<playerobj*>(_currentEntity);
}
