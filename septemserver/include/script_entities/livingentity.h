#ifndef LIVINGENTITY_BASE_H_
#define LIVINGENTITY_BASE_H_

#include <string>
#include <vector>
#include "script_entities/script_entity.h"
#include "script_entities/roomobj.h"
#include "script_entities/itemobj.h"
#include "script_entities/container_base.h"



class roomobj;
struct living_entity;

/*

struct inventory_slot : public container_base, public script_entity
{
    
    inventory_slot()
    {
        
    }

    inventory_slot( std::string slotName, unsigned int maxItems,  ItemSize maxItemSize ) :
        m_alias(slotName), m_maxItems(maxItems), m_maxSize(maxItemSize)
    {
        
    }
    
    virtual ~inventory_slot()
    {
        
    }

    virtual bool AddEntityToInventory(script_entity * se) override
    {
        return container_base::AddEntityToInventory(se);

    }

    virtual bool RemoveEntityFromInventoryByID( const std::string& id  ) override
    {
        return container_base::RemoveEntityFromInventoryByID(id);
    }
     
    virtual bool RemoveEntityFromInventory( script_entity * se ) override
    {
        return container_base::RemoveEntityFromInventory(se);
    }
    
    virtual void AddItem(itemobj * i)
    {
        container_base::AddEntityToInventory( dynamic_cast<script_entity*>(i));

    }

    virtual bool RemoveItem( itemobj * i )
    {
        return RemoveEntityFromInventory( dynamic_cast<script_entity*>(i) );
        //return container_base::RemoveEntityFromInventory(se);
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
    
    std::string & get_alias()
    {
        return m_alias;
    }
    
    void set_alias(std::string& s)
    {
        m_alias = s;
    }
    
    unsigned int get_maxItems()
    {
        return m_maxItems;
    }
    
    void set_maxItems( unsigned int i )
    {
        m_maxItems = i;
    }
    
private:
    std::string m_alias; // human-readable name
    unsigned int m_maxItems = 1; // default is 1 item
    ItemSize m_maxSize = ItemSize::COLOSSAL; // biggest item that can fit
};
*/
struct handobj : public container_base, public script_entity
{
     handobj()
     {
         m_type = EntityType::HAND;
         
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
    {
        bool b = container_base::AddEntityToInventory( dynamic_cast<script_entity*>(i));
        //GetOwner()->do_save();
        do_save();
        return b;

    }

    virtual bool RemoveItem( itemobj * i )
    {
        return RemoveEntityFromInventory( dynamic_cast<script_entity*>(i) );
        //return container_base::RemoveEntityFromInventory(se);
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
    
    virtual bool do_save() override
    {
        if( m_livingEntity != NULL )
            m_livingEntity->do_save();
    }
    
    
    script_entity * m_livingEntity = NULL;
    
};

enum EntityGender { MALE, FEMALE, UNKNOWN };

struct living_entity : public script_entity, public container_base
{
    living_entity()
    {
        m_RightHand.m_livingEntity = this;
        m_LeftHand.m_livingEntity = this;
    }

    living_entity(sol::this_state ts, sol::this_environment te, EntityType et, std::string name)
        : script_entity(ts, te, et, name)
    {
        m_RightHand.m_livingEntity = this;
        m_LeftHand.m_livingEntity = this;
    }
    
    ~living_entity()
    {
        /*
        for( auto kvp : m_inventory_slots )
        {
            delete (kvp.second);
        }
        */
    }

    virtual void SendToEntity(const std::string& msg)
    {
    }

    virtual void SendToEnvironment(const std::string& msg);
    
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
    virtual bool AddEntityToInventory(script_entity * se) override;
    virtual bool RemoveEntityFromInventory(script_entity * se) override;
    
    virtual script_entity * GetOwner() override
    {
        return this;
    }
    
    std::vector< itemobj *> GetItems();
    handobj m_RightHand;
    handobj m_LeftHand;
    
    
    EntityGender get_gender()
    {
        return m_gender;
    }
    
    void set_gender( EntityGender g )
    {
        m_gender = g;
    }
    
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override
    {
        
    }
    
    virtual void debug(const std::string& msg) override
    {
        
    }
  //  bool AddInventorySlot( std::string slotName, unsigned int maxItems=1, ItemSize maxItemSize = ItemSize::COLOSSAL );
    /*
    std::vector<inventory_slot*> GetInventorySlots()
    {
        std::vector< inventory_slot * > slots;
        slots.reserve(m_inventory_slots.size());
        for( auto kvp : m_inventory_slots )
        {
            slots.push_back(kvp.second);
        }
        return slots;
    }
    */
    
private:
    EntityGender m_gender = EntityGender::UNKNOWN;
    //std::map < std::string, inventory_slot* > m_inventory_slots;

};
#endif