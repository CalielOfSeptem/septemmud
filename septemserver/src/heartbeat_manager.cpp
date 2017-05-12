#include "heartbeat_manager.h"
#include "entity_wrapper.h"
#include "entity_manager.h"
#include "script_entities/roomobj.h"
#include <boost/algorithm/string.hpp>

void heartbeat_manager::do_heartbeats()
{
    for(auto& kv : bindings) {
        try {
            //sol::state st = sol::state_view(kv.second.lua_state);
            
            kv.second.lua_state.set_function("debug_hook", [&kv](sol::object o) -> void 
            { 
                sol::optional<std::string> spath = kv.second.script_path;//lua_state["_INTERNAL_SCRIPT_PATH_"];
                sol::optional<std::string> sent = kv.second.etype;//lua_state["_INTERNAL_ENTITY_TYPE_"];
                script_entity * se = NULL;
                if( sent && boost::to_lower_copy(sent.value()) == "room")
                {
                    roomobj * re = entity_manager::Instance().GetRoomByScriptPath(spath.value(), 0);
                    se = re;
                }
                
                sol::optional<std::string> err = o.as<std::string>();
                if( err && err.value() == "count" )
                {
                    std::string s = "Infinite loop detected. Operation terminated.";
                    if( se )
                    {
                        sol::this_state sta;
                        sta.L = kv.second.lua_state.lua_state();
                        //rooms[0]->script_ent->Debug(sta, s);
                        se->debug(sta, s);
                    }
                    lua_pushstring( kv.second.lua_state.lua_state(), s.c_str());
                    lua_error( kv.second.lua_state.lua_state() );
                }
                else
                {
                    std::string s = "Error. Reason = " + err.value();
                    if( se )
                    {
                        sol::this_state sta;
                        sta.L = kv.second.lua_state.lua_state();
                        //rooms[0]->script_ent->Debug(sta, s);
                        se->debug(sta, s);
                    }
                    lua_pushstring( kv.second.lua_state.lua_state(), s.c_str());
                    lua_error( kv.second.lua_state.lua_state() );
                }

            });

            
            kv.second.lua_state.script("debug.sethook (debug_hook, '', 1048575)");
            auto result = kv.second.pf();
            if(!result.valid()) {
                sol::error err = result;
                std::cout << err.what() << std::endl;
            }
        } catch(std::exception& ex) {
            std::cout << ex.what();
        }
    }
}
