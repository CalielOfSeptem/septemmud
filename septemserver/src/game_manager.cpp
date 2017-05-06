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
    std::string reason;
    // load libs
    std::string libs_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    libs_path += global_settings::Instance().GetSetting(DEFAULT_LIBS_PATH);

    LOG_DEBUG << "Loading libs: " << libs_path;
   
    for ( fs::recursive_directory_iterator end, dir(libs_path);
        dir != end; ++dir ) {
        // std::cout << *dir << "\n";  // full path
        if( fs::is_regular(dir->path()) )
        {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load lib [" << dir->path().string() << "], reason = " << reason;
                LOG_ERROR << ss.str();
                on_error(ss.str());
                return;
            } 
        }
        //std::cout << dir->path().filename() << "\n"; // just last bit
    }
    
    
    
    // Load void room
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    void_script_path += global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);

    LOG_DEBUG << "Loading void room: " << void_script_path;
    if(!entity_manager::Instance().compile_script_file(void_script_path, reason)) {
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
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
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
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
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
    playerobj * precluse = entity_manager::Instance().get_player("recluse");
    roomobj * roomt = get_void_room();
    
    
    if( pcaliel == NULL )
    {
        entity_manager::Instance().load_player("caliel");
        pcaliel = entity_manager::Instance().get_player("caliel");
        assert( pcaliel != NULL );
        if( !entity_manager::Instance().move_entity(pcaliel, roomt) )
        {
            LOG_ERROR << "Unable to move Caliel..";
            return false;
        }
    }
    if( precluse == NULL )
    {
        entity_manager::Instance().load_player("recluse");
        precluse = entity_manager::Instance().get_player("recluse");
        assert( precluse != NULL );
        if( !entity_manager::Instance().move_entity(precluse, roomt) )
        {
            LOG_ERROR << "Unable to move Recluse..";
            return false;
        }
        
    }
    
    //roomobj * roomt = get_void_room();
    if( pcaliel->GetEnvironment() != NULL )
        roomt = pcaliel->GetRoom();

    
    
    assert( roomt != NULL );

        
    // move caliel into the room...
    //roomt->AddEntityToInventory(pcaliel);
    if( entity_manager::Instance().do_command(pcaliel, cmd) )
    {
        
    }
    else
    {
        LOG_ERROR << "Unable to execute command.";
    }
    
    return true;
}

bool game_manager::move_entity_into_room(script_entity* en, std::string room_path)
{
    
}

