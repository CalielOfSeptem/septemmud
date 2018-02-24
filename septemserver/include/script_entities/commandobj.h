// ==========================================================================
// commandobj.h
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
#ifndef COMMANDOBJ_H_
#define COMMANDOBJ_H_
#include "sol.hpp"
#include <string>
#include <vector>
#include "script_entities/script_entity.h"

struct living_entity;

struct commandobj : script_entity
{
    commandobj(sol::this_state ts, sol::this_environment te, std::string name);
    
    commandobj(sol::this_state ts, sol::this_environment te, std::string name, int priority);
    
 
    ~commandobj();
    

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
    
    void SetPriority(int priority)
    {
        this->priority = priority;
    }
    
    int GetPriority() const
    {
        return priority;
    }
    


private:
    std::vector<std::string> command_aliases;
    int priority;


};

#endif