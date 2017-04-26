#include "game_manager.h"
#include "config.h"
#include <sstream>
#include "global_settings.h"
#include "entity_manager.h"

void game_manager::init()
{
    LOG_DEBUG << "Begin Init";
    
    // Load void room
    std::string void_script_path = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH);
    void_script_path += global_settings::Instance().GetSetting(DEFAULT_VOID_ROOM);
    
    LOG_DEBUG << "Loading void room: " << void_script_path;
    std::string reason;
    if( !entity_manager::Instance().compile_script(void_script_path, reason) )
    {
        std::stringstream ss;
        ss << "Unable to load void room, reason = " << reason;
        LOG_ERROR << ss.str();
        on_error(ss.str());
    }
    else
    {
        SetState(gameState::RUNNING); 
    }
   
}
