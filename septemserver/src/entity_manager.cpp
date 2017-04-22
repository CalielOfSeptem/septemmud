#include "include/entity_manager.h"
#include "include/script_entity.h"
#include "include/roomobj.h"
#include <plog/Log.h>

std::string get_env_str( EntityType etype )
{
    std::string entitys;
    switch(etype) {
    case EntityType::DAEMON: {
        entitys = "_daemon_env";
    }break;
    case EntityType::COMMAND: {
        entitys = "_command_env";
    }break;
    case EntityType::ITEM: {
        entitys = "_item_env";
    }break;
    case EntityType::NPC: {
        entitys = "_npc_env";
    }break;
    case EntityType::PLAYER: {
        entitys = "_player_env";
    }break;
    case EntityType::ROOM: {
        entitys = "_room_env";
    }break;
    default:
    break;
    }
    
    return entitys;
}


bool entity_manager::compile_script(std::string& path)
{
    if( !m_state )
    {
        m_state = std::shared_ptr < sol::state > (new sol::state);
        init_lua(*m_state);
    }
    lua_safe_script(path, *m_state);
    return true;
}

bool entity_manager::lua_safe_script(std::string& script_path, sol::state& lua)
{
        auto simple_handler = [](lua_State*, sol::protected_function_result result) {
            // You can just pass it through to let the call-site handle it
            return result;
        };
        auto result = lua.script_file(script_path, simple_handler);
        if (result.valid()) {
            LOG_VERBOSE << "Successfully executed script.";
           return true;
        }
        else {
            sol::error err = result;
            LOG_ERROR << err.what();
        }
        return false;
}


void entity_manager::init_lua(sol::state& lua)
{
    lua.open_libraries(sol::lib::base);

    lua.new_usertype<roomobj>("room",
            sol::constructors<roomobj(sol::this_state)>(),
            sol::meta_function::new_index, &roomobj::set_property_lua,
            sol::meta_function::index, &roomobj::get_property_lua,
            "GetTitle", &roomobj::GetTitle,
            "SetTitle", &roomobj::SetTitle,
            "GetDescription", &roomobj::GetDescription,
            "SetDescription", &roomobj::SetDescription,
            "GetShortDescription", &roomobj::GetShortDescription,
            "SetShortDescription", &roomobj::SetShortDescription,
            "GetExits", &roomobj::GetExits,
            "AddExit", &roomobj::AddExit,
            sol::base_classes, sol::bases<script_entity>()
    );
    
    
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::math, sol::lib::table);
    
    sol::environment global_env = lua.globals();
    sol::environment base_env;
    
    _init_lua_env_(lua, global_env, global_env, "base_entity_", base_env);
    
    sol::environment daemon_env;
    _init_lua_env_( lua, global_env, base_env, get_env_str(EntityType::DAEMON), daemon_env );
    
    sol::environment command_env;
    _init_lua_env_( lua, global_env, base_env, get_env_str(EntityType::COMMAND), command_env );
    
    sol::environment item_env;
    _init_lua_env_( lua, global_env, base_env, get_env_str(EntityType::ITEM), item_env );
    
    sol::environment npc_env;
    _init_lua_env_( lua, global_env, base_env, get_env_str(EntityType::NPC), npc_env );
    
    sol::environment player_env;
    _init_lua_env_( lua, global_env, base_env, get_env_str(EntityType::PLAYER), player_env );
    
    sol::environment room_env;
    _init_lua_env_( lua, global_env, base_env, get_env_str(EntityType::ROOM), room_env );

}

bool entity_manager::_init_lua_env_( sol::state& lua, sol::environment parent, 
        sol::environment inherit, std::string new_child_env_name, 
        sol::environment& new_child_env )
{
    sol::environment test_env = parent[new_child_env_name];//parent.get<sol::environment>( new_child_env_name );
    if( test_env  )
    {
        parent[new_child_env_name] = sol::nil;
        lua.collect_garbage();
    }
    new_child_env = sol::environment(lua, sol::create, inherit);
    parent[new_child_env_name] = new_child_env;
    
    return true;
} 
