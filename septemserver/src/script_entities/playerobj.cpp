#include "script_entities/playerobj.h"
#include "fs/fs_manager.h"
#include <iomanip>
#include "json.hpp"
using json = nlohmann::json;

bool playerobj::do_save()
{
    std::string p;
    fs_manager::Instance().get_player_save_dir( this->name, p);
    p += "/player_save";
    json j;
    
    try
    {
        if( m_RightHand.GetItem() )
        {
            j["rightHand"] = std::map<std::string, std::string> {{ m_RightHand.GetItem()->GetBaseScriptPath(), m_RightHand.GetItem()->get_uid() }};
        }
        else
            j["rightHand"] = NULL;
            
        if( m_LeftHand.GetItem() )
           j["leftHand"] = std::map<std::string, std::string> {{ m_LeftHand.GetItem()->GetBaseScriptPath(), m_LeftHand.GetItem()->get_uid() }};
        else
            j["leftHand"] = NULL;
            
        std::ofstream o(p);
        o << std::setw(4) << j << std::endl;
    }
    catch( std::exception & ex )
    {
        // TODO: add in log output
    }

       
    return true;
}
     
     
