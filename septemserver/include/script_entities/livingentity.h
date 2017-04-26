#ifndef LIVINGENTITY_BASE_H_
#define LIVINGENTITY_BASE_H_

#include <string>
#include <vector>
#include "script_entities/script_entity.h"
#include "script_entities/roomobj.h"


struct living_entity : public script_entity
{
  //  living_entity() = default;
    
    living_entity(sol::this_state ts, EntityType et)
        : script_entity(ts, et)
    {

    }

    virtual void SendToEntity(const std::string& msg)
    {
    }

    virtual void SendToEnvironment(const std::string& msg)
    {
    }
    
    roomobj* GetRoom()
    {
        return dynamic_cast<roomobj*>(GetEnvironment());
    }

};
#endif