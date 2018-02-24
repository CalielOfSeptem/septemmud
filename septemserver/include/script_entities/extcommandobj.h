// ==========================================================================
// extcommandobj.h
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
#ifndef EXTCOMMANDOBJ_H_
#define EXTCOMMANDOBJ_H_
#define SOL_CHECK_ARGUMENTS
#include "sol.hpp"
#include "script_entities/livingentity.h"

struct extcommandobj
{
       /**
     * @brief Constructor to allow commands to be instantiated by script entities
     * @param func
     * @param userData
     * @return 
     */
    extcommandobj( sol::state_view lua_state, sol::protected_function func, const std::string& name, sol::object userData = sol::nil ) :
    m_lua_state(lua_state)
    {
        this->func = func;
        this->user_data = userData;
        this->name = name;
    }
    

    ~extcommandobj()
    {
        
    }
    
   void SetCommand(const std::string& name)
    {
        this->name = name;
        
    }
    const std::string& GetCommand()
    {
        return name;
    }
    
    void SetAliases(const sol::as_table_t<std::vector<std::string> >& aliases)
    {
        const auto& vex = aliases.source;
        for(auto& s : vex) {
            this->command_aliases.push_back(s);
        }
    }
    
    void AddAlias(const std::string& alias)
    {
        this->command_aliases.push_back(alias);
    }
    
    const std::vector<std::string>& GetAliases()
    {
        return command_aliases;
    }
    
    
    // Following two functions are for when a command is instantiated by a
    // script entity such as a room or item.
    sol::protected_function GetFunc()
    {
        return func;
    }
    
    bool ExecuteFunc(living_entity * le, const std::string& cmd);
    
    sol::object GetUserData()
    {
        return user_data;
    }
    

   
private:
    std::vector<std::string> command_aliases;
    sol::protected_function func;
    sol::object user_data;
    std::string name;
    sol::state_view m_lua_state;

};

#endif