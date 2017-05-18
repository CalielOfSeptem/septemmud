#ifndef ITEM_OBJ_H_
#define ITEM_OBJ_H_
 
#include <memory>
#include "script_entities/script_entity.h"
#include "script_entities/container_base.h"
#include <stdio.h>

enum class ItemSize { FINE, DIMINUTIVE, TINY, SMALL, MEDIUM, LARGE, GARGANTUAN, COLOSSAL };

struct itemobj : public script_entity, public container_base
{
     itemobj(sol::this_state ts, sol::this_environment te, std::string name) :
        script_entity(ts, te, EntityType::ITEM, name)
     {
        
     }
     
     
     double get_weight ()
     {
         return m_weight;
     }
     void set_weight(double d)
     {
         m_weight = d;
     }
     
     ItemSize get_size()
     {
         return m_size;
     }
     
     void set_size(ItemSize is)
     {
         m_size = is;
     }
     
     bool get_isWearable()
     {
         return bisWearable;
     }
     
     void set_isWearable(bool b)
     {
         bisWearable = b;
     }
     
     bool get_isStackable()
     {
         return bisStackable;
     }
     
     void set_isStackable(bool b)
     {
         bisStackable = b;
     }
     
     bool get_isContainer()
     {
         return bisContainer;
     }
     
     void set_isContainer(bool b)
     {
         bisContainer = b;
     }
     
     bool get_uid()
     {
         return uid;
     }
     
     void set_uid(std::string id)
     {
         uid = id;
     }
     
     double m_weight; // item weight in stones
     ItemSize m_size = ItemSize::TINY;
     bool bisWearable;
     bool bisStackable;
     bool bisContainer;
     std::string uid;
};
 
#endif