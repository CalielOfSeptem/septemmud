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
#include "entity_constants.h"
#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"
#include "script_entities/handobj.h"
#include "script_entities/inventory_slot.h"
#include "script_entities/itemobj.h"

class roomobj;
struct living_entity;
struct itemobj;

enum class EntityGender { MALE, FEMALE, UNKNOWN };
enum class EntityBodyPosition { PRONE, SITTING, KNEELING, STANDING };

struct living_entity : public script_entity, public container_base
{
    living_entity();

    living_entity(sol::this_state ts, sol::this_environment te, EntityType et, std::string name);
    
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
    ;
    
    std::vector< itemobj *> GetItems();
    handobj m_RightHand;
    handobj m_LeftHand;
    
    
    EntityGender get_gender()
    ;
    
    void set_gender( EntityGender g )
    ;
    
    EntityBodyPosition get_body_position()
    ;
    
    void set_body_position( EntityBodyPosition g )
    ;
    
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