#ifndef SCRIPT_ENTITY_H_
#define SCRIPT_ENTITY_H_


#include <unordered_map>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>
#include <cassert>
#include <iostream>
#include "loghelper.h"

enum class EntityType { UNKNOWN, ROOM, ITEM, NPC, PLAYER, COMMAND, DAEMON, LIB };

struct _sol_userdata_
{
    ~_sol_userdata_()
    {
       // LOG_DEBUG << "sol_userdata destroyed.." << std::endl;
    }
    sol::userdata selfobj;
};

/*
struct entity_log
{
    static unsigned long hash(std::string& str)
    {
        unsigned long hash = 5381;
        for (size_t i = 0; i < str.size(); ++i)
            hash = 33 * hash + (unsigned char)str[i];
        return hash;
    }
    
    entity_log(std::string& logpath, EntityType& et)
    {
       // std::string roomLog = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH) +
       //                   global_settings::Instance().GetSetting(DEFAULT_LOGS_PATH) +
       //                   global_settings::Instance().GetSetting(DEFAULT_ROOM_LOG_PATH);
                          
        entityLogFileAppender.reset( new plog::RollingFileAppender<plog::MyFormatter>(logpath.c_str(), 8000, 3));
        entityConsoleAppender.reset( new plog::ConsoleAppender<plog::MyFormatter>());
        virtualEntityAppender.reset( new plog::MyAppender<plog::MyFormatter>());
        int logid = hash(logpath);
        if( et == EntityType::ROOM )
            plog::init<logid> (plog::debug, entityLogFileAppender.get()).addAppender(entityConsoleAppender.get()).addAppender(virtualEntityAppender.get()); 
            // TODO:  Add in other log types
    }
    
private:
    std::shared_ptr<plog::RollingFileAppender<plog::MyFormatter>> entityLogFileAppender;//(roomLog.c_str(), 8000, 3);
    std::shared_ptr<plog::ConsoleAppender<plog::MyFormatter>> entityConsoleAppender; // Create the 2nd appender.
    std::shared_ptr<plog::MyAppender<plog::MyFormatter>> virtualEntityAppender;   // Create our custom appender.
};
*/

struct script_entity {
    
        script_entity(EntityType myType, std::string name) : m_type(myType), environment_(NULL), name(name)
        {
            
        }
        script_entity(sol::this_state ts, EntityType myType, std::string name);
        
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
        void SetScriptPath(std::string& path) ;
        
        std::string GetInstancePath()
        {
            return script_path + ":id=" + std::to_string(instanceID);
        }
        
        unsigned int instanceID = 0;
        
        std::shared_ptr< _sol_userdata_ > m_userdata;
        
        virtual void debug( sol::this_state ts, const std::string& msg );
        

        const std::string& GetName()
        {
            return name;
        }
        
        void SetName(const std::string& name)
        {
            this->name = name;
        }
        
private:
        my_sink m_entityLog;
        std::unordered_map<std::string, sol::object> props;
        EntityType m_type = EntityType::UNKNOWN;
        script_entity* environment_;
        std::string script_path;
  
protected:
        std::string name;
};


#endif