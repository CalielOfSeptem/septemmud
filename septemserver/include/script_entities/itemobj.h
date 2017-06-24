#ifndef ITEM_OBJ_H_
#define ITEM_OBJ_H_
 
#include <memory>
#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"
#include <stdio.h>
#include "string_utils.h"
#include <boost/algorithm/string.hpp>


#include "json.hpp"

using json = nlohmann::json;

// These match the lua defines, do not change without also updating defines..
enum class ItemSize { FINE=0, DIMINUTIVE, TINY, SMALL, MEDIUM, LARGE, ENORMOUS, GARGANTUAN, COLOSSAL };
enum class ItemType { DEFAULT=0, ARMOR, PLANT, INSTRUMENT, TOOL, MELEE, RANGED, THROWN, SKIN, AMMO, WEARABLE };


enum InventorySlot {
        NONE                        = 0,
        BODY                        = 1<<0,
        BACK                        = 1<<1,
        WAIST                       = 1<<2,
        LIKE_HEAD_ARMOR             = 1<<3,
        OVER_SHOULDER               = 1<<4,
        LIKE_PANTS                  = 1<<5,
        LIKE_SHIRT                  = 1<<6,
        ON_WRIST                    = 1<<7,
        ON_FINGER                   = 1<<8,
        ON_FEET                     = 1<<9,
        NECK                        = 1<<10,
        BELT                        = 1<<11,
        LIKE_ARM_ARMOR              = 1<<12,
        LIKE_LEG_ARMOR              = 1<<13,
        ON_AN_EAR                   = 1<<14,
        ON_BOTH_EARS                = 1<<15,
        ANKLE                       = 1<<16,
        HANDS                       = 1<<17,
        TAIL                        = 1<<18,
        THIGH                       = 1<<19,
        UPPER_ARM                   = 1<<20,
        NOSE                        = 1<<21,
        OVER_LEFT_EYE               = 1<<22,
        OVER_RIGHT_EYE              = 1<<23,
        ON_HAIR                     = 1<<24,
        IN_HAIR                     = 1<<25,
        HEAD                        = 1<<26
    };


struct itemobj : public script_entity, public container_base
{
    itemobj(sol::this_state ts, sol::this_environment te, std::string name, ItemType itype);
    
    //itemobj(sol::this_state ts, sol::this_environment te, std::string name, std::string article, ItemType itype);
    
    virtual script_entity * GetOwner() override
    {
        return this;
    }

     
    virtual double get_weight ()
    {
        return m_weight;
    }
    
    virtual void set_weight(double d)
    {
        m_weight = d;
    }
     
    virtual ItemSize get_size()
    {
        return m_size;
    }
     
    virtual void set_size(ItemSize is)
    {
        m_size = is;
    }
     
    virtual bool get_isWearable()
    {
        return bisWearable;
    }
     
    virtual void set_isWearable(bool b)
    {
        bisWearable = b;
    }
     
    virtual bool get_isStackable()
    {
        return bisStackable;
    }
     
    virtual void set_isStackable(bool b)
    {
        bisStackable = b;
    }
     
    virtual bool get_isContainer()
    {
        return bisContainer;
    }
     
    virtual void set_isContainer(bool b)
    {
        bisContainer = b;
    }
    
    
    virtual bool get_isLockable()
    {
        return bisLockable;
    }
     
    virtual void set_isLockable(bool b)
    {
        bisLockable = b;
    }
    
    virtual bool get_isCloseable()
    {
        return bisCloseable;
    }
     
    virtual void set_isCloseable(bool b)
    {
        bisCloseable = b;
    }
    
    virtual bool get_isOpen()
    {
        return bisOpen;
    }
     
    virtual void set_isOpen(bool b)
    {
        bisOpen = b;
    }
    
    virtual bool get_isLocked()
    {
        return bisLocked;
    }
     
    virtual void set_isLocked(bool b)
    {
        bisLocked = b;
    }
    
    
    
    virtual bool get_isInitialized()
    {
        return bisInitialized;
    }
     
    virtual void set_isInitialized(bool b)
    {
        bisInitialized = b;
    }
     
    
    virtual std::string get_itemArticle()
    {
        return itemArticle;
    }
     
    virtual void set_itemArticle(std::string article)
    {
        itemArticle = article;
    }
    
    virtual std::string get_itemNoun()
    {
        return itemNoun;
    }
     
    virtual void set_itemNoun(std::string noun)
    {
        itemNoun = noun;
    }
    
    virtual ItemType get_itemType()
    {
        return m_type;
    }
     
    virtual void set_itemType(ItemType t)
    {
        m_type = t;
    }
    
    virtual std::string get_itemAdjectives()
    {
        return itemAdjectives;
    }
     
    virtual void set_itemAdjectives(std::string adjectives)
    {
        itemAdjectives = adjectives;
    }
    
    
    virtual const std::string& get_pluralName()
    {
        return pluralName;
    }
     
    virtual void set_pluralName(std::string pluralName)
    {
        this->pluralName = pluralName;
    }
    
    virtual std::string get_itemPluralNoun()
    {
        std::vector<std::string> parts = split(pluralName, ' ');
        if( parts.size() == 0 )
            return "";
        return parts[parts.size()-1];
    }
    
    
    virtual void SetName(const std::string& name) override
    {
        std::vector<std::string> parts = split(name, ' ');
        assert( parts.size() !=  0);
        
        // the first part should be 'a' or 'an'
        std::string article = boost::to_lower_copy(parts[0]);
        if( article != "a" && article != "an" )
        {
            // we're going to just add an article, it may have been omitted on accident..
            this->set_itemArticle("a");
        }
        else
        {
            this->set_itemArticle(article);
        }
        // now we need to know if there are adjectives..
        // adjectives always follow the article
        if( parts.size() == 2 )
        {
            // no adjectives.. pull the noun out
            this->set_itemNoun(parts[1]);
        }
        else
        {
            for( unsigned int x = 1; x < parts.size()-1; x++ )
            {
                this->itemAdjectives = this->itemAdjectives + parts[x] + " ";
            }
            trim(this->itemAdjectives); // remove extra white spaces..
            this->set_itemNoun(parts[parts.size()-1]);
        }
        script_entity::SetName(name);
    }
     
    virtual const std::string& GetName() override
    {
        if( this->get_isStackable() && this->get_currentStackCount() > 1 )
        {
            return this->get_pluralName();
        }
        else
        {
            return script_entity::GetName();
        }
        
    }

    virtual int get_defaultStackSize()
    {
        return this->m_defaultStackSize;
    }
     
    virtual void set_defaultStackSize(int stackSize)
    {
        m_defaultStackSize = stackSize;
    }
     
    virtual int get_currentStackCount()
    {
        return this->m_currentStackCount;
    }
     
    virtual void set_currentStackCount(int stackCount)
    {
        m_currentStackCount = stackCount;
    }
    
    virtual int get_inventorySlot()
    {
        return this->m_slot_mask;
    }
     
    virtual void set_inventorySlot(int slot)
    {
        m_slot_mask = slot;
    }
    

     
    virtual void incrementStackCount()
    {
        m_currentStackCount++;     
    }

    virtual void decrementStackCount()
    {
        if( m_currentStackCount == 1 )
        {
            return;
        }
        else
        {
            m_currentStackCount--;
        }
    }
    
    virtual void debug(const std::string& msg) override
    {
       // if( this->isCreator() )
        //    SendToEntity(msg);
        if( this->GetEnvironment() )
        {
            this->GetEnvironment()->debug(msg);
        }
    }
    
    std::string Serialize();
    
    virtual bool do_save() override;
    
    virtual bool do_load() override;
    
    virtual bool do_json_load( std::string& j) override;
    
    bool _load_from_json_(json& j);
    
    virtual bool remove_settings_file();
    
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override;
    
   // virtual void SetEnvironment(script_entity* be) override;

    virtual bool AddEntityToInventory(script_entity * se) override;
    virtual bool RemoveEntityFromInventory(script_entity * se) override;
    
    std::vector< itemobj *> GetItems();
     
    double m_weight; // item weight in stones
    ItemSize m_size = ItemSize::TINY;
    ItemType m_type = ItemType::DEFAULT;
    int m_defaultStackSize = 10;
    int m_currentStackCount = 1;
    
    int m_slot_mask = 0;
    
    bool bisWearable = false;
    bool bisStackable = false;
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