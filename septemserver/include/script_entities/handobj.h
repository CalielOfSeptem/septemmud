#ifndef HANDOBJ_H_
#define HANDOBJ_H_

#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"
struct itemobj;
struct handobj : public container_base, public script_entity
{
     handobj()
     {
         m_type = EntityType::HAND;
         //env = NULL;
         
     }
     virtual bool AddEntityToInventory(script_entity * se) override
     {
         bool b = container_base::AddEntityToInventory(se);
         do_save();
         return b;
     }
     
     virtual bool RemoveEntityFromInventoryByID( const std::string& id  ) override
     {
         return container_base::RemoveEntityFromInventoryByID(id);
     }
     
    virtual bool RemoveEntityFromInventory( script_entity * se ) override
    {
        return container_base::RemoveEntityFromInventory(se);
    }
    
    virtual bool AddItem(itemobj * i)
    ;

    virtual bool RemoveItem( itemobj * i )
    ;
    
    
    bool IsEmpty()
    ;
    
    itemobj * GetItem()
    ;
    
    virtual script_entity * GetOwner() override
    ;
    
    virtual bool do_save() override
    ;
    
    
    script_entity * m_livingEntity = NULL;
    
};

#endif