#ifndef SCRIPT_ENTITY_H_
#define SCRIPT_ENTITY_H_


#include <unordered_map>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>
#include <cassert>
#include <iostream>
#include "loghelper.h"

enum class EntityType { UNKNOWN, ROOM, ITEM, NPC, PLAYER, COMMAND, DAEMON, LIB, HAND };
enum class EnvironmentChangeEvent { ADDED, REMOVED };
enum class LogLevel { INFO, DEBUG, ERROR, CRITICAL };
 
struct _sol_userdata_
{
    ~_sol_userdata_()
    {
       // LOG_DEBUG << "sol_userdata destroyed.." << std::endl;
    }
    sol::userdata selfobj;
};

struct script_entity {
    
        script_entity(EntityType myType, std::string name) : 
            m_type(myType)
            , environment_(NULL)
            , name(name)
        {

        }
        
        script_entity(sol::this_state ts, sol::this_environment te, EntityType myType, std::string name);
        
        script_entity() :
            m_type(EntityType::UNKNOWN)
            , environment_(NULL)
            , name("")
        {
            
        }
        
        ~script_entity();

        sol::object get_property_lua(const char* name, sol::this_state s)
        {
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

        virtual void SetEnvironment(script_entity* be)
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
        void SetScriptPath(std::string& path) ;
        
        std::string GetBaseScriptPath() { return base_script_path; }
        void SetBaseScriptPath(std::string& path) ;
        
        std::string GetInstancePath()
        {
            return script_path + ":id=" + std::to_string(instanceID);
        }
        
        unsigned int instanceID = 0;
        
        std::shared_ptr< _sol_userdata_ > m_userdata;
        
        //virtual void debug_script(const std::string& msg, LogLevel ll);
        
        virtual void debug(const std::string& msg);

        virtual const std::string& GetName()
        {
            return name;
        }
        
        virtual void SetName(const std::string& name)
        {
            this->name = name;
        }
        
        const std::string& GetLook()
        {
            return look;
        }
        
        void SetLook(const std::string& look)
        {
            this->look = look;
        }
        
        const bool get_destroy()
        {
            return m_destroy;
        }
        
        void set_destroy(bool destroy)
        {
            m_destroy = destroy;
            if( destroy )
                clear_props();
        }
        
        virtual std::string get_uid()
        {
            return uid;
        }
         
        virtual void set_uid(std::string id)
        {
            uid = id;
        }
        
        std::string get_entityStorageLocation()
        {
            return entity_storage_location;
        }
        
        void set_entityStorageLocation(std::string& str)
        {
            entity_storage_location = str;
        }
        
        virtual bool do_save()
        {
            return true;
        }
        
        virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env)
        {
            
        }
        
        virtual bool do_load()
        {
            return true;
        }
        
        virtual bool do_json_load( std::string& j )
        {
            return true;
        }
        
private:
        std::string entity_storage_location; // used for persistence
        my_sink m_entityLog;
        std::unordered_map<std::string, sol::object> props;
        
        script_entity* environment_;
        std::string script_path;
        bool m_destroy = false;
        
  
protected:
        EntityType m_type;// = EntityType::UNKNOWN;
        std::string base_script_path;
        std::string look;
        std::string name;
        std::string uid;
};


#endif