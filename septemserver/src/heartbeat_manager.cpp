#include "heartbeat_manager.h"
#include "entity_wrapper.h"
#include "entity_manager.h"
#include "script_entities/roomobj.h"
#include <boost/algorithm/string.hpp>

void heartbeat_manager::do_heartbeats()
{
    for(auto& kv : bindings) {
        try {

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
        } catch(std::exception& ex) {
            std::cout << ex.what();
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
