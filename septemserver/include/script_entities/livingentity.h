#ifndef LIVINGENTITY_BASE_H_
#define LIVINGENTITY_BASE_H_

#include <string>
#include <vector>
#include "script_entities/script_entity.h"
#include "script_entities/roomobj.h"

class roomobj;

struct handobj : public container_base
{
     virtual void AddEntityToInventory(script_entity * se) override
     {
         container_base::AddEntityToInventory(se);
     }
     
     virtual bool RemoveEntityFromInventoryByID( const std::string& id  ) override
     {
         return container_base::RemoveEntityFromInventoryByID(id);
     }
     
    virtual bool RemoveEntityFromInventory( script_entity * se ) override
    {
        return container_base::RemoveEntityFromInventory(se);
    }
     
};

struct living_entity : public script_entity
{

    living_entity(sol::this_state ts, sol::this_environment te, EntityType et, std::string name)
        : script_entity(ts, te, et, name)
    {

    }

    virtual void SendToEntity(const std::string& msg)
    {
    }

    virtual void SendToEnvironment(const std::string& msg)
    ;
    
    virtual bool DoCommand(sol::this_state ts, const std::string& cmd );
    
    roomobj* GetRoom();
    
    handobj RightHand;
    handobj LeftHand;

};
#endif