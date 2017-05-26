#ifndef ITEM_OBJ_H_
#define ITEM_OBJ_H_
 
#include <memory>
#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"
#include <stdio.h>

// These match the lua defines, do not change without also updating defines..
enum class ItemSize { FINE=0, DIMINUTIVE, TINY, SMALL, MEDIUM, LARGE, ENORMOUS, GARGANTUAN, COLOSSAL };
enum class ItemType { DEFAULT=0, ARMOR, PLANT, INSTRUMENT, TOOL, MELEE, RANGED, THROWN, SKIN, AMMO };


struct itemobj : public script_entity, public container_base
{
    itemobj(sol::this_state ts, sol::this_environment te, std::string name, ItemType itype);

    //itemobj(sol::this_state ts, sol::this_environment te);

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
     
    virtual std::string get_uid()
    {
        return uid;
    }
     
    virtual void set_uid(std::string id)
    {
        uid = id;
    }
    
    virtual std::string get_pluralName()
    {
       // if( get_currentStackCount() == 1 )
        //    return name;
        //else return pluralName;
        //return pluralName;
    }
     
    virtual void set_pluralName(std::string s)
    {
        pluralName = s;
    }
    
        
    virtual std::string get_currentName()
    {
        if( get_currentStackCount() == 1 )
            return name;
        else 
            return pluralName;
        //return pluralName;
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
     
    double m_weight; // item weight in stones
    ItemSize m_size = ItemSize::TINY;
    ItemType m_type = ItemType::DEFAULT;
    int m_defaultStackSize = 10;
    int m_currentStackCount = 1;
    bool bisWearable;
    bool bisStackable = false;
    bool bisContainer;
    std::string uid;
    std::string pluralName;
};
 
#endif