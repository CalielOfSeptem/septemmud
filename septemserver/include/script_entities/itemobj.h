#ifndef ITEM_OBJ_H_
#define ITEM_OBJ_H_
 
#include <memory>
#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"
#include <stdio.h>
#include "string_utils.h"
#include <boost/algorithm/string.hpp>
// These match the lua defines, do not change without also updating defines..
enum class ItemSize { FINE=0, DIMINUTIVE, TINY, SMALL, MEDIUM, LARGE, ENORMOUS, GARGANTUAN, COLOSSAL };
enum class ItemType { DEFAULT=0, ARMOR, PLANT, INSTRUMENT, TOOL, MELEE, RANGED, THROWN, SKIN, AMMO };


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
     
    virtual std::string get_uid()
    {
        return uid;
    }
     
    virtual void set_uid(std::string id)
    {
        uid = id;
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
    
    virtual const std::string& get_itemPluralNoun()
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
    std::string itemNoun;
    std::string itemArticle;
    std::string itemAdjectives;
};
 
#endif