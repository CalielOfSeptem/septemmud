#include "entity_manager.h"
#include "script_entities/script_entity.h"
#include "script_entities/roomobj.h"
#include "script_entities/livingentity.h"
#include "script_entities/playerobj.h"
#include "script_entities/daemonobj.h"
#include "script_entities/commandobj.h"
#include "string_utils.h"
#include "global_settings.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_set>

#include "spdlog/spdlog.h"

namespace spd = spdlog;

namespace fs = boost::filesystem;

void get_entity_str(EntityType etype, std::string& str)
{

    switch(etype) {
    case EntityType::DAEMON: {
        str = "DAEMON";
    } break;
    case EntityType::COMMAND: {
        str = "COMMAND";
    } break;
    case EntityType::ITEM: {
        str = "ITEM";
    } break;
    case EntityType::NPC: {
        str = "NPC";
    } break;
    case EntityType::PLAYER: {
        str = "PLAYER";
    } break;
    case EntityType::ROOM: {
        str = "ROOM";
    } break;
    case EntityType::LIB: {
        str = "LIB";
    } break;
    default:
        str = "UNKNOWN";
        break;
    }
}

entity_manager::entity_manager()
{


}

bool entity_manager::compile_virtual_file(std::string& relative_script_path,
                                          EntityType etype,
                                          std::string& script_text,
                                          std::string& reason)
{
    return compile_entity(relative_script_path, etype, script_text, reason);
}

bool entity_manager::compile_and_clone(std::string& relative_script_path,
                                       std::string& script_path_virtual,
                                       std::string& tag_text,
                                       std::string& tag_replace,
                                       std::string& reason)
{
    // return compile_entity(relative_script_path, etype, script_text, reason);

    std::string _fullpath = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    _fullpath += relative_script_path;

    try {
        if(!fs::exists(_fullpath)) {
            reason = "Unable to locate file: " + _fullpath;
            return false;
        }

        EntityType et;
        std::string _scriptext;
        if(!load_script_text(_fullpath, _scriptext, et, reason)) {
            return false;
        }
        if(tag_text.size() != 0) {
            boost::replace_all(_scriptext, tag_text, tag_replace);
        }
        
        if(!compile_entity(script_path_virtual, et, _scriptext, reason))
            return false;

    } catch(std::exception& ex) {
        reason = ex.what();
        return false;
    }
    return true;
}

bool entity_manager::compile_entity(std::string& relative_script_path,
                                    EntityType etype,
                                    std::string& script_text,
                                    std::string& reason)
{
    /*
for( int x = 2; x < 1000; x++ )
{

    script_text += "p"+std::to_string(x) + " = room.new()\r\n";
    script_text += "p"+std::to_string(x) + ":SetTitle('" + "ROOM" + std::to_string(x) + "')\r\n";
    script_text += "function p" + std::to_string(x) +":test() print(p"+ std::to_string(x) +":GetTitle()) end\r\n";
    script_text += "y = register_heartbeat(p"+std::to_string(x)+".test)\r\n";


    script_text += "\r\n";
}
*/
    // std::cout << script_text;

    // First step get the correct collcetion..
    //  std::unordered_map< std::string, entity_wrapper > & ent_ = m_room_objs; // just to initialize it
    switch(etype) {
    case EntityType::ROOM: {
        // Check for existing rooms
        std::set<std::shared_ptr<entity_wrapper> > rooms;
        get_rooms_from_path(relative_script_path, rooms);

       // for(auto& r : rooms) {
            // script_entity& self = r->script_ent.value().selfobj.as<script_entity>();
            //*self = NULL;
            // r->script_ent.value().selfobj = NULL;
            // r->script_env.value() = sol::nil;
            //(*r->script_state).collect_garbage();
       // }
        if(rooms.size() > 0)
            destroy_room(relative_script_path);

    } break;
    case EntityType::PLAYER: {
        // TODO: logic that finds the player and reconnects them with their client object, if they have an object in the
        // world
    } break;
    case EntityType::DAEMON: {

        std::set<std::shared_ptr<entity_wrapper> > daemons;
        get_daemons_from_path(relative_script_path, daemons);

        //for(auto& r : daemons) {
            // script_entity& self = r->script_ent.value().selfobj.as<script_entity>();
            //*self = NULL;
            // r->script_ent.value().selfobj = NULL;
            // r->script_env.value() = sol::nil;
            //(*r->script_state).collect_garbage();
        //}
        if(daemons.size() > 0)
            destroy_daemon(relative_script_path);
    } break;
    case EntityType::COMMAND: {

    } break;
    case EntityType::LIB: {
        return lua_safe_script(script_text, (*m_state).globals(), reason);
    } break;
    default:
        break;
    }
    sol::environment to_load;
    _init_entity_env(relative_script_path, etype, to_load);

    std::string test_ = to_load["_INTERNAL_SCRIPT_PATH_"];
    //assert(test_ == relative_script_path);

    bool b = lua_safe_script(script_text, to_load, reason);

    //_currently_loading_script = "";
    return b;
}

bool entity_manager::compile_script_file(std::string& file_path, std::string& reason)
{
    auto log = spd::get("main");
    std::stringstream ss;
    ss << "Compiling script: " << file_path;
    log->debug( ss.str() );

    fs::path p(file_path);
    if(!fs::exists(p)) {
        reason = "Script does not exist.";
        return false;
    }

    std::string script_ = std::regex_replace(
        file_path, std::regex("\\" + global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");
    //_currently_loading_script = script_;

    if(!m_state) {
        m_state = std::shared_ptr<sol::state>(new sol::state);
        init_lua();
    }
    // lua_safe_script(path, *m_state);
    std::string script_text;
    EntityType etype;
    std::string error_str;

    if(!load_script_text(file_path, script_text, etype, error_str)) {
        reason = error_str;
        return false;
    }

    return compile_entity(script_, etype, script_text, reason);
}

bool entity_manager::load_player(std::string playerName)
{
    // std::string player_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    std::string player_script_path = global_settings::Instance().GetSetting(BASE_PLAYER_ENTITY_PATH);
    std::string reason;

    std::stringstream ss;
    ss << "entities/player_" << playerName;

    std::string tpath = ss.str();
    std::string tagtext = "player_name"; // a hack to get our player name into the lua object constructor
    return compile_and_clone(player_script_path, tpath, tagtext, playerName, reason);
}

bool entity_manager::lua_safe_script(std::string& script_text, sol::environment env, std::string& reason)
{
    sol::state& lua = (*m_state);

    sol::load_result lr = lua.load(script_text);
    if(!lr.valid()) {
        sol::error err = lr;
        reason = err.what();
        return false;
    }

    m_state_internal->_current_script_f_ = lr; // lua.load(script_text);

    env.set_on(m_state_internal->_current_script_f_);

    sol::protected_function_result result = m_state_internal->_current_script_f_();
    if(!result.valid()) {
        sol::error err = result;
        reason = err.what();
        // std::cout << "call failed, sol::error::what() is " << what << std::endl;
        return false;
    }

    return true;
}

template <typename T> T* downcast(script_entity* b)
{
    return dynamic_cast<T*>(b);
}

void entity_manager::init_lua()
{
    sol::state& lua = (*m_state);
    m_state_internal.reset(new _internal_lua_);
    lua.open_libraries(sol::lib::base,
                       sol::lib::string,
                       sol::lib::math,
                       sol::lib::table,
                       sol::lib::package,
                       sol::lib::debug);

    lua.new_usertype<script_entity>("script_entity", 
            "Reset",  &script_entity::clear_props,
            "GetType", 
            &script_entity::GetEntityTypeString);

    lua.new_usertype<exitobj>("exitobj",
                              "GetExitPath",
                              &exitobj::GetExitPath,
                              "SetExit",
                              &exitobj::SetExit,
                              "SetExitDesc",
                              &exitobj::SetExitDesc,
                              "SetExitPath",
                              &exitobj::SetExitPath,
                              "SetObvious",
                              &exitobj::SetObvious,
                              "GetExit",
                              &exitobj::GetExit);

    lua.new_usertype<roomobj>("room",
                              sol::constructors<roomobj(sol::this_state, sol::this_environment)>(),
                              sol::meta_function::new_index,
                              &roomobj::set_property_lua,
                              sol::meta_function::index,
                              &roomobj::get_property_lua,
                              "GetTitle",
                              &roomobj::GetTitle,
                              "SetTitle",
                              &roomobj::SetTitle,
                              "GetDescription",
                              &roomobj::GetDescription,
                              "SetDescription",
                              &roomobj::SetDescription,
                              "GetShortDescription",
                              &roomobj::GetShortDescription,
                              "SetShortDescription",
                              &roomobj::SetShortDescription,
                              "GetExits",
                              &roomobj::GetExits,
                              "AddExit",
                              &roomobj::AddExit,
                              "GetType",
                              &script_entity::GetEntityTypeString,
                              "Debug",
                              &script_entity::debug,
                              "GetInventory",
                              &container_base::GetInventory,
                              "GetPlayers",
                              &roomobj::GetPlayers,
                              sol::base_classes,
                              sol::bases<script_entity, container_base>());

    lua.set_function("register_heartbeat", &heartbeat_manager::register_heartbeat_func_on, &_heartbeat);
    lua.set_function("deregister_heartbeat", &heartbeat_manager::deregister_heartbeat_func, &_heartbeat);

    lua.new_usertype<daemonobj>("daemon",
                                sol::constructors<daemonobj(sol::this_state ts, sol::this_environment, std::string)>(),
                                sol::meta_function::new_index,
                                &daemonobj::set_property_lua,
                                sol::meta_function::index,
                                &daemonobj::get_property_lua,
                                "GetName",
                                &daemonobj::GetName,
                                "SetName",
                                &daemonobj::SetName,
                                "GetType",
                                &script_entity::GetEntityTypeString,
                                //  "Debug", &script_entity::debug,
                                sol::base_classes,
                                sol::bases<script_entity>());

    lua.new_usertype<playerobj>("player",
                                sol::constructors<playerobj(sol::this_state, sol::this_environment, std::string)>(),
                                sol::meta_function::new_index,
                                &playerobj::set_property_lua,
                                sol::meta_function::index,
                                &playerobj::get_property_lua,
                                "GetPlayerName",
                                &playerobj::GetPlayerName,
                                "SendToEntity",
                                &playerobj::SendToEntity,
                                "SendToPlayer",
                                &playerobj::SendToEntity,
                                "GetEnvironment",
                                &playerobj::GetEnvironment,
                                "SetEnvironment",
                                &playerobj::SetEnvironment,
                                "SendToRoom",
                                &playerobj::SendToEnvironment,
                                "GetRoom",
                                &playerobj::GetRoom,
                                "GetType",
                                &script_entity::GetEntityTypeString,
                                "cwd", sol::readonly(&playerobj::cwd),
                                // "Debug", &script_entity::debug,
                                "GetName",
                                &script_entity::GetName,
                                "DoCommand",
                                &living_entity::DoCommand,
                                sol::base_classes,
                                sol::bases<living_entity, script_entity>());

    lua.new_usertype<living_entity>("living_entity",
                                    "GetType",
                                    &script_entity::GetEntityTypeString,
                                    // "Debug", &script_entity::debug,
                                    "DoCommand",
                                    &living_entity::DoCommand,
                                    "SendToEntity",
                                    &living_entity::SendToEntity,
                                    "SendToPlayer",
                                    &living_entity::SendToEntity,
                                    "SendToRoom",
                                    &living_entity::SendToEnvironment,
                                    "GetRoom",
                                    &living_entity::GetRoom,
                                    "GetName",
                                    &script_entity::GetName,
                                    sol::base_classes,
                                    sol::bases<script_entity>());

    lua.new_usertype<commandobj>(
        "command",
        sol::constructors<commandobj(sol::this_state, sol::this_environment, std::string), commandobj(sol::this_state, sol::this_environment, std::string, int)>(),
        sol::meta_function::new_index,
        &playerobj::set_property_lua,
        sol::meta_function::index,
        &playerobj::get_property_lua,
        "GetCommand",
        &commandobj::GetCommand,
        "SetCommand",
        &commandobj::SetCommand,
        "GetAliases",
        &commandobj::GetAliases,
        "SetAliases",
        &commandobj::SetAliases,
        "SetPriority",
        &commandobj::SetPriority,
        "GetPriority",
        &commandobj::GetPriority,
        "GetType",
        &script_entity::GetEntityTypeString,
        // "Debug", &script_entity::debug,
        sol::base_classes,
        sol::bases<script_entity>());

    lua.set_function("deregister_all_heartbeat", &heartbeat_manager::deregister_all_heartbeat_funcs, &_heartbeat);

    lua.set_function("move_living", &entity_manager::move_living, this);

    lua.set_function("command_cast", &downcast<commandobj>);
    lua.set_function("room_cast", &downcast<roomobj>);
    lua.set_function("player_cast", &downcast<playerobj>);

    lua.set_function("get_default_commands", [&]() -> std::map<std::string, commandobj*> & 
    { 
        return m_default_cmds; 
    });

    lua.set_function("get_move_command", [&]() -> commandobj * & { return m_default_cmds.find("move")->second; });

    lua.set_function("get_room_list", [&]() -> std::map<std::string, roomobj*> & { return m_room_lookup; });

    lua.set_function("get_entity_by_name", [&](std::string ename) -> playerobj * { return this->get_player(ename); });
    
    lua.set_function("get_setting", [&](std::string sname) -> std::string { return global_settings::Instance().GetSetting(sname); });

    //lua.set_function("tail_entity_log",
    //                 [&](script_entity * se) -> std::vector<std::string> & { return this->get_player(ename); });

    sol::environment tmp = sol::environment(lua, sol::create); // inherit);
    lua["_g_mirror_"] = tmp;

    /*
        sol::environment global_env = lua.globals();
        sol::environment base_env;

        _init_lua_env_(lua, global_env, global_env, "base_entity_", base_env);

        sol::environment daemon_env;
        _init_lua_env_(lua, global_env, base_env, get_env_str(EntityType::DAEMON), daemon_env);

        sol::environment command_env;
        _init_lua_env_(lua, global_env, base_env, get_env_str(EntityType::COMMAND), command_env);

        sol::environment item_env;
        _init_lua_env_(lua, global_env, base_env, get_env_str(EntityType::ITEM), item_env);

        sol::environment npc_env;
        _init_lua_env_(lua, global_env, base_env, get_env_str(EntityType::NPC), npc_env);

        sol::environment player_env;
        _init_lua_env_(lua, global_env, base_env, get_env_str(EntityType::PLAYER), player_env);

        sol::environment room_env;
        _init_lua_env_(lua, global_env, base_env, get_env_str(EntityType::ROOM), room_env);
    */
}

bool entity_manager::load_script_text(std::string& script_path,
                                      std::string& script_text,
                                      EntityType& obj_type,
                                      std::string& reason)
{
    std::ifstream in(script_path);
    std::stringstream buffer;
    buffer << in.rdbuf();

    std::string token;
    std::vector<std::string> file_tokens;

    bool bFoundType = false;
    while(std::getline(buffer, token, '\n')) {
        trim(token);

        if(token.compare("inherit lib") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::LIB;
            token = "";
            file_tokens.push_back(token);

            bFoundType = true;
        } else if(token.compare("inherit daemon") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::DAEMON;
            token = "";
            file_tokens.push_back(token);

            bFoundType = true;
        } else if(token.compare("inherit room") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::ROOM;
            token = "";
            file_tokens.push_back(token);

            bFoundType = true;
        } else if(token.compare("inherit player") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::PLAYER;
            token = "";
            file_tokens.push_back(token);
            bFoundType = true;
        } else if(token.compare("inherit object") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::ITEM;
            token = "";
            file_tokens.push_back(token);
            bFoundType = true;
        } else if(token.compare("inherit npc") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::NPC;
            token = "";
            file_tokens.push_back(token);

            bFoundType = true;
        } else if(token.compare("inherit command") == 0) {
            if(bFoundType) // bad. only one type per script
            {
                reason = "Multiple inherit directives detected. Only one entity type is allowed per script.";
                return false;
            }
            obj_type = EntityType::COMMAND;
            token = "";
            file_tokens.push_back(token);

            bFoundType = true;
        } else {
            file_tokens.push_back(token);
        }
    }
    script_text = vector_to_string(file_tokens, '\n');
    if(bFoundType == false) {
        reason = "No inherit directive found in script.";
        return false;
    }
    return true;
}
/*
bool entity_manager::get_room_by_script(std::string& script_path, std::unordered_set<entity_wrapper>& room_ents)
{
    auto search = m_room_objs.find(script_path);
    if(search == m_room_objs.end()) {
        return false;
    }
    room_ents = search->second;
    return true;
}
*/

bool entity_manager::destroy_player(std::string& script_path)
{
    sol::state& lua = (*m_state);

    {
        auto search = m_player_objs.find(script_path);
        if(search == m_player_objs.end()) {
            return false;
        }

        sol::environment env;
        std::string name;
        get_parent_env_of_entity(script_path, env, name);

        destroy_entity(search->second);

        env[name] = sol::nil;
        m_player_objs.erase(search);
    }

    lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_room(std::string& script_path)
{

    sol::state& lua = (*m_state);
    {
        auto search = m_room_objs.find(script_path);
        if(search == m_room_objs.end()) {
            return false;
        }

        sol::environment env;
        std::string name;
        get_parent_env_of_entity(script_path, env, name);

        _heartbeat.clear_heartbeat_funcs(script_path);

        for(auto e : search->second) {
            destroy_entity(e);
        }

        env[name] = sol::nil;
        m_room_objs.erase(search);
    }

    lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_command(std::string& script_path)
{

    sol::state& lua = (*m_state);
    {
        auto search = m_cmd_objs.find(script_path);
        if(search == m_cmd_objs.end()) {
            return false;
        }

        sol::environment env;
        std::string name;
        get_parent_env_of_entity(script_path, env, name);

        for(auto e : search->second) {
            destroy_entity(e);
        }

        env[name] = sol::nil;
        m_cmd_objs.erase(search);
    }

    lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_daemon(std::string& script_path)
{

    sol::state& lua = (*m_state);
    {
        auto search = m_daemon_objs.find(script_path);
        if(search == m_daemon_objs.end()) {
            return false;
        }

        sol::environment env;
        std::string name;
        get_parent_env_of_entity(script_path, env, name);

        for(auto e : search->second) {
            destroy_entity(e);
        }
        // sol::environment en_ = (*m_state).globals(); // env[name];
        env[name] = sol::nil;
        m_daemon_objs.erase(search);
    }

    lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_entity(std::shared_ptr<entity_wrapper>& ew)
{
    ew->script_ent->clear_props(); // = NULL;
    sol::environment genv = (*m_state).globals();
    genv.set_on(ew->_script_f_);
    ew.reset();
    return true;
}

bool entity_manager::_init_entity_env(std::string& script_path, EntityType etype, sol::environment& env)
{
    // std::string envstr = std::regex_replace(script_path, std::regex("\\" +
    // global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");  // remove the extra pathing stuff we don't
    // need

    sol::state& lua = (*m_state);

    std::vector<std::string> strs;
    boost::split(strs, script_path, boost::is_any_of("/"));

    sol::environment current_env = lua.globals();

    for(auto& en : strs) {
        sol::optional<sol::environment> maybe_env = current_env[en + "_env_"];

        if(maybe_env) {
            // it has already been initialized
            current_env = maybe_env.value(); // set it so we can go deeper..

        } else {
            // be careful with the next call, if the environment exists then
            // this function will destroy it.

            _init_lua_env_(current_env, lua.globals(), en + "_env_", current_env);
            // just for now, so we can sanity check where we are..
            current_env["_FS_PATH_"] = en + "_env_";
        }
    }

    current_env["_INTERNAL_SCRIPT_PATH_"] = script_path;
    std::string s;
    get_entity_str(etype, s);
    current_env["_INTERNAL_ENTITY_TYPE_"] = s;
    env = current_env;
    return true;
}

bool entity_manager::_init_lua_env_(sol::environment parent,
                                    sol::environment inherit,
                                    std::string new_child_env_name,
                                    sol::environment& new_child_env)
{
    sol::state& lua = (*m_state);
    sol::optional<sol::environment> test_env =
        parent[new_child_env_name]; // parent.get<sol::environment>( new_child_env_name );
    assert(!test_env);
    if(test_env) {
       
        parent[new_child_env_name] = sol::nil;
        lua.collect_garbage();
    }
    sol::environment tmp = sol::environment(lua, sol::create, lua.globals());
    parent[new_child_env_name] = tmp;
    // std::string force_weak = "setmetatable(" + new_child_env_name + ", {__mode = 'k'})";
    // lua.script( force_weak , parent);
    // __index = _G
    // tmp["__index"] = "_G";
    new_child_env = tmp;
    
    return true;
}

void entity_manager::invoke_heartbeat()
{
    _heartbeat.do_heartbeats();
}

void entity_manager::register_command(commandobj* cmd)
{
    std::string verb = cmd->GetCommand();
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);
    m_default_cmds[verb] = cmd;
    auto log = spd::get("main");
  
    std::stringstream ss;
    ss << "Registered command, command = " << verb << ", script=" << cmd->GetScriptPath();
    log->debug( ss.str() );
    
}

void entity_manager::deregister_command(commandobj* cmd)
{
    std::string verb = cmd->GetCommand();
    std::transform(verb.begin(), verb.end(), verb.begin(), ::tolower);
    auto search = m_default_cmds.find(verb);
    if(search != m_default_cmds.end()) {
        m_default_cmds.erase(search);
        
        auto log = spd::get("main");
  
        std::stringstream ss;
        ss << "De-Registered command, command = " << verb << ", script=" << cmd->GetScriptPath();
        log->debug( ss.str() );
        
      
        // search->second.insert(ew);
    }
}

void entity_manager::register_room(roomobj* room)
{
    std::string room_path = room->GetInstancePath();
    m_room_lookup[room_path] = room;
    auto log = spd::get("main");

    std::stringstream ss;
    ss << "Registered room into lookup table, room = " << room_path;
    log->debug( ss.str() );

}

void entity_manager::deregister_room(roomobj* room)
{
    std::string room_path = room->GetInstancePath();
    auto search = m_room_lookup.find(room_path);
    if(search != m_room_lookup.end()) {
        m_room_lookup.erase(search);
  
        auto log = spd::get("main");

        std::stringstream ss;
        ss << "De-Registered room from lookup table, room = " << room_path;
        log->debug( ss.str() );
        // search->second.insert(ew);
    }
}

void entity_manager::register_daemon(daemonobj* daemon)
{
    std::string daemon_path = daemon->GetInstancePath();
    m_daemon_lookup[daemon_path] = daemon;
    auto log = spd::get("main");

    std::stringstream ss;
    ss << "Registered daemon into lookup table, daemon = " << daemon_path;
    log->debug( ss.str() );
}

void entity_manager::deregister_daemon(daemonobj* daemon)
{
    std::string daemon_path = daemon->GetInstancePath();
    auto search = m_daemon_lookup.find(daemon_path);
    if(search != m_daemon_lookup.end()) {
        m_daemon_lookup.erase(search);
        auto log = spd::get("main");
        std::stringstream ss;
        ss  << "De-Registered daemon from lookup table, daemon = " << daemon_path;
        log->debug( ss.str() );
        
        // search->second.insert(ew);
    }
}

void entity_manager::register_entity(script_entity* entityobj, std::string &sp, EntityType etype)
{

    std::shared_ptr<entity_wrapper> ew(new entity_wrapper);
    ew->entity_type = etype;
    //assert(_currently_loading_script.size() != 0);
    ew->script_path = sp;
    entityobj->SetScriptPath(ew->script_path);
    ew->script_ent = entityobj;
    ew->_script_f_ = m_state_internal->_current_script_f_; // <-- work around to ensure all objects in env get destroyed

    m_state_internal->_last_registered_object_ = entityobj;

    sol::environment e_parent;
    std::string env_name;
    get_parent_env_of_entity(ew->script_path, e_parent, env_name);
    


    //  The idea behind the next few calls is to make sure that each entity gets it's own
    //  instance ID.  Each time a entity loads we look to the parent env to grab
    //  the intance ID, then we increment for the next entity.  Most of the time
    //  only 1 entity will be instantiated per script, so this will only be used
    //  when people get clever.  Primarily, this allows the entity manager
    //  to figure out which object is being referenced, e.g., during a destroy, so
    //  the other entities declared in the script remain untouched

    sol::optional<unsigned int> instance_id = e_parent[env_name]["_internal_instance_id_"];
    if(!instance_id) {
        e_parent[env_name]["_internal_instance_id_"] = 0;
        //  ew->instance_id = 0;
        entityobj->instanceID = 0;
    } else {
        // ew->instance_id = instance_id.value();
        int new_id = instance_id.value() + 1;
        e_parent[env_name]["_internal_instance_id_"] = new_id;
        entityobj->instanceID = new_id;
    }
    
    auto log = spd::get("main");
    std::stringstream ss;

    switch(etype) {
    case EntityType::PLAYER: {
        //assert(_currently_loading_script.size() != 0);
        auto search = m_player_objs.find(sp);
        if(search != m_player_objs.end()) {

            // search->second.insert(ew);
        } else {
            m_player_objs.insert({ ew->script_path, ew }); // new_set ); //new_set );get_entity_str()
            std::string e_str;
            get_entity_str(ew->entity_type, e_str);
            
            ss << "Registered new entity, type = " << e_str << ", Path =" << ew->script_path;
            log->debug( ss.str() );
        }
    } break;
    case EntityType::ROOM: {
        // get the current instance ID from the env..
        //assert(_currently_loading_script.size() != 0);
        auto search = m_room_objs.find(sp);
        if(search != m_room_objs.end()) {

            search->second.insert(ew);
        } else {
            std::set<std::shared_ptr<entity_wrapper> > new_set;
            new_set.insert(ew);
            m_room_objs.insert({ ew->script_path, new_set }); // new_set ); //new_set );get_entity_str()
            std::string e_str;
            get_entity_str(ew->entity_type, e_str);
            
            ss << "Registered new entity, type = " << e_str << ", Path =" << ew->script_path;
            log->debug( ss.str() );
            
        }
    } break;
    case EntityType::COMMAND: {
        // get the current instance ID from the env..
        // commandobj * cmdobj = static_cast<commandobj*>(entityobj);
        // std::string cmd_noun = cmdobj->GetName();
        //  auto cmd_search = m_base_cmds.find(cmd_noun);
        /*
        if( cmd_search != m_base_cmds.end() )
        {
            m_base_cmds.erase(cmd_search); // may need to move this destruct code elsewhere
        }
        */
        // m_base_cmds[cmd_noun] = cmdobj;
        //assert(_currently_loading_script.size() != 0);
        auto search = m_cmd_objs.find(sp);
        if(search != m_cmd_objs.end()) {

            search->second.insert(ew);
        } else {
            std::set<std::shared_ptr<entity_wrapper> > new_set;
            new_set.insert(ew);
            m_cmd_objs.insert({ ew->script_path, new_set }); // new_set ); //new_set );get_entity_str()
            std::string e_str;
            get_entity_str(ew->entity_type, e_str);
            ss << "Registered new entity, type = " << e_str << ", Path =" << ew->script_path;
            log->debug( ss.str() );
        }
    } break;
    case EntityType::DAEMON: {
        //assert(_currently_loading_script.size() != 0);
        auto search = m_daemon_objs.find(sp);
        if(search != m_daemon_objs.end()) {

            search->second.insert(ew);
        } else {
            std::set<std::shared_ptr<entity_wrapper> > new_set;
            new_set.insert(ew);
            m_daemon_objs.insert({ ew->script_path, new_set }); // new_set ); //new_set );get_entity_str()
            std::string e_str;
            get_entity_str(ew->entity_type, e_str);
            ss << "Registered new entity, type = " << e_str << ", Path =" << ew->script_path;
            log->debug( ss.str() );
        }
    } break;
    default:
        break;
    }
}

void entity_manager::deregister_entity(script_entity* entityobj, EntityType etype)
{
    return; // just for now
    /*
    switch( etype )
    {
        case EntityType::ROOM:
        {
            auto search = m_room_objs.find(entityobj.GetScriptPath());
            if(search != m_room_objs.end())
            {
                m_room_objs.erase(search);
            }
        }
        break;
        case EntityType::PLAYER:
        {
            auto search = m_player_objs.find(entityobj.GetScriptPath());
            if(search != m_player_objs.end())
            {
                m_player_objs.erase(search);
            }
        }
        default:
        break;
    }
    */
}

void entity_manager::get_rooms_from_path(std::string& script_path, std::set<std::shared_ptr<entity_wrapper> >& rooms)
{
    auto search = m_room_objs.find(script_path);
    if(search != m_room_objs.end()) {
        for(auto& e : search->second) {
            rooms.insert(e);
        }
    }
}

void entity_manager::get_daemons_from_path(std::string& script_path,
                                           std::set<std::shared_ptr<entity_wrapper> >& deamons)
{
    auto search = m_daemon_objs.find(script_path);
    if(search != m_daemon_objs.end()) {
        for(auto& e : search->second) {
            deamons.insert(e);
        }
    }
}

bool entity_manager::get_parent_env_of_entity(std::string& script_path, sol::environment& env, std::string& env_name)
{
    sol::state& lua = (*m_state);
    std::vector<std::string> strs;
    boost::split(strs, script_path, boost::is_any_of("/"));

    // the second to the last will always be the parent,
    // ex/ realms/void, realms is parent
    sol::environment curr_env = lua.globals();
    for(unsigned int x = 0; x < strs.size(); x++) {
        curr_env = curr_env[strs[x] + "_env_"];
        if(x == strs.size() - 2) {
            env = curr_env;
            env_name = strs[x + 1] + "_env_";
            return true;
        }
    }
    return false;
}

void entity_manager::reset()
{
    this->m_daemon_lookup.clear();
    this->m_default_cmds.clear();
    this->m_room_lookup.clear();

    _heartbeat.deregister_all_heartbeat_funcs();

    std::vector<std::string> destroy_entities;
    for(auto& e : m_room_objs) {
        for(auto& ew : e.second) {
            destroy_entities.push_back(ew->script_path);
        }
    }

    for(auto& rname : destroy_entities)
        destroy_room(rname);

    destroy_entities.clear();

    for(auto& e : m_player_objs) {
        destroy_entities.push_back(e.second->script_path);
    }

    for(auto& pname : destroy_entities)
        destroy_player(pname);

    destroy_entities.clear();
    for(auto& d : m_daemon_objs) {
        for(auto& ew : d.second) {
            destroy_entities.push_back(ew->script_path);
        }
    }

    for(auto& dname : destroy_entities)
        destroy_daemon(dname);

    destroy_entities.clear();
    for(auto& cmd : m_cmd_objs) {
        for(auto& ew : cmd.second) {
            destroy_entities.push_back(ew->script_path);
        }
    }

    for(auto& cmdname : destroy_entities)
        destroy_command(cmdname);
    this->m_state_internal.reset();
    m_state.reset();
    // m_room_objs.clear();
}

roomobj* entity_manager::GetRoomByScriptPath(std::string& script_path, unsigned int instance_id)
{
    std::string room_path = script_path + ":id=" + std::to_string(instance_id);
    auto search = m_room_lookup.find(room_path);
    if(search != m_room_lookup.end()) {
        return search->second;
    }
    /*
    std::set<std::shared_ptr<entity_wrapper> > rooms;
    get_rooms_from_path(script_path, rooms);

    for(auto& ew : rooms) {
        if(ew->instance_id == instance_id) {
            return dynamic_cast<roomobj*>(ew->script_ent);
        }
    }
    */

    return NULL;
}

daemonobj* entity_manager::GetDaemonByScriptPath(std::string& script_path, unsigned int& instance_id)
{
    std::string daemon_path = script_path + ":id=" + std::to_string(instance_id);
    auto search = m_daemon_lookup.find(daemon_path);
    if(search != m_daemon_lookup.end()) {
        return search->second;
    }
    /*
    std::set<std::shared_ptr<entity_wrapper> > dms;
    get_daemons_from_path(script_path, dms);

    for(auto& ew : dms) {
        if(ew->instance_id == instance_id) {
            return dynamic_cast<daemonobj*>(ew->script_ent);
        }
    }
    */
    return NULL;
}

playerobj* entity_manager::get_player(const std::string& player_name)
{
    std::string player_script_path = global_settings::Instance().GetSetting(BASE_PLAYER_ENTITY_PATH);
    player_script_path += "_" + boost::to_lower_copy(player_name);

    // file_path, std::regex("\\" + global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");
    //_currently_loading_script = script_;

    auto search = m_player_objs.find(player_script_path);
    if(search != m_player_objs.end()) {
        return static_cast<playerobj*>(search->second->script_ent);
    } else {
        return NULL;
    }
}

bool
get_entity_path_from_id_string(const std::string& entity_id, std::string& entity_script_path, unsigned int& instanceid)
{
    unsigned int tmp = 0;
    entity_script_path = boost::to_lower_copy(entity_id);
    std::size_t found = entity_script_path.find("id=");

    if(found != std::string::npos) {
        // the id has a id= in it..

        sscanf(entity_script_path.c_str(), "%*[^=]=%u", &tmp);
        entity_script_path.erase(entity_script_path.begin() + found,
                                 entity_script_path.end()); // remove it from the string
    }
    if(entity_script_path[entity_script_path.size() - 1] == '/' ||
       entity_script_path[entity_script_path.size() - 1] == ':') // no need to have this.
    {
        entity_script_path.erase(entity_script_path.size() - 1);
    }

    return true;
}

bool entity_manager::move_living(script_entity* target, const std::string& roomid)
{
    std::string roompath = roomid;
    std::size_t found = roompath.find(":id=");
    if(found == std::string::npos) {
        roompath += ":id=0";
    }
    //unsigned int instance = 0;
    //
    auto search = m_room_lookup.find(boost::to_lower_copy(roompath));
    roomobj* r = search->second;
    if(r != NULL) {
        move_entity(target, static_cast<script_entity*>(r));
    }
    // LOG_DEBUG << r->GetTitle();
    // get_entity_path_from_id_string(roomid, roompath, instance);
    return true;
}

bool entity_manager::move_entity(script_entity* target, script_entity* dest)
{
    assert(target != NULL);
    assert(dest != NULL);
    // bunch of sanity checks.. which should be done on the lua side but will
    // be enforced here too.
    switch(target->GetType()) {
    case EntityType::ROOM:
    case EntityType::DAEMON:
    case EntityType::UNKNOWN: {
        return false; // can't move these..
        break;
    }
    case EntityType::ITEM: {
        // TODO: Implement item movement
        break;
    }
    case EntityType::PLAYER:
    case EntityType::NPC: {
        if(dest->GetType() != EntityType::ROOM) {
            return false;
        }
        if(roomobj* r = dynamic_cast<roomobj*>(dest)) {
            playerobj* p = dynamic_cast<playerobj*>(target);
            if(p->GetEnvironment() != NULL)
                p->GetRoom()->RemoveEntityFromInventory(target);
            r->AddEntityToInventory(target);
            return true;
        } else {
            // cannot move into a non-container..
            return false;
        }
        break;
    }
    default:
        return false;
        break;
    }
    return true;
}

void entity_manager::debug(std::string& msg)
{
    //LOG_DEBUG << msg;
}

bool entity_manager::do_command(living_entity* e, const std::string cmd)
{
    std::string command_proc_path = global_settings::Instance().GetSetting(DEFAULT_COMMAND_PROC);
    unsigned int id = 0;
    daemonobj* dobj = entity_manager::Instance().GetDaemonByScriptPath(command_proc_path, id);
    if(dobj == NULL) {
        auto log = spd::get("main");
        std::stringstream ss;
        ss << "Error attempting to retrieve command proc.";
        log->debug( ss.str() );
        
        return false;
    }

    sol::optional<sol::table> self =
        dobj->m_userdata
            ->selfobj; // ew_daemon->env_obj.value()[ew_daemon->script_obj_name];//(*lua_primary)[entity_env[0]][entity_env[1]][ew_daemon->script_obj_name];
                       // //(*ew_daemon->script_state)[ew_daemon->script_obj_name];

    if(self) {
        sol::protected_function exec = self.value()["process_command"];
        auto result = exec(self, e, cmd);
        if(!result.valid()) {
            sol::error err = result;
            auto log = spd::get("main");
            std::stringstream ss;
            ss << err.what();
            log->debug( ss.str() );
        }
        return true;
    } else {
        auto log = spd::get("main");
        std::stringstream ss;
        ss << "Error attempting to retrieve command proc.";
        log->debug( ss.str() );
        return false;
    }

    return false;
}

void entity_manager::debug_script(script_entity* se, const std::string& msg)
{
    switch(se->GetType()) {
    case EntityType::ROOM: {
        //LOG_DEBUG_(RoomLog) << "[" << se->GetInstancePath() << "] " << msg;
    } break;
    default:
        break;
    }
}

std::vector<std::string> entity_manager::tail_entity_log(script_entity* se)
{
    // for(;;) {
    // std::ifstream infile("filename");
    // int current_position = find_new_text(infile);
    // sleep(1);
    // }
    std::vector<std::string> s;
    return s;
}



/*
bool entity_manager::compile_lib(std::string& script_or_path, std::string& reason)
{

    auto simple_handler = [](lua_State*, sol::protected_function_result result) {
        // You can just pass it through to let the call-site handle it
        return result;
    };

    if(!m_state) {
        m_state = std::shared_ptr<sol::state>(new sol::state);
        init_lua();
    }

     sol::state& lua = (*m_state);

    if( fs::exists(script_or_path) )
    {
        auto result = lua.script_file(script_or_path, simple_handler);
        if(result.valid()) {
            LOG_DEBUG << "Successfully executed script.";
            return true;
        } else {
            sol::error err = result;
            LOG_ERROR << err.what();
            return false;
        }
    }
    else
    {
        auto result = lua.script(script_or_path, simple_handler);
        if(result.valid()) {
            LOG_DEBUG << "Successfully executed script.";
            return true;
        } else {
            sol::error err = result;
            LOG_ERROR << err.what();
            return false;
        }
    }

}

*/
/*
bool entity_manager::get_command(std::string& verb, shared_ptr<entity_wrapper>& cmd)
{
    auto search = command_objs.find(boost::to_lower_copy(verb));
    if(search != command_objs.end()) {
        cmd = search->second;
        return true;
    } else {
        return false;
    }
}
 */
