#include "game_manager.h"
#include "config.h"
#include <sstream>
#include "global_settings.h"
#include "entity_manager.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

namespace dt = boost::date_time;

namespace fs = boost::filesystem;

void game_manager::init()
{
    using namespace boost::posix_time;
    using namespace boost::gregorian;
    
    std::string last_reboot = global_settings::Instance().GetSetting( DEFAULT_LAST_REBOOT );
    auto log = spd::get("main");
    log->debug( "Begin Init");
    
    if( last_reboot.size() > 0 )
    {
        /*
        date d(from_simple_string(last_reboot));
        std::stringstream ss;
        date::ymd_type ymd = d.year_month_day();
        greg_weekday wd = d.day_of_week();
        ss << "Last reboot = ";
        ss << wd.as_long_string() << " "
                << ymd.month.as_long_string() << " "
                << ymd.day << ", " << ymd.year
                << std::endl;
        */
        std::stringstream ss;
        ss << "Last reboot = ";
        ss << last_reboot;
        //std::string date_string = to_simple_string(ptime);
       log->debug(ss.str());
    }
    
    ptime now = second_clock::local_time();
    std::cout << now << std::endl;
    global_settings::Instance().SetSetting( DEFAULT_LAST_REBOOT, to_simple_string(now) );
    
    

    
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
    
    ss.str("");
    ss << "Loading rooms via the room cache..";
    log->debug(ss.str());
    
    
    std::vector<std::string> r_cache;
    std::string r_cache_reason;
    
        // Load void room path for comparison purposes
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    void_script_path += global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);
    bool bVoidLoaded = false;
    
    if( load_room_cache( r_cache, r_cache_reason ) )
    {
        std::string game_data_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
        for( auto r : r_cache)
        {
            std::string r_t = game_data_path + r;
            ss.str("");
            ss << "Loading room: " << r_t;
            log->debug(ss.str());
            if( void_script_path == r_t )
            {
                bVoidLoaded = true;
            }
            
            if(!entity_manager::Instance().compile_script_file(r_t, reason)) {
                std::stringstream ss;
                ss << "Unable to load room, reason = " << reason;
                log->debug(ss.str());
                if( void_script_path == r_t )
                {
                    on_error(ss.str());
                }
                return;
            } 
        }
    }
    
    if( !bVoidLoaded )
    {
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
    }


    /*
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
     */

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





bool game_manager::process_player_cmd(playerobj* p, std::string& cmd)
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
   // std::string wm_path = "workspaces/caliel/workroom";
    roomobj * roomt = NULL;//entity_manager::Instance().GetRoomByScriptPath(wm_path, 0);
    
    /*
    if( pcaliel == NULL )
    {
        entity_manager::Instance().load_player("caliel");
        pcaliel = entity_manager::Instance().get_player("caliel");
    }
    */

   // if( precluse == NULL )
   // {
   //     entity_manager::Instance().load_player("recluse");
   //     pcaliel = entity_manager::Instance().get_player("recluse");
   // }
    
    //roomobj * roomt = get_void_room();
   // if( pcaliel->GetEnvironment() != NULL )
    //    roomt = pcaliel->GetRoom();

    
    
   // assert( roomt != NULL );

        
    // move caliel into the room...
    //roomt->AddEntityToInventory(pcaliel);
    if( entity_manager::Instance().do_command(p, cmd) )
    {
        entity_manager::Instance().garbage_collect();
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
    return true;
}

bool game_manager::load_room_cache( std::vector<std::string>& cache, std::string& reason )
{
    try {
        json _j;
        auto game_root_path = boost::filesystem::canonical(global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH));
        auto room_cache = global_settings::Instance().GetSetting(DEFAULT_ROOM_CACHE_PATH);
        
        auto patha = boost::filesystem::weakly_canonical(game_root_path/room_cache);
        
        std::ifstream i(patha.string() + "/roomcache"); 
        i >> _j;
        
        assert( !_j["room_cache"].is_null() );
        if( !_j["room_cache"].is_null() )
        {
            cache = _j["room_cache"].get<std::vector<std::string>>();
        }
        
    } catch(std::exception& ex) {
        auto log = spd::get("main");
        std::stringstream ss;
        ss << "Error when attempting to load account " << ex.what() << ".";
        log->debug(ss.str());
        reason = ss.str();
        return false;
    }
    return true;
}

