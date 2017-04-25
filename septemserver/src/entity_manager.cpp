#include "entity_manager.h"
#include "script_entity.h"
#include "roomobj.h"
#include "string_utils.h"
#include "global_settings.h"
#include "config.h"
#include <plog/Log.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

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
    default:
        str = "UNKNOWN";
        break;
    }

}

bool entity_manager::compile_script(std::string& file_path, std::string& reason)
{
    LOG_DEBUG << "Compiling script: " << file_path;
    
    fs::path p(file_path);
    if(!fs::exists(p)) {
        reason = "Script does not exist.";
        return false;
    }
    
    std::string script_ = std::regex_replace(file_path, std::regex("\\" + 
            global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");  
    _currently_loading_script = script_;
    
    if(!m_state) {
        m_state = std::shared_ptr<sol::state>(new sol::state);
        init_lua(*m_state);
    }
    //lua_safe_script(path, *m_state);
    std::string script_text;
    EntityType etype;
    std::string error_str;

    if(!load_script_text(file_path, script_text, etype, error_str)) {
        reason = error_str;
        return false;
    }
    
    // First step get the correct collcetion..
  //  std::unordered_map< std::string, entity_wrapper > & ent_ = m_room_objs; // just to initialize it
    switch( etype )
    {
        case EntityType::ROOM:
        {
            // Check for existing rooms
            std::set<std::shared_ptr<entity_wrapper>> rooms;
            get_rooms_from_path(script_, rooms);
            
            for( auto r : rooms )
            {
               // script_entity& self = r->script_ent.value().selfobj.as<script_entity>();
                //*self = NULL;
                //r->script_ent.value().selfobj = NULL;
                //r->script_env.value() = sol::nil;
                //(*r->script_state).collect_garbage();
            }
            if( rooms.size() > 0 )
                destroy_room(script_, *m_state);
            
            sol::environment to_load;
            _init_entity_env(script_, etype, *m_state, to_load);
            
          
            assert( to_load["_INTERNAL_SCRIPT_PATH_"] == script_ );
            
            lua_safe_script(script_text, *m_state, to_load);
        }
        break;
        default:
        break;
    }
    
    _currently_loading_script = "";
    return true;
}

bool entity_manager::lua_safe_script(std::string& script_text, sol::state& lua, sol::environment env)
{
    auto simple_handler = [](lua_State*, sol::protected_function_result result) {
        // You can just pass it through to let the call-site handle it
        return result;
    };
    auto result = lua.script(script_text, env, simple_handler);
    if(result.valid()) {
        LOG_VERBOSE << "Successfully executed script.";
        return true;
    } else {
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
                              sol::base_classes,
                              sol::bases<script_entity>());

    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::package, sol::lib::math, sol::lib::table);
    
    lua.set_function("register_heartbeat", &heartbeat_manager::register_heartbeat_func_on, &_heartbeat);
    lua.set_function("deregister_heartbeat", &heartbeat_manager::deregister_heartbeat_func, &_heartbeat);
    lua.set_function("deregister_all_heartbeat", &heartbeat_manager::deregister_all_heartbeat_funcs, &_heartbeat);
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
        if(token.compare("inherit daemon") == 0) {
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
bool entity_manager::destroy_room(std::string& script_path, sol::state& lua)
{

    
 
    //TODO: Implement this function
    {
        auto search = m_room_objs.find(script_path);
        if(search == m_room_objs.end()) {
            return false;
        }
    
        sol::environment env;
        std::string name;
        get_parent_env_of_entity(script_path, env, name, lua);
        
        _heartbeat.clear_heartbeat_funcs(script_path);
        
        for( auto e : search->second )
        {
            entity_wrapper & ew = *e;
            ew.script_ent->clear_props();// = NULL;
            e.reset();
        }
        
        env[name] = sol::nil;
        m_room_objs.erase(search);

    }

   // env[name] = sol::nil;
  //  lua["realms"]["void"] = sol::nil;
   /// m_room_objs.clear();
   // lua["realms"]["void"]["p1"] = sol::nil;
    lua.collect_garbage();
    
    
    //x
    
    return true;
}


bool entity_manager::_init_entity_env( std::string& script_path, EntityType etype, sol::state& lua, sol::environment& env )
{
    //std::string envstr = std::regex_replace(script_path, std::regex("\\" + 
    //global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH)), "");  // remove the extra pathing stuff we don't need
        
    
    std::vector<std::string> strs;
    boost::split(strs, script_path, boost::is_any_of("/"));
    
    sol::environment current_env = lua.globals();
    
    for( auto en : strs )
    {
        sol::optional<sol::environment> maybe_env = current_env[en];
        if( maybe_env )
        {
            // it has already been initialized
            current_env = maybe_env.value(); // set it so we can go deeper..
            //LOG_VERBOSE << "Environment [" << en << "] already exists.";
        }
        else
        {
            // be careful with the next call, if the environment exists then
            // this function will destroy it.
            LOG_DEBUG << "Initializing environment: " << en;
            _init_lua_env_(lua, current_env, current_env, en, current_env);
           // just for now, so we can sanity check where we are..
           current_env["_FS_PATH_"] = en;
        }
    }
    
    current_env["_INTERNAL_SCRIPT_PATH_"] = script_path;
    std::string s;
    get_entity_str(etype, s);
    current_env["_INTERNAL_ENTITY_TYPE_"] = s;
    env = current_env;
    return true;
}

bool entity_manager::_init_lua_env_(sol::state& lua,
                                    sol::environment parent,
                                    sol::environment inherit,
                                    std::string new_child_env_name,
                                    sol::environment& new_child_env)
{
    sol::optional<sol::environment> test_env = parent[new_child_env_name]; // parent.get<sol::environment>( new_child_env_name );
    if(test_env) {
        LOG_DEBUG << "Destroying environment: " << new_child_env_name;
        parent[new_child_env_name] = sol::nil;
        lua.collect_garbage();
    }
    sol::environment tmp = sol::environment(lua, sol::create, inherit);
    parent[new_child_env_name] = tmp;
    new_child_env = tmp;
    return true;
}

void entity_manager::invoke_heartbeat()
{
    _heartbeat.do_heartbeats();
}

void entity_manager::register_entity(script_entity& entityobj, EntityType etype)
{
    switch( etype )
    {
        case EntityType::ROOM:
        {
            //m_room_objs
            auto search = m_room_objs.find(GetCurrentlyCompiledScript());
            if(search != m_room_objs.end()) {

                
                // Script exists, but no object matching selfobj exists in association with it

                LOG_DEBUG << "Located script..";
            }
            else
            {
                // does not exist
                std::shared_ptr<entity_wrapper> ew( new entity_wrapper );
                ew->entity_type = etype;
             //   ew->script_env = GetCurrentLoadingEnv();
                ew->script_path = GetCurrentlyCompiledScript();
               // ew->script_state = this->m_state;
                ew->script_ent = &entityobj;//sol::optional<script_entity&>(entityobj);
               
                std::set<std::shared_ptr<entity_wrapper>> new_set;
                
                new_set.insert( ew );
                m_room_objs.insert( {ew->script_path, new_set} );// new_set ); //new_set );get_entity_str() 
                std::string e_str;
                get_entity_str(ew->entity_type, e_str); 
                LOG_DEBUG << "Registered new entity, type = " << e_str << ", Path =" << ew->script_path;
               // m_room_objs[ew.script_path] = new_set;
            }
        }
        break;
        default:
        break;
    }
}

void entity_manager::get_rooms_from_path(std::string& script_path, std::set<std::shared_ptr<entity_wrapper> >& rooms)
{
    auto search = m_room_objs.find(script_path);
    if(search != m_room_objs.end()) 
    {
        for( auto e : search->second )
        {
            rooms.insert(e);
        }
    }
}

bool entity_manager::get_parent_env_of_entity(std::string& script_path, sol::environment& env, std::string& env_name, sol::state& lua)
{
    std::vector<std::string> strs;
    boost::split(strs, script_path, boost::is_any_of("/"));
    
    // the second to the last will always be the parent,
    // ex/ realms/void, realms is parent
    sol::environment curr_env = lua.globals();
    for( unsigned int x = 0; x < strs.size(); x++ )
    {
        curr_env = curr_env[strs[x]];
        if( x == strs.size() - 2 )
        {
            env = curr_env;
            env_name = strs[x+1];
            return true;
        }
    }
    return false;
}
