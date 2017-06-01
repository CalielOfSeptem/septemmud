#include "script_entities/playerobj.h"
#include "fs/fs_manager.h"
#include <iomanip>
#include "json.hpp"
using json = nlohmann::json;

playerobj::playerobj(sol::this_state ts, sol::this_environment te, std::string name) :
living_entity(ts, te, EntityType::PLAYER, name)
{
    bIsCreator = true; // just for now, and for debug purposes
    std::string p;
   // p += "/player_save";
    fs_manager::Instance().get_player_save_dir( name, p);
    this->set_entityStorageLocation( p );
    m_RightHand.set_entityStorageLocation(p);
    m_LeftHand.set_entityStorageLocation(p);
}
     

bool playerobj::do_save()
{
    json j;
    
    try
    {
        if( m_RightHand.GetItem() )
        {
            j["rightHand"] = std::map<std::string, std::string> {{ 
                    m_RightHand.GetItem()->GetBaseScriptPath(), 
                    m_RightHand.GetItem()->get_uid() }};
        }
        else
            j["rightHand"] = NULL;
            
        if( m_LeftHand.GetItem() )
           j["leftHand"] = std::map<std::string, std::string> {{ 
                    m_LeftHand.GetItem()->GetBaseScriptPath(), 
                    m_LeftHand.GetItem()->get_uid() }};
        else
            j["leftHand"] = NULL;
            
        std::ofstream o(this->get_entityStorageLocation() + "/player_save");
        o << std::setw(4) << j << std::endl;
    }
    catch( std::exception & ex )
    {
        std::stringstream ss;
        ss << "Error during save: " << ex.what();
        this->debug(ss.str());
        return false;
    }

       
    return true;
}
     
     
