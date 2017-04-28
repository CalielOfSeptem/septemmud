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
    
        script_entity(EntityType myType) : m_type(myType)
        {
            
        }
        script_entity(sol::this_state ts, EntityType myType);
        
        ~script_entity();

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
        
        
        script_entity* GetEnvironment()
        {
            return environment_; //.value();
        }

        void SetEnvironment(script_entity* be)
        {
            environment_ = be;
        }
        
        
        virtual EntityType& GetType()
        {
            return m_type;
        }
        
        std::string GetScriptPath() { return script_path; }
        void SetScriptPath(std::string& path) { script_path = path; }
        
private:
        std::unordered_map<std::string, sol::object> props;
       // sol::userdata selfobj;
        EntityType m_type = EntityType::UNKNOWN;
        script_entity* environment_;
        std::string script_path;
};


#endif