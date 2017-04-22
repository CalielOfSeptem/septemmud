#ifndef SCRIPT_ENTITY_H_
#define SCRIPT_ENTITY_H_

#include <sol.hpp>
#include <unordered_map>
#define SOL_CHECK_ARGUMENTS

#include <cassert>
#include <iostream>

enum class EntityType { UNKNOWN, ROOM, ITEM, NPC, PLAYER, COMMAND, DAEMON };

struct script_entity {
  
        script_entity(sol::this_state ts, EntityType myType) {
            m_type = myType;
			lua_State* L = ts;
			// references the object that called this function
			// in constructors:
			selfobj = sol::userdata(L, -1);
            
			// the -1 (NEGATIVE one) above 
			// means "off the top fo the stack"
			// (-1 is the top, -2 is one below, etc...)

			// definitely the same
			script_entity& self = selfobj.as<script_entity>();
			assert(&self == this);
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

        std::unordered_map<std::string, sol::object> props;
        sol::userdata selfobj;
        EntityType m_type;
};


#endif