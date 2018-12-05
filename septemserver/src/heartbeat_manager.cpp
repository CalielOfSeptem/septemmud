#include "stdafx.h"
#include "heartbeat_manager.h"
#include "entity_wrapper.h"
#include "entity_manager.h"
#include "script_entities/roomobj.h"

#include "security_context.hpp"
heartbeat_manager::heartbeat_manager()
{
}
void heartbeat_manager::do_heartbeats()
{
    for(auto& kv : bindings) {
        try {

            // the bound script path
            sol::optional<std::string> spath = kv.second.script_path; // lua_state["_INTERNAL_SCRIPT_PATH_"];
            // the entity type that registered for HBs
            sol::optional<std::string> sent = kv.second.etype; // lua_state["_INTERNAL_ENTITY_TYPE_"];
            script_entity* se = NULL;
            // check if the entity is a room..
            // TODO: add additional entity types such as items, NPCS, etc..
            if(sent && boost::to_lower_copy(sent.value()) == "room") {
                roomobj* re = entity_manager::Instance().GetRoomByScriptPath(spath.value(), 0);
                se = re;
            }

            if(se == NULL) {

                std::stringstream ss;
                ss << "Error. Heartbeats are limited to room entities only at this time. Path = " << spath.value()
                   << " type = " << sent.value();
                log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
                break;
            }
            security_context::Instance().SetCurrentEntity(se);
            entity_manager::Instance().register_hook(se); // register the debug hook to detect infinite loop

            auto result = kv.second.pf();
            if(!result.valid()) {
                sol::error err = result;

                std::stringstream ss;
                ss << "Error attempting to perform hearbeat: " << err.what() << ", Path = " << spath.value()
                   << " type = " << sent.value();
                log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
                se->debug(ss.str());
                // std::cout << err.what() << std::endl;
            }

            entity_manager::Instance().register_hook(NULL);
            /*

            kv.second.lua_state.set_function("debug_hook", [&kv](sol::object o) -> void {
                sol::optional<std::string> spath = kv.second.script_path; // lua_state["_INTERNAL_SCRIPT_PATH_"];
                sol::optional<std::string> sent = kv.second.etype; // lua_state["_INTERNAL_ENTITY_TYPE_"];
                script_entity* se = NULL;
                if(sent && boost::to_lower_copy(sent.value()) == "room") {
                    roomobj* re = entity_manager::Instance().GetRoomByScriptPath(spath.value(), 0);
                    se = re;
                }

                sol::optional<std::string> err = o.as<std::string>();
                if(err && err.value() == "count") {
                    std::string s = "Infinite loop detected. Operation terminated.";
                    if(se) {
                        // sol::this_state sta;
                        /// sta.L = kv.second.lua_state.lua_state();
                        se->debug(s);
                    }
                    lua_pushstring(kv.second.lua_state.lua_state(), s.c_str());
                    lua_error(kv.second.lua_state.lua_state());
                } else {
                    std::string s = "Error. Reason = " + err.value();
                    if(se) {
                        // sol::this_state sta;
                        // sta.L = kv.second.lua_state.lua_state();
                        // rooms[0]->script_ent->Debug(sta, s);
                        se->debug(s);
                    }
                    lua_pushstring(kv.second.lua_state.lua_state(), s.c_str());
                    lua_error(kv.second.lua_state.lua_state());
                }

            });

            kv.second.lua_state.script("debug.sethook (debug_hook, '', 10000)");
            auto result = kv.second.pf();
            if(!result.valid()) {
                sol::error err = result;
                std::cout << err.what() << std::endl;
            }
            kv.second.lua_state.script("debug.sethook ()");
            */
        } catch(std::exception& ex) {
            // std::cout << ex.what();
            std::stringstream ss;
            ss << "Error attempting to perform heartbeats: " << ex.what();
            log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
        }
    }
}

void lua_stacktrace_ex(lua_State* L)
{

    lua_Debug entry;
    int depth = 1;

    while(lua_getstack(L, depth, &entry)) {
        lua_Debug info;
        // Level 0 means current function (this C function, which is useless for our purposes)
        // Level 1 means nect call frame up the stack. This is probably the environment we're looking for?
        //?int level = 1;
        int pre_stack_size = lua_gettop(L);
        if(lua_getstack(L, depth, &info) != 1) {
            // failure: call it quits
            std::cout << "error: unable to traverse the stack" << std::endl;
            lua_settop(L, pre_stack_size);
            return;
        }
        // the "f" identifier is the most important here
        // it pushes the function running at `level` onto the stack:
        // we can get the environment from this
        // the rest is for printing / debugging purposes
        if(lua_getinfo(L, "fnluS", &info) == 0) {
            // failure?
            std::cout << "error: unable to get stack information" << std::endl;
            lua_settop(L, pre_stack_size);
            return;
        }

        // Okay, so all the calls worked.
        // Print out some information about this "level"
        std::cout << "[" << depth << "] " << info.short_src << ":" << info.currentline << " -- "
                  << (info.name ? info.name : "<unknown>") << "[" << info.what << "]" << std::endl;

        // Grab the function off the top of the stack
        // remember: -1 means top, -2 means 1 below the top, and so on...
        // 1 means the very bottom of the stack,
        // 2 means 1 more up, and so on to the top value...
        sol::function f(L, -1);
        // The environment can now be ripped out of the function
        sol::environment env(sol::env_key, f);
        if(!env.valid()) {
            std::cout << "error: no environment to get" << std::endl;
            lua_settop(L, pre_stack_size);
            return;
        }
        sol::state_view lua(L);
        sol::optional<std::string> s = env["_INTERNAL_SCRIPT_PATH_"];
        if(s)
            std::cout << "ENV VARIABLE LOCATED, ENV = " << s.value() << std::endl;
        // sol::environment freshenv = lua["cmds_env_"]["move_env_"];
        // bool is_same_env = freshenv == env;
        // std::cout << "env == freshenv : " << is_same_env << std::endl;
        depth++;
    }
}

unsigned int heartbeat_manager::register_heartbeat_func_on(sol::this_state ts, sol::object f)
{
    lua_State* L = ts;
    lua_stacktrace_ex(L);

    // return 1;
    sol::type t = f.get_type();
    unsigned int ret = -1;
    switch(t) {
    case sol::type::function: {
        ret = register_heartbeat_func(sol::state_view(ts), f);

        break;
    }
    case sol::type::string: {
        sol::protected_function mp = sol::state_view(ts)[f.as<std::string>()];
        ret = register_heartbeat_func(sol::state_view(ts), mp);
    } break;
    case sol::type::table: {

    } break;
    case sol::type::userdata: {

    } break;
    default:
        // std::cout << "";
        break;
    }
    return ret;
}

unsigned int heartbeat_manager::register_heartbeat_func(sol::state_view lua_state, sol::protected_function pf)
{
    sol::environment target_env(sol::env_key, pf);
    sol::optional<std::string> script_path = target_env["_INTERNAL_SCRIPT_PATH_"];
    // sol::optional<std::string> script_path = target_env["_INTERNAL_PHYSICAL_SCRIPT_PATH_"];
    sol::optional<std::string> e_type = target_env["_INTERNAL_ENTITY_TYPE_"];

    if(script_path) {
        std::cout << script_path.value() << std::endl;
        if(script_path.value().empty())
            return -1; // yeah.. don't go trying to use an empty value
                       // std::cout << script_path.value();
        bindings.insert({ ++bindingId, state_wrapper(lua_state, pf, script_path.value(), e_type.value()) });
    } else {
        return -1;
    }

    return bindingId;
}



bool heartbeat_manager::deregister_heartbeat_func(int func_id)
{
    if(bindings.erase(func_id) == 1)
        return true;
    else
        return false;
}

bool heartbeat_manager::deregister_all_heartbeat_funcs()
{
    bindings.clear();
    return true;
}

bool heartbeat_manager::clear_heartbeat_funcs(std::string script_path)
{
    for(auto it = begin(bindings); it != end(bindings);) {
        if(it->second.script_path.compare(script_path) == 0) {

            it = bindings.erase(it); // previously this was something like bindings.erase(it++);
        } else
            ++it;
    }
    return true;
}
