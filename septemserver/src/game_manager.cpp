#include "game_manager.h"
#include "config.h"
#include <sstream>
#include "global_settings.h"
#include "entity_manager.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
namespace fs = boost::filesystem;

void game_manager::init()
{
    LOG_DEBUG << "Begin Init";

    // Load void room
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    void_script_path += global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);

    LOG_DEBUG << "Loading void room: " << void_script_path;
    std::string reason;
    if(!entity_manager::Instance().compile_script(void_script_path, reason)) {
        std::stringstream ss;
        ss << "Unable to load void room, reason = " << reason;
        LOG_ERROR << ss.str();
        on_error(ss.str());
        return;
    } 

    std::string daemon_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    daemon_path += global_settings::Instance().GetSetting(DEFAULT_DAEMON_PATH);
    LOG_DEBUG << "Loading daemons..: " << daemon_path;
    
    for ( fs::recursive_directory_iterator end, dir(daemon_path);
        dir != end; ++dir ) {
        // std::cout << *dir << "\n";  // full path
        if( fs::is_regular(dir->path()) )
        {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load daemon [" << dir->path().string() << "], reason = " << reason;
                LOG_ERROR << ss.str();
                on_error(ss.str());
                return;
            } 
        }
        //std::cout << dir->path().filename() << "\n"; // just last bit
    }
    
    std::string command_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    command_path += global_settings::Instance().GetSetting(DEFAULT_COMMANDS_PATH);
    LOG_DEBUG << "Loading commands..: " << command_path;
    
    for ( fs::recursive_directory_iterator end, dir(command_path);
        dir != end; ++dir ) {
        // std::cout << *dir << "\n";  // full path
        if( fs::is_regular(dir->path()) )
        {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load command [" << dir->path().string() << "], reason = " << reason;
                LOG_ERROR << ss.str();
                on_error(ss.str());
                return;
            } 
        }
        //std::cout << dir->path().filename() << "\n"; // just last bit
    }

    SetState(gameState::RUNNING);
}

roomobj* game_manager::get_void_room()
{
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);
    unsigned int id = 0;
    roomobj * r = entity_manager::Instance().GetRoomByScriptPath(void_script_path, id);
    if( r != NULL )
    {
        return r;
    }
    else
    {
        std::string err = "Error attempting to retrieve void path.";
        LOG_ERROR << err;
        on_error(err);
    }
    return NULL;
}

daemonobj* game_manager::get_command_proc()
{
    std::string command_proc_path = global_settings::Instance().GetSetting(DEFAULT_COMMAND_PROC);
    unsigned int id = 0;
    daemonobj * r = entity_manager::Instance().GetDaemonByScriptPath(command_proc_path, id);
    if( r != NULL )
    {
        return r;
    }
    else
    {
        std::string err = "Error attempting to retrieve command proc.";
        LOG_ERROR << err;
        on_error(err);
    }
    return NULL;
}

bool game_manager::process_player_cmd(script_entity* p, std::string& cmd)
{
    if( m_state != gameState::RUNNING )
    {
        LOG_DEBUG << "State != RUNNING, unable to process command";
        return false;
    }
    // for testing purposes, just pull up my test player..
    // TODO: implement the actual logic..
    
    playerobj * pcaliel = entity_manager::Instance().get_player("caliel");
    if( pcaliel == NULL )
    {
        entity_manager::Instance().load_player();
        pcaliel = entity_manager::Instance().get_player("caliel");
    }
    roomobj * roomt = get_void_room();
    daemonobj * dobj = get_command_proc();

    assert( pcaliel != NULL );
    assert( roomt != NULL );
    assert( dobj != NULL );
    
        
    // move caliel into the room...
    //roomt->AddEntityToInventory(pcaliel);
    if( !entity_manager::Instance().move_entity(pcaliel, roomt) )
    {
        LOG_ERROR << "Unable to move Caliel..";
        return false;
    }
    
    sol::optional<sol::table> self = dobj->m_userdata->selfobj; //ew_daemon->env_obj.value()[ew_daemon->script_obj_name];//(*lua_primary)[entity_env[0]][entity_env[1]][ew_daemon->script_obj_name]; //(*ew_daemon->script_state)[ew_daemon->script_obj_name];
    
    //sol::optional<base_entity&> bep = ew_daemon->script_obj;
    if( self )
    {
        sol::protected_function exec = self.value()["process_command"];
        auto result = exec(self, pcaliel, cmd );
        if ( !result.valid() ) {
            sol::error err = result;
            LOG_ERROR << err.what();
        }
    }
    else
    {
        LOG_ERROR << "Unable to load command processor.";
    }
    
    return true;
}

bool game_manager::move_entity_into_room(script_entity* en, std::string room_path)
{
    
}

