#ifndef SCRIPT_ENTITY_H_
#define SCRIPT_ENTITY_H_


#include <unordered_map>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>
#include <cassert>
#include <iostream>
#include <plog/Log.h>

enum class EntityType { UNKNOWN, ROOM, ITEM, NPC, PLAYER, COMMAND, DAEMON };

struct _sol_userdata_
{
    ~_sol_userdata_()
    {
       // LOG_DEBUG << "sol_userdata destroyed.." << std::endl;
    }
    sol::userdata selfobj;
};

struct script_entity {
    
        script_entity(EntityType myType) : m_type(myType), environment_(NULL)
        {
            
        }
        script_entity(sol::this_state ts, EntityType myType);
        
        ~script_entity();

        sol::object get_property_lua(const char* name, sol::this_state s)
        {
               // auto& var = props[name];
                //return sol::make_object(s, var);
                return props[name];
        }

        void set_property_lua(const char* name, sol::stack_object object)
        {
                props[name] = object.as<sol::object>();
        }
        
        void clear_props() 
        { 
            m_userdata.reset();
            props.clear(); 
        }
        
        
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
        
        std::string GetEntityTypeString()
        {
            switch(m_type) {
            case EntityType::UNKNOWN:
                return "unknown";
            case EntityType::ROOM:
                return "room";
            case EntityType::ITEM:
                return "item";
            case EntityType::NPC:
                return "npc";
            case EntityType::PLAYER:
                return "player";
            default:
                return "unknown";
                break;
            }
        }
        
        std::string GetScriptPath() { return script_path; }
        void SetScriptPath(std::string& path) { script_path = path; }
        
        std::string GetInstancePath()
        {
            return script_path + ":id=" + std::to_string(instanceID);
        }
        
        unsigned int instanceID = 0;
        
        std::shared_ptr< _sol_userdata_ > m_userdata;
private:
        std::unordered_map<std::string, sol::object> props;
       // sol::userdata selfobj;
        EntityType m_type = EntityType::UNKNOWN;
        script_entity* environment_;
        std::string script_path;
        
        //sol::userdata selfobj;
        
        
};


#endif