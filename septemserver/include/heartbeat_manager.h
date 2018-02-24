// ==========================================================================
// heartbeat_manager.h
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
#ifndef HEARTBEAT_MANAGER_H_
#define HEARTBEAT_MANAGER_H_

#include <sol.hpp>

#include <iostream>
#include "state_wrapper.hpp"
//#include <thread>
//#include <future>
//#include <signal.h>
//#include <boost/thread.hpp>
//#include <boost/thread/future.hpp>
//#include <boost/chrono/chrono.hpp>

struct heartbeat_manager
{
    heartbeat_manager()
    {
      
    }
    

    
    unsigned int register_heartbeat_func(sol::state_view lua_state, sol::protected_function pf)
    {
        sol::environment target_env(sol::env_key, pf);
        sol::optional<std::string> script_path = target_env["_INTERNAL_SCRIPT_PATH_"];
        //sol::optional<std::string> script_path = target_env["_INTERNAL_PHYSICAL_SCRIPT_PATH_"];
        sol::optional<std::string> e_type = target_env["_INTERNAL_ENTITY_TYPE_"];
       
        if( script_path )
        {
             std::cout << script_path.value() << std::endl;
            if( script_path.value().empty() )
                return -1; // yeah.. don't go trying to use an empty value
           // std::cout << script_path.value();
            bindings.insert( {++bindingId, state_wrapper (lua_state, pf, script_path.value(), e_type.value())});
        }
        else
        {
            return -1;
        }
        
        return bindingId;
    }
    
    bool deregister_heartbeat_func(int func_id)
    {
        if (bindings.erase(func_id) == 1)
           return true;
        else
            return false;
    }
    
    bool deregister_all_heartbeat_funcs()
    {
        bindings.clear();
        return true;
    }
    
    bool clear_heartbeat_funcs(std::string script_path)
    {
        for(auto it = begin(bindings); it != end(bindings);)
        {
          if (it->second.script_path.compare(script_path) == 0)
          {
            
            it = bindings.erase(it); // previously this was something like bindings.erase(it++);
          }
          else
            ++it;
        }
        return true;
    }
    
    void do_heartbeats();
 

    unsigned int register_heartbeat_func_on(sol::this_state ts, sol::object f)
    ;
    
private:
    std::unordered_map< int, state_wrapper > bindings;
    unsigned int bindingId;
};
#endif