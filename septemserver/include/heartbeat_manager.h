#ifndef HEARTBEAT_MANAGER_H_
#define HEARTBEAT_MANAGER_H_

#include <sol.hpp>

#include <iostream>
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
    
    struct state_wrapper
    {
        state_wrapper(sol::state_view lua_state, sol::protected_function pf, std::string script_path, std::string etype) :
             lua_state(lua_state), pf(pf), script_path(script_path), etype(etype)
            {
                
            }
        sol::state_view lua_state;
        sol::protected_function pf;
        std::string script_path;
        std::string etype;
    };
    
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
    {
        //return 1;
        sol::type t = f.get_type();
        unsigned int ret = -1;
        switch (t) {
        case sol::type::function:
        {
            ret = register_heartbeat_func(sol::state_view(ts), f);
           
            break;
        }
        case sol::type::string:
        {
            sol::protected_function mp =  sol::state_view(ts)[f.as<std::string>()];
            ret =register_heartbeat_func(sol::state_view(ts), mp);
        }
        break;
        case sol::type::table: {

        }
        break;
        case sol::type::userdata: {

        }
        break;
        default:
           // std::cout << "";
            break;
        }
        return ret;

    }
    
private:
    std::unordered_map< int, state_wrapper > bindings;
    unsigned int bindingId;
};
#endif