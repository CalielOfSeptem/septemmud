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
    auto log = spd::get("main");
    log->debug( "Begin Init");
    
    std::string reason;
    // load libs
    std::string libs_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    libs_path += global_settings::Instance().GetSetting(DEFAULT_LIBS_PATH);
    
    std::stringstream ss;
    ss << "Loading libs: " << libs_path;
    log->debug(ss.str());
    ss.str("");
    
   
    for ( fs::recursive_directory_iterator end, dir(libs_path);
        dir != end; ++dir ) {
        // std::cout << *dir << "\n";  // full path
        if( fs::is_regular(dir->path()) )
        {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load lib [" << dir->path().string() << "], reason = " << reason;
                log->debug(ss.str());
                //LOG_ERROR << ss.str();
                on_error(ss.str());
                return;
            } 
        }
        //std::cout << dir->path().filename() << "\n"; // just last bit
    }
    
    
    
    // Load void room
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    void_script_path += global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);

    //std::stringstream ss;
    ss.str("");
    ss << "Loading void room: " << void_script_path;
    log->debug(ss.str());
                
    if(!entity_manager::Instance().compile_script_file(void_script_path, reason)) {
        std::stringstream ss;
        ss << "Unable to load void room, reason = " << reason;
        //LOG_ERROR << ss.str();
        log->debug(ss.str());
        on_error(ss.str());
        return;
    } 
    
    std::string caliel_wr_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    caliel_wr_path += "workspaces/caliel/workroom";

    //std::stringstream ss;
    ss.str("");
    ss << "Loading Caliel's room: " << caliel_wr_path;
    log->debug(ss.str());
                
    if(!entity_manager::Instance().compile_script_file(caliel_wr_path, reason)) {
        std::stringstream ss;
        ss << "Unable to load Caliel's workroom, reason = " << reason;
        //LOG_ERROR << ss.str();
        log->debug(ss.str());
        on_error(ss.str());
        return;
    } 

    std::string daemon_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    daemon_path += global_settings::Instance().GetSetting(DEFAULT_DAEMON_PATH);
    
    //std::stringstream ss;
    ss.str("");
    ss << "Loading daemons..: " << daemon_path;
    log->debug(ss.str());
    
   // LOG_DEBUG << 
    
    for ( fs::recursive_directory_iterator end, dir(daemon_path);
        dir != end; ++dir ) {
        // std::cout << *dir << "\n";  // full path
        if( fs::is_regular(dir->path()) )
        {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load daemon [" << dir->path().string() << "], reason = " << reason;
                //LOG_ERROR << ss.str();
                log->debug(ss.str());
                on_error(ss.str());
                return;
            } 
        }
        //std::cout << dir->path().filename() << "\n"; // just last bit
    }
    
    std::string command_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    command_path += global_settings::Instance().GetSetting(DEFAULT_COMMANDS_PATH);
    ss.str("");
    ss << "Loading commands..: " << command_path;
    log->debug(ss.str());
    
    for ( fs::recursive_directory_iterator end, dir(command_path);
        dir != end; ++dir ) {
        // std::cout << *dir << "\n";  // full path
        if( fs::is_regular(dir->path()) )
        {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load command [" << dir->path().string() << "], reason = " << reason;
                //LOG_ERROR << ss.str();
                log->debug(ss.str());
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
    auto log = spd::get("main");
    std::stringstream ss;
    
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
        ss << err;
        log->info(ss.str());
        on_error(err);
    }
    return NULL;
}


bool game_manager::process_player_cmd(script_entity* p, std::string& cmd)
{
    
    if( m_state != gameState::RUNNING )
    {
        auto log = spd::get("main");
        log->debug("State != RUNNING, unable to process command");
        return false;
    }
    
    
    //std::stringstream ss;
    // for testing purposes, just pull up my test player..
    // TODO: implement the actual logic..
    
    playerobj * pcaliel = entity_manager::Instance().get_player("caliel");
    playerobj * precluse = entity_manager::Instance().get_player("recluse");
    std::string wm_path = "workspaces/caliel/workroom";
    roomobj * roomt = entity_manager::Instance().GetRoomByScriptPath(wm_path, 0);
    
    
    
    if( pcaliel == NULL )
    {
        entity_manager::Instance().load_player("caliel");
        pcaliel = entity_manager::Instance().get_player("caliel");
        assert( pcaliel != NULL );
        if( !entity_manager::Instance().move_entity(pcaliel, roomt) )
        {
            auto log = spd::get("main");
            //LOG_ERROR << "Unable to move Caliel..";
            log->debug("Unable to move Caliel..");
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
            auto log = spd::get("main");
            log->debug("Unable to move Recluse..");
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
        auto log = spd::get("main");
        log->debug("Unable to execute command.");
    }
    
    return true;
}

bool game_manager::move_entity_into_room(script_entity* en, std::string room_path)
{
    
}

