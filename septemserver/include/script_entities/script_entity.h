// ==========================================================================
// script_entity.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================
#ifndef SCRIPT_ENTITY_H_
#define SCRIPT_ENTITY_H_


#include <unordered_map>
#define SOL_CHECK_ARGUMENTS
#include <sol.hpp>
#include <cassert>
#include <iostream>
#include "loghelper.h"
#include "script_entities/actionobj.h"


struct extcommandobj;

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
        
        std::string GetVirtualScriptPath() { return virtual_script_path; }
        void SetVirtualScriptPath(std::string& path) ;
        
        std::string GetPhysicalScriptPath() { return physical_script_path; }
        void SetPhysicalScriptPath(std::string& path) ;
        
        std::string GetInstancePath()
        {
            return virtual_script_path + ":id=" + std::to_string(instanceID);
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
        
        actionobj * AddAction( sol::protected_function func, unsigned int interval, sol::object userData = sol::nil)
        {
            actions.push_back(std::shared_ptr<actionobj>( new actionobj(func, interval, userData) ) );
            return actions[actions.size()-1].get();
        }
        
        
        std::vector<std::shared_ptr<actionobj>>& GetActions()
        {
            return actions;
        }
        
        std::vector<std::shared_ptr<extcommandobj>>& GetCommands()
        {
            return commands;
        }
        
        
        void DoActions()
        {
            for( auto r : GetActions() )
            {
                r->DoAction();
            }
        }
        
        
        extcommandobj * AddCommand(sol::this_state ts, sol::protected_function func, const sol::as_table_t<std::vector<std::string> >& aliases, sol::object userData = sol::nil)
        ;
        

private:
        std::string entity_storage_location; // used for persistence
        my_sink m_entityLog;
        std::unordered_map<std::string, sol::object> props;
        
        script_entity* environment_;
        std::string virtual_script_path;
        bool m_destroy = false;
        
  
protected:
        std::map<std::string, std::string> userProps;
        EntityType m_type;// = EntityType::UNKNOWN;
        std::string physical_script_path;
        std::string look;
        std::string name;
        std::string uid;
        std::vector<std::shared_ptr<actionobj>> actions;
        std::vector<std::shared_ptr<extcommandobj>> commands;
};


#endif