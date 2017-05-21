#ifndef LIVINGENTITY_BASE_H_
#define LIVINGENTITY_BASE_H_

#include <string>
#include <vector>
#include "script_entities/script_entity.h"
#include "script_entities/roomobj.h"
#include "script_entities/itemobj.h"
class roomobj;
struct living_entity;

struct handobj : public container_base, public script_entity
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
    
    bool IsEmpty()
    {
        return (this->GetInventory().size()==0);
    }
    
    itemobj * GetItem()
    {
        if(IsEmpty())
            return NULL;
        return dynamic_cast<itemobj*>(*std::next(this->GetInventory().begin(), 0));
    }
    
    virtual script_entity * GetOwner() override
    {
        return this;
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
    
    handobj * GetRightHand()
    {
        return &m_RightHand;
    }
    
    handobj * GetLeftHand()
    {
        return &m_LeftHand;
    }
    
    handobj m_RightHand;
    handobj m_LeftHand;

};
#endif