#ifndef HANDOBJ_H_
#define HANDOBJ_H_

#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"


struct itemobj;
struct handobj : public container_base, public script_entity
{
     handobj();
     virtual bool AddEntityToInventory(script_entity * se) override;
     
     virtual bool RemoveEntityFromInventoryByID( const std::string& id  ) override;
     
    virtual bool RemoveEntityFromInventory( script_entity * se ) override;
    
    virtual bool AddItem(itemobj * i);

    virtual bool RemoveItem( itemobj * i );
    
    
    bool IsEmpty();
    
    itemobj * GetItem();
    
    virtual script_entity * GetOwner() override;
    
    virtual bool do_save() override;
    
    script_entity * m_livingEntity = NULL;
    
};

#endif