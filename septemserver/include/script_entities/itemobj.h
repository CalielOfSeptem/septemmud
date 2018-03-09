// ==========================================================================
// itemobj.h
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
#ifndef ITEM_OBJ_H_
#define ITEM_OBJ_H_
 
#include "entity_constants.h"
#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"

    // These match the lua defines, do not change without also updating defines..
enum class ItemSize { FINE=0, DIMINUTIVE, TINY, SMALL, MEDIUM, LARGE, ENORMOUS, GARGANTUAN, COLOSSAL };
enum class ItemType { DEFAULT=0, ARMOR, PLANT, INSTRUMENT, TOOL, MELEE, RANGED, THROWN, SKIN, AMMO, WEARABLE };
enum class ItemMaterial { UNKNOWN=0, METAL, WOOD, FIBER, CLOTH, STONE };

using json = nlohmann::json;


struct itemobj : public script_entity, public container_base
{
    itemobj(sol::this_state ts, sol::this_environment te, std::string name, ItemType itype);
    itemobj();
    
    //itemobj(sol::this_state ts, sol::this_environment te, std::string name, std::string article, ItemType itype);
    
    virtual script_entity * GetOwner() override;

     
    virtual double get_weight ();
    
    virtual void set_weight(double d);
     
    virtual ItemSize get_size();
     
    virtual void set_size(ItemSize is);
     
    virtual bool get_isWearable();
     
    virtual bool get_isStackable();
        
    virtual bool get_isContainer();
     
    virtual void set_isContainer(bool b);
    
    
    virtual bool get_isLockable();
     
    virtual void set_isLockable(bool b);
    
    virtual bool get_isCloseable();
     
    virtual void set_isCloseable(bool b);
    
    virtual bool get_isOpen();
     
    virtual void set_isOpen(bool b);
    
    virtual bool get_isLocked();
     
    virtual void set_isLocked(bool b);
    
    
    
    virtual bool get_isInitialized();
     
    virtual void set_isInitialized(bool b);
     
    
    virtual std::string get_itemArticle();
     
    virtual void set_itemArticle(std::string article);
    
    virtual std::string get_itemNoun();
     
    virtual void set_itemNoun(std::string noun);
    
    virtual ItemType get_itemType();
     
    virtual void set_itemType(ItemType t);
    
    virtual std::string get_itemAdjectives();
     
    virtual void set_itemAdjectives(std::string adjectives);
    
    
    virtual const std::string& get_pluralName();
     
    virtual void set_pluralName(std::string pluralName);
    
    virtual std::string get_itemPluralNoun();
    
    
    virtual void SetName(const std::string& name) override;
     
    virtual const std::string& GetName() override;

    virtual int get_defaultStackSize();
     
    virtual void set_defaultStackSize(int stackSize);
     
    virtual int get_currentStackCount();
     
    virtual void set_currentStackCount(int stackCount);
    
    virtual int get_inventorySlot();
     
    virtual void set_inventorySlot(int slot);
    

     
    virtual void incrementStackCount();

    virtual void decrementStackCount();
    
    bool SaveProperty(const std::string& name, sol::object ob);
    
    sol::object GetProperty( const std::string& name, sol::this_state L );
    
    virtual void debug(const std::string& msg) override;
    
    std::string Serialize();
    
    virtual bool do_save() override;
    
    virtual bool do_load() override;
    
    virtual bool do_json_load( std::string& j) override;
    
    bool _load_from_json_(json& j);
    
    virtual bool remove_settings_file();
    
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override;

    virtual bool AddEntityToInventory(script_entity * se) override;
    virtual bool RemoveEntityFromInventory(script_entity * se) override;
    
    std::vector< itemobj *> GetItems();
     
    double m_weight; // item weight in stones
    ItemSize m_size = ItemSize::TINY;
    ItemType m_type = ItemType::DEFAULT;
    int m_defaultStackSize = 0;
    int m_currentStackCount = 1;
    
    int m_slot_mask = 0;
    

    bool bisContainer = false;
    bool bisInitialized = false;
    
    bool bisLockable = false;
    bool bisCloseable = false;
    bool bisOpen = true;
    bool bisLocked = false;
    

    std::string pluralName;
    std::string itemNoun;
    std::string itemArticle;
    std::string itemAdjectives;

};
 
#endif