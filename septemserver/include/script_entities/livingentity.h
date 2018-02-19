// ==========================================================================
// livingentity.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================
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



struct inventory_slot
{
    
    inventory_slot()
    {
        
    }

    inventory_slot( InventorySlot slot, unsigned int maxItems,  ItemSize maxItemSize ) :
        m_slot(slot), m_maxItems(maxItems), m_maxSize(maxItemSize)
    {
        
    }
    
    virtual ~inventory_slot()
    {
        
    }

    InventorySlot get_inventorySlot()
    {
        return m_slot;
    }
    
    unsigned int get_maxItems()
    {
        return m_maxItems;
    }
    
    ItemSize get_maxItemSize()
    {
        return m_maxSize;
    }
    
    void set_maxItems( unsigned int i )
    {
        m_maxItems = i;
    }
    
    void set_itemCount( unsigned int i )
    {
        m_itemCount = i;
    }
    
    unsigned int get_itemCount()
    {
        return m_itemCount;
    }
    
    void incrementItemCount()
    {
        m_itemCount++;
    }
    
private:
    InventorySlot m_slot;
    unsigned int m_maxItems = 1; // default is 1 item
    ItemSize m_maxSize = ItemSize::COLOSSAL; // biggest item that can fit
    unsigned int m_itemCount = 0;
};

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
            return m_livingEntity->do_save();
        return false;
    }
    
    
    script_entity * m_livingEntity = NULL;
    
};

enum EntityGender { MALE, FEMALE, UNKNOWN };
enum EntityBodyPosition { PRONE, SITTING, KNEELING, STANDING };
enum InventorySlotError { SLOT_OK, NO_SLOT, ITEM_COUNT_EXCEEDED, ITEM_SIZE_EXCEEDED };
struct living_entity : public script_entity, public container_base
{
    living_entity()
    {
        m_RightHand.m_livingEntity = this;
        m_LeftHand.m_livingEntity = this;
        load_default_command_directories();
        //env = NULL;
    }

    living_entity(sol::this_state ts, sol::this_environment te, EntityType et, std::string name)
        : script_entity(ts, te, et, name)
    {
        m_RightHand.m_livingEntity = this;
        m_LeftHand.m_livingEntity = this;
        load_default_command_directories();
      //  env = NULL;
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
    
    virtual InventorySlotError SafeAddItemToInventory( itemobj* io );
    
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
    
    EntityBodyPosition get_body_position()
    {
        return m_bodyPosition;
    }
    
    void set_body_position( EntityBodyPosition g )
    {
        m_bodyPosition = g;
    }
    
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override
    {
        
    }
    
    virtual void debug(const std::string& msg) override
    {
        
    }
    
    bool AddInventorySlot( InventorySlot slot, unsigned int maxItems=1, ItemSize maxItemSize = ItemSize::COLOSSAL );
    
    //itemobj * CheckInventorySlot( InventorySlot slot );
    
    std::vector<inventory_slot*> GetInventorySlots();
    
    std::vector<itemobj*>  GetItemsInInventorySlot( InventorySlot s );
    
    
    
    void unload_inventory_from_game();
    
    std::vector<std::string>& GetCommandDirs() { return m_command_directories; }
    
private:
    EntityGender m_gender = EntityGender::UNKNOWN;
    EntityBodyPosition m_bodyPosition = EntityBodyPosition::STANDING;
    
    bool is_stunned;
    
    std::map < InventorySlot, std::shared_ptr<inventory_slot>> m_inventory_slots;
    
    void recursive_unload( script_entity* se );
    
protected:
    // Loads paths to default allowed commands.
    virtual void load_default_command_directories();
    
    std::vector<std::string> m_command_directories;

};
#endif