#include "stdafx.h"
#include "game_manager.h"

#include "entity_manager.h"
#include "script_entities/playerobj.h"

namespace dt = boost::date_time;

namespace fs = boost::filesystem;

void game_manager::init()
{
    using namespace boost::posix_time;
    using namespace boost::gregorian;

    std::string last_reboot = global_settings::Instance().GetSetting(DEFAULT_LAST_REBOOT);
    log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Begin Init");
    //log->debug("Begin Init");

    if(last_reboot.size() > 0) {
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
        // std::string date_string = to_simple_string(ptime);
		log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
    }

    ptime now = second_clock::local_time();
    // std::cout << now << std::endl;
    global_settings::Instance().SetSetting(DEFAULT_LAST_REBOOT, to_simple_string(now));

    std::string reason;
    // load libs
    std::string libs_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    libs_path += global_settings::Instance().GetSetting(DEFAULT_LIBS_PATH);

    std::stringstream ss;
    ss << "Loading libs: " << libs_path;
    log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
    ss.str("");

    for(fs::recursive_directory_iterator end, dir(libs_path); dir != end; ++dir) {
        // std::cout << *dir << "\n";  // full path
        if(fs::is_regular(dir->path())) {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load lib [" << dir->path().string() << "], reason = " << reason;
                log_interface::Instance().log(LOGLEVEL::LOGLEVEL_ERROR, ss.str());
                // LOG_ERROR << ss.str();
                on_error(ss.str());
                return;
            }
        }
        // std::cout << dir->path().filename() << "\n"; // just last bit
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

    // std::stringstream ss;
    ss.str("");
    ss << "Loading daemons..: " << daemon_path;
    log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());

    // LOG_DEBUG <<

    for(fs::recursive_directory_iterator end, dir(daemon_path); dir != end; ++dir) {
        // std::cout << *dir << "\n";  // full path
        if(fs::is_regular(dir->path())) {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load daemon [" << dir->path().string() << "], reason = " << reason;
                // LOG_ERROR << ss.str();
                log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
                on_error(ss.str());
                return;
            }
        }
        // std::cout << dir->path().filename() << "\n"; // just last bit
    }

    std::string command_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    command_path += global_settings::Instance().GetSetting(DEFAULT_COMMANDS_PATH);
    ss.str("");
    ss << "Loading commands..: " << command_path;
    log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());

    for(fs::recursive_directory_iterator end, dir(command_path); dir != end; ++dir) {
        // std::cout << *dir << "\n";  // full path
        if(fs::is_regular(dir->path())) {
            std::string pathstr = dir->path().string();
            if(!entity_manager::Instance().compile_script_file(pathstr, reason)) {
                std::stringstream ss;
                ss << "Unable to load command [" << dir->path().string() << "], reason = " << reason;
                // LOG_ERROR << ss.str();
                log_interface::Instance().log(LOGLEVEL::LOGLEVEL_ERROR, ss.str());
                on_error(ss.str());
                return;
            }
        }
        // std::cout << dir->path().filename() << "\n"; // just last bit
    }
	
	 ss.str("");
    ss << "Loading rooms via the room cache..";
    log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());

    std::vector<std::string> r_cache;
    std::string r_cache_reason;

    // Load void room path for comparison purposes
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    void_script_path += global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);
    bool bVoidLoaded = false;

    if(entity_manager::Instance().load_room_cache(r_cache, r_cache_reason)) {
        std::string game_data_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
        for(auto r : r_cache) {
            std::string r_t = game_data_path + r;
            ss.str("");
            ss << "Loading room: " << r_t;
            log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, r_t, ss.str());
            if(void_script_path == r_t) {
                bVoidLoaded = true;
            }

            if(!entity_manager::Instance().compile_script_file(r_t, reason)) {
                std::stringstream ss;
                ss << "Unable to load room, reason = " << reason;
                log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, r_t, ss.str());
                if(void_script_path == r_t) {
                    on_error(ss.str());
                }
                return;
            }
        }
    }

    if(!bVoidLoaded) {
        // std::stringstream ss;
        ss.str("");
        ss << "Loading void room: " << void_script_path;
        log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());

        if(!entity_manager::Instance().compile_script_file(void_script_path, reason)) {
            std::stringstream ss;
            ss << "Unable to load void room, reason = " << reason;
            // LOG_ERROR << ss.str();
            log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, ss.str());
            on_error(ss.str());
            return;
        }
    }

    SetState(gameState::RUNNING);
}

bool game_manager::process_player_cmd(playerobj* p, std::string& cmd)
{

    if(m_state != gameState::RUNNING) {
		log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "State != RUNNING, unable to process command");
        return false;
    }

    // std::stringstream ss;
    // for testing purposes, just pull up my test player..
    // TODO: implement the actual logic..

    playerobj* pcaliel = entity_manager::Instance().get_player("caliel");
    // playerobj * precluse = entity_manager::Instance().get_player("recluse");
    // std::string wm_path = "workspaces/caliel/workroom";
    // roomobj * roomt = NULL;//entity_manager::Instance().GetRoomByScriptPath(wm_path, 0);

    if(pcaliel == NULL) {
        entity_manager::Instance().load_player("caliel");
        pcaliel = entity_manager::Instance().get_player("caliel");
        pcaliel->set_loggedIn(true);
    } else {
        pcaliel->set_loggedIn(true);
    }

    // if( precluse == NULL )
    // {
    //     entity_manager::Instance().load_player("recluse");
    //     pcaliel = entity_manager::Instance().get_player("recluse");
    // }

    // roomobj * roomt = get_void_room();
    // if( pcaliel->GetEnvironment() != NULL )
    //    roomt = pcaliel->GetRoom();

    // assert( roomt != NULL );

    // move caliel into the room...
    // roomt->AddEntityToInventory(pcaliel);
    if(entity_manager::Instance().do_command(p, cmd)) {
        entity_manager::Instance().garbage_collect();
    } else {
		log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Unable to execute command.");
    }

    return true;
}

bool game_manager::move_entity_into_room(script_entity* en, std::string room_path)
{
    return true;
}



game_manager::game_manager()
{
    m_state = gameState::STOPPED;
}

bool game_manager::start()
{
    return SetState(gameState::INITIALIZING);
}

bool game_manager::stop()
{
    return SetState(gameState::STOPPING);
}

void game_manager::do_heartbeats()
{
    entity_manager::Instance().invoke_heartbeat();
}

bool game_manager::SetState(gameState new_state)
{
    if(new_state == gameState::INITIALIZING) {
        if(m_state != gameState::STOPPED || m_state == gameState::INITIALIZING) {
            // do nothing if we aren't stopped, and
            // also do nothing if the state is already initializing
            return false;
        } else {
            m_state = new_state;
			log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Game state changed to INITIALIZING.");
            init();
            return true;
        }
    } else if(new_state == gameState::STOPPING) {
        if(m_state != gameState::RUNNING || m_state == gameState::STOPPING) {
            // do nothing if we aren't running, and
            // also do nothing if the state is already stopping
            return false;
        } else {
            m_state = new_state;
            //log->debug("Game state changed to STOPPING");
			log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Game state changed to STOPPING.");
            do_stop();
            return true;
        }
    } else if(new_state == gameState::RUNNING) {
        if(m_state == gameState::RUNNING) {
            // do nothing if we aren't running, and
            // also do nothing if the state is already stopping
            return false;
        } else {
			log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Game state changed to RUNNING.");
            m_state = new_state;
            return true;
        }
    } else if(new_state == gameState::STOPPED) {
        if(m_state == gameState::STOPPED) {
            // do nothing if we are already stopped, and
            // also do nothing if the state is already stopping
            return false;
        } else {
            m_state = new_state;
			log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Game state changed to STOPPED");
            return true;
        }
    } else if(new_state == gameState::ERROR) {

		log_interface::Instance().log(LOGLEVEL::LOGLEVEL_DEBUG, "Game state changed to ERROR");
        m_state = new_state;
        do_stop();

        return true;
    }
    assert(false);
    return false;
}

void game_manager::do_stop()
{
    entity_manager::Instance().reset();
    SetState(gameState::STOPPED);
}

void game_manager::on_error(std::string err)
{
    SetState(gameState::ERROR);
}
