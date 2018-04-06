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


#include "loghelper.h"


class get_uid;
struct extcommandobj;
struct actionobj;


 
struct _sol_userdata_
{
    ~_sol_userdata_()
    {
       // LOG_DEBUG << "sol_userdata destroyed.." << std::endl;
    }
    sol::userdata selfobj;
};


struct script_entity {
    
        script_entity(EntityType myType, std::string name);
        
        script_entity(sol::this_state ts, sol::this_environment te, EntityType myType, std::string name);
        
        script_entity();
        
        ~script_entity();

        sol::object get_property_lua(const char* name, sol::this_state s);

        void set_property_lua(const char* name, sol::stack_object object);
        
        void clear_props();
        
        
        script_entity* GetEnvironment();

        virtual void SetEnvironment(script_entity* be);
        
        
        virtual EntityType& GetType();
        
        std::string GetEntityTypeString();
        
        std::string GetVirtualScriptPath() { return virtual_script_path; }
        void SetVirtualScriptPath(std::string& path) ;
        
        std::string GetPhysicalScriptPath() { return physical_script_path; }
        void SetPhysicalScriptPath(std::string& path) ;
        
        std::string GetInstancePath();
        
        unsigned int instanceID = 0;
        
        std::shared_ptr< _sol_userdata_ > m_userdata;
        
        //virtual void debug_script(const std::string& msg, LogLevel ll);
        
        virtual void debug(const std::string& msg);

        virtual const std::string& GetName();
        
        virtual void SetName(const std::string& name);
        
        const std::string& GetLook();
        
        void SetLook(const std::string& look);
        
        const bool get_destroy();
        
        void set_destroy(bool destroy);
        
        virtual std::string get_uid();
         
        virtual void set_uid(std::string id);
        
        std::string get_entityStorageLocation();
        
        void set_entityStorageLocation(std::string& str);
        
        
        virtual bool do_save();
        
        virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env);
        
        virtual bool do_load();
        
        virtual bool do_json_load( std::string& j );
        
        actionobj * AddAction( sol::this_state ts, sol::protected_function func, sol::object interval, sol::object userData = sol::nil);
        
        void RemoveAction(actionobj * a);
        
        std::vector<std::shared_ptr<actionobj>>& GetActions();
        
        std::vector<std::shared_ptr<extcommandobj>>& GetCommands();
        
        
        void DoActions();
        
        
        extcommandobj * AddCommand(sol::this_state ts, sol::protected_function func, const sol::as_table_t<std::vector<std::string> >& aliases, sol::object userData = sol::nil);
        
		void invoke_on_load();
		void invoke_on_destroy();
private:
        std::string entity_storage_location; // used for persistence
        my_sink m_entityLog;
        std::unordered_map<std::string, sol::object> props;
        
        script_entity* environment_;
        std::string virtual_script_path;
        bool m_destroy = false;
        
		void invoke_on_environment_change();
		
  
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