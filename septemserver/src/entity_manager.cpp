#include "entity_manager.h"
#include "script_entities/script_entity.h"
#include "script_entities/roomobj.h"
#include "script_entities/livingentity.h"
#include "script_entities/playerobj.h"
#include "script_entities/daemonobj.h"
#include "script_entities/commandobj.h"
#include "script_entities/itemobj.h"
#include "string_utils.h"
#include "global_settings.h"
#include "fs/fs_manager.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_set>

#include <random>
#include <algorithm>
#include <iterator>
#include "spdlog/spdlog.h"

namespace spd = spdlog;

namespace fs = boost::filesystem;

namespace {
std::string const default_chars = 
    "abcdefghijklmnaoqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
}

std::string random_string(size_t len = 15, std::string const &allowed_chars = default_chars) {
    std::mt19937_64 gen { std::random_device()() };

    std::uniform_int_distribution<size_t> dist { 0, allowed_chars.length()-1 };

    std::string ret;

    std::generate_n(std::back_inserter(ret), len, [&] { return allowed_chars[dist(gen)]; });
    return ret;
}



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
                                       std::string& addl_lua,
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
        //_scriptext = _scriptext + "\n_INTERNAL_UUID_ = '"+tag_replace+"'"; //may need to rethink this
        //std::cout << _scriptext << std::endl;
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
    bool b = false;
    switch(etype) {
    case EntityType::ROOM: {
        // Check for existing rooms
        std::set<std::shared_ptr<entity_wrapper> > rooms;
        get_rooms_from_path(relative_script_path, rooms);
        std::map < std::string, std::vector<playerobj*> > _pents;
        
        //std::vector< playerobj * > ps;
        
        for(auto& r : rooms) {
            roomobj * rp = dynamic_cast<roomobj*>(r->script_ent);
            for( auto pe : rp->GetInventory() )
            {
                if( auto t = dynamic_cast<playerobj*>(pe) )
                {
                    //ps.push_back(t);
                    //rp->RemoveEntityFromInventory(pe);
                    t->SetEnvironment(NULL); // need to do this, but maybe not here.  TODO: rethink this
                    _pents[rp->GetInstancePath()].push_back(t);
                }
                // TODO: destroy npcs and items in the room
            }
        }
        
        if(rooms.size() > 0)
            destroy_room(relative_script_path);
        sol::environment to_load;
        _init_entity_env(relative_script_path, etype, to_load);
        b = lua_safe_script(script_text, to_load, reason);
        if( !b )
        {
            // we have a problem, move players to void.. or shutdown server is void is borked too
        }
        else
        {
            rooms.clear();
            get_rooms_from_path(relative_script_path, rooms);
            
            for( auto & k : _pents )
            {
                for( auto v : k.second )
                {
                    // for each player ..
                    // try to move them..
                    if( !move_living(dynamic_cast<script_entity*>(v), k.first) )
                    {
                        // move them to void, or fail.
                    }
                    else
                    {
                        v->SendToEntity("A paralyzing sense of vertigo overtakes you.. and then quickly passes.");
                    }
                }
            }
            
        }

    } break;
    case EntityType::PLAYER: {
        // TODO: logic that finds the player and reconnects them with their client object, if they have an object in the
        // world
        sol::environment to_load;
        _init_entity_env(relative_script_path, etype, to_load);
        b = lua_safe_script(script_text, to_load, reason);
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
        sol::environment to_load;
        _init_entity_env(relative_script_path, etype, to_load);
        b = lua_safe_script(script_text, to_load, reason);
    } break;
    case EntityType::ITEM: {
        sol::environment to_load;
        _init_entity_env(relative_script_path, etype, to_load);
        b = lua_safe_script(script_text, to_load, reason);
    } break;
    case EntityType::COMMAND: {
        sol::environment to_load;
        _init_entity_env(relative_script_path, etype, to_load);
        b = lua_safe_script(script_text, to_load, reason);
    } break;
    case EntityType::LIB: {
        return lua_safe_script(script_text, (*m_state).globals(), reason);
    } break;
    
    default:
        break;
    }
    return b;
}

bool entity_manager::clone_item(std::string& relative_script_path, script_entity* obj)
{
    std::string uid = random_string();
    std::string reason;
    std::string tag = "";
    std::string vpath = relative_script_path + "/" + uid;
    std::string adl_lua = "\n_ENTITY_ENV_PATH_ = '" + obj->GetInstancePath() + "'";
    adl_lua += "\n_ENTITY_ENV_TYPE_ = '" + obj->GetEntityTypeString() + "'";
    adl_lua += "\n_INTERNAL_UUID_ = '"+uid+"'"; //may need to rethink this
    if( !this->compile_and_clone( relative_script_path, vpath, tag, uid, adl_lua, reason ) )
    {
        return false;
    }
    else
    {
        itemobj * i = GetItemByScriptPath(vpath);
        if( i != NULL )
        {
            switch( obj->GetType() )
            {
                case EntityType::ROOM:
                case EntityType::PLAYER:
                case EntityType::NPC:
                {
                    if( auto e = dynamic_cast<container_base*>(obj) )
                    {
                        e->AddEntityToInventory(i);
                        return true;
                    }
                    else
                    {
                        std::cout << "Died" << std::endl;
                        return false;
                    }
                        
                } break;
                default:
                    return false;
                break;
            }
            std::cout << "Found it" << std::endl;
        }
        // now find the item &..
      //  get_daemons_from_path()
    }
    return true;
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
    std::string adllua = "";
    return compile_and_clone(player_script_path, tpath, tagtext, playerName, adllua, reason);
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
            "GetName",  &script_entity::GetName,
            "SetName", &script_entity::SetName,
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

    lua.new_usertype<handobj>("hand",
                            "GetInventory",
                            &container_base::GetInventory,
                            "IsEmpty",
                            &handobj::IsEmpty,
                            "AddToInventory",
                            &handobj::AddEntityToInventory,
                            "RemoveFromInventory",
                            &handobj::RemoveEntityFromInventory,
                            sol::base_classes,
                            sol::bases<container_base>() );
                            
    lua.new_usertype<itemobj>("item",
                            sol::constructors<itemobj(sol::this_state, sol::this_environment)>(),
                            sol::meta_function::new_index,
                            &itemobj::set_property_lua,
                            sol::meta_function::index,
                            &itemobj::get_property_lua,
                            "GetInventory",
                            &container_base::GetInventory,
                            
                            "GetSize", &itemobj::get_size,
                            "SetSize", &itemobj::set_size,
                            
                            "GetWeight", &itemobj::get_weight,
                            "SetWeight", &itemobj::set_weight,
                            
                            "GetWearable", &itemobj::get_isWearable,
                            "SetWearable", &itemobj::set_isWearable,

                            "GetStackable", &itemobj::get_isStackable,
                            "SetStackable", &itemobj::set_isStackable,
                            
                            "GetIsContainer", &itemobj::get_isContainer,
                            "SetIsContainer", &itemobj::set_isContainer,
                            
                            "weight", sol::property(&itemobj::get_weight, &itemobj::set_weight),
                            "isWearable", sol::property(&itemobj::get_isWearable, &itemobj::set_isWearable),
                            "isStackable", sol::property(&itemobj::get_isStackable, &itemobj::set_isStackable),
                            "isContainer", sol::property(&itemobj::get_isContainer, &itemobj::set_isContainer),
                            
                            "size", sol::property(&itemobj::get_size, &itemobj::set_size),
                            "weight", sol::property(&itemobj::get_weight, &itemobj::set_weight),
                            "isWearable", sol::property(&itemobj::get_isWearable, &itemobj::set_isWearable),
                            "isStackable", sol::property(&itemobj::get_isStackable, &itemobj::set_isStackable),
                            "isContainer", sol::property(&itemobj::get_isContainer, &itemobj::set_isContainer),
                            
                            
                            sol::base_classes,
                            sol::bases<script_entity, container_base>() );
                            
                            
     //                            double m_weight; // item weight in stones
    // ItemSize m_size = ItemSize::TINY;
    // bool bisWearable;
    // bool bisStackable;
    // bool bisContainer;
    
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
                              "GetItems",
                              &roomobj::GetItems,
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
                                "workspacePath", sol::readonly(&playerobj::workspacePath),
                                // "Debug", &script_entity::debug,
                                "GetName",
                                &script_entity::GetName,
                                "DoCommand",
                                &living_entity::DoCommand,
                                sol::base_classes,
                                sol::bases<living_entity, script_entity>());
                                
    lua.new_usertype<file_entity>("file_entity", 
            "Name", sol::readonly(&file_entity::name),
            "isDirectory", sol::readonly(&file_entity::isDirectory));

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
                                    "RightHand",
                                    &living_entity::GetRightHand,
                                    "LeftHand",
                                    &living_entity::GetLeftHand,
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
    lua.set_function("living_cast", &downcast<living_entity>);


    lua.set_function("get_default_commands", [&]() -> std::map<std::string, commandobj*> & 
    { 
        return m_default_cmds; 
    });

    lua.set_function("get_move_command", [&]() -> commandobj * & { return m_default_cmds.find("move")->second; });

    lua.set_function("get_room_list", [&]() -> std::map<std::string, roomobj*> & { return m_room_lookup; });

    lua.set_function("get_entity_by_name", [&](std::string ename) -> playerobj * { return this->get_player(ename); });
    
    lua.set_function("get_setting", [&](std::string sname) -> std::string { return global_settings::Instance().GetSetting(sname); });
    
    lua.set_function("get_dir_list", [&](std::string dir) -> std::vector<file_entity> { return fs_manager::Instance().get_dir_list(dir); });

    lua.set_function("change_directory", [&](std::string dir, playerobj * p) -> bool { return fs_manager::Instance().change_directory(dir, p); });
    
    lua.set_function("do_update", [&](std::string dir, playerobj * p) -> bool { return this->do_update(dir, p); });
    
    lua.set_function("do_copy", [&](std::string patha, std::string pathb, playerobj * p) -> bool { return fs_manager::Instance().do_copy(patha, pathb, p); });
    
    lua.set_function("do_remove", [&](std::string path, playerobj * p) -> bool { return fs_manager::Instance().do_remove(path, p); });
     
    lua.set_function("do_goto", [&](std::string path, playerobj * p) -> bool { return this->do_goto(path, p); });
     
    lua.set_function("do_tp", [&](std::string path, playerobj * p1, playerobj *p2) -> bool { return this->do_tp(path, p1, p2); });
    
    lua.set_function("clone_item", [&](std::string path, script_entity * e) -> bool { return this->clone_item(path, e); });
    //lua.set_function("tail_entity_log",
    //                 [&](script_entity * se) -> std::vector<std::string> & { return this->get_player(ename); });

    //sol::environment tmp = sol::environment(lua, sol::create); // inherit);
    //lua["_g_mirror_"] = tmp;

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
        } else if(token.compare("inherit item") == 0) {
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
    //sol::state& lua = (*m_state);

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

  //  lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_room(std::string& script_path)
{

    //sol::state& lua = (*m_state);
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

    //lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_command(std::string& script_path)
{

    //sol::state& lua = (*m_state);
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

    //lua.collect_garbage();

    return true;
}

bool entity_manager::destroy_item(std::string& script_path)
{

    //sol::state& lua = (*m_state);
    {
        auto search = m_item_objs.find(script_path);
        if(search == m_item_objs.end()) {
            return false;
        }

        sol::environment env;
        std::string name;
        get_parent_env_of_entity(script_path, env, name);

        for(auto e : search->second) {
            destroy_entity(e);
        }

        env[name] = sol::nil;
        m_item_objs.erase(search);
    }

    //lua.collect_garbage();

    return true;
}


bool entity_manager::destroy_daemon(std::string& script_path)
{

    //sol::state& lua = (*m_state);
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

    //lua.collect_garbage();

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


void entity_manager::register_item(itemobj* item)
{
    
    std::string item_path = item->GetInstancePath();
    m_item_lookup[item_path] = item;
    auto log = spd::get("main");

    std::stringstream ss;
    ss << "Registered item into lookup table, item = " << item_path;
    log->debug( ss.str() );
    
}

void entity_manager::deregister_item(itemobj* item)
{
    std::string item_path = item->GetInstancePath();
    auto search = m_item_lookup.find(item_path);
    if(search != m_item_lookup.end()) {
        m_item_lookup.erase(search);
        auto log = spd::get("main");
        std::stringstream ss;
        ss  << "De-Registered item from lookup table, item = " << item_path;
        log->debug( ss.str() );
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
    case EntityType::ITEM: {
        auto search = m_item_objs.find(sp);
        if(search != m_item_objs.end()) {
            search->second.insert(ew);
        } else {
            std::set<std::shared_ptr<entity_wrapper> > new_set;
            new_set.insert(ew);
            m_item_objs.insert({ ew->script_path, new_set }); // new_set ); //new_set );get_entity_str()
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
    this->m_item_lookup.clear();

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
        
    destroy_entities.clear();
    for(auto& cmd : m_item_objs) {
        for(auto& ew : cmd.second) {
            destroy_entities.push_back(ew->script_path);
        }
    }

    for(auto& itemname : destroy_entities)
        destroy_item(itemname);
        
        
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


itemobj* entity_manager::GetItemByScriptPath(std::string& script_path, unsigned int instance_id)
{
    std::string item_path = script_path + ":id=" + std::to_string(instance_id);
    auto search = m_item_lookup.find(script_path);
    if(search != m_item_lookup.end()) {
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

itemobj* entity_manager::GetItemByScriptPath(std::string& script_path)
{
    //std::string room_path = script_path + ":id=" + std::to_string(instance_id);
    std::string itempath = script_path;
    std::size_t found = itempath.find(":id=");
    if(found == std::string::npos) {
        itempath += ":id=0";
    }
    auto search = m_item_lookup.find(itempath);
    if(search != m_item_lookup.end()) {
        return search->second;
    }

    return NULL;
}


roomobj* entity_manager::GetRoomByScriptPath(std::string& script_path)
{
    //std::string room_path = script_path + ":id=" + std::to_string(instance_id);
    std::string roompath = script_path;
    std::size_t found = roompath.find(":id=");
    if(found == std::string::npos) {
        roompath += ":id=0";
    }
    auto search = m_room_lookup.find(roompath);
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
    if( roomid.size() == 0 )
    {
        target->debug("Error, path cannot be empty.");
        return false;
    }
    std::string roompath = boost::to_lower_copy(roomid);
    std::size_t found = roompath.find(":id=");
    if(found == std::string::npos) {
        roompath += ":id=0";
    }
    if( roompath[0] == '/' )
        roompath.erase(0, 1);
    
    roomobj* r = GetRoomByScriptPath(roompath);
    if(r != NULL) {
        move_entity(target, static_cast<script_entity*>(r));
    }
    else
    {
        std::stringstream ss;
        ss << "Error. Path [" << roomid << "] not found.";
        target->debug(ss.str());
        return false;
    }
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


bool entity_manager::do_update(std::string& entitypath, playerobj* p )
{
    std::string temp = entitypath;
    std::string reason;
    if( !fs_manager::Instance().translate_path( temp, p, reason ) )
    {
        p->SendToEntity(reason);
    }
    if( fs::is_directory(temp ))
    {
        p->SendToEntity("Directory compiling not yet implemeneted..");
    }
    else
    {
        std::string reason;
        if ( !compile_script_file(temp, reason) )
        {
            p->SendToEntity(std::string("Error: ") + reason );
        }
    }
    return true;
}

bool entity_manager::do_goto(std::string& entitypath, playerobj* p )
{
    std::string temp = entitypath;
    
    std::string reason;
    if( !fs_manager::Instance().translate_path( temp, p, reason ) )
    {
        p->SendToEntity(reason);
        return false;
    }
    if( p == NULL )
    {
        p->SendToEntity("Target player cannot be null");
    }
    
    std::string rel_path = std::regex_replace(temp, std::regex(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");
    
    roomobj * r =  GetRoomByScriptPath(rel_path);
    if( r != NULL )
    {
        return move_living(p, r->GetInstancePath());
    }
    else
    {
        p->SendToEntity("Unable to goto location, room does not exist.");
    }

    return false;
}

bool entity_manager::do_tp(std::string& entitypath, playerobj* p_targ, playerobj* p_caller )
{
    std::string temp = entitypath;
    
    std::string reason;
    if( !fs_manager::Instance().translate_path( temp, p_caller, reason ) )
    {
        p_caller->SendToEntity(reason);
    }
    if( p_targ == NULL || p_caller == NULL )
    {
        p_caller->SendToEntity("Target player cannot be null");
    }
    
    std::string rel_path = std::regex_replace(temp, std::regex(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");
    
    roomobj * r =  GetRoomByScriptPath(rel_path);
    if( r != NULL )
    {
        return move_living(p_targ, r->GetInstancePath());
    }
    else
    {
        p_caller->SendToEntity("Unable to goto location, room does not exist.");
    }

    return false;
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
