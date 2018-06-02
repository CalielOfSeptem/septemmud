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


#include "state_wrapper.hpp"


struct heartbeat_manager
{
    heartbeat_manager();
    
    unsigned int register_heartbeat_func(sol::state_view lua_state, sol::protected_function pf);
    
    bool deregister_heartbeat_func(int func_id);
    
    bool deregister_all_heartbeat_funcs();
    
    bool clear_heartbeat_funcs(std::string script_path);
    
    void do_heartbeats();
 

    unsigned int register_heartbeat_func_on(sol::this_state ts, sol::object f);
    
private:
    std::unordered_map< int, state_wrapper > bindings;
    unsigned int bindingId;
};
#endif