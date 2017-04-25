#ifndef SCRIPT_ENTITY_H_
#define SCRIPT_ENTITY_H_


#include <unordered_map>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>
#include <cassert>
#include <iostream>
#include <plog/Log.h>

enum class EntityType { UNKNOWN, ROOM, ITEM, NPC, PLAYER, COMMAND, DAEMON };

struct script_entity {
  
        script_entity(sol::this_state ts, EntityType myType);
        
        ~script_entity()
        {
            LOG_DEBUG  << "Destroyed object.";
        }

        sol::object get_property_lua(const char* name, sol::this_state s)
        {
                auto& var = props[name];
                return sol::make_object(s, var);
        }

        void set_property_lua(const char* name, sol::stack_object object)
        {
                props[name] = object.as<sol::object>();
        }
        
        void clear_props() { props.clear(); }

        std::unordered_map<std::string, sol::object> props;
       // sol::userdata selfobj;
        EntityType m_type;
};


#endif