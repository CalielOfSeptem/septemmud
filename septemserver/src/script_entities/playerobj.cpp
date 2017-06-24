#include "script_entities/playerobj.h"
#include "fs/fs_manager.h"
#include <iomanip>
#include "entity_manager.h"
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
    do_load();
}
     

bool playerobj::do_save()
{
    json j;
    
    try
    {
        if( m_RightHand.GetItem() )
        {
            j["rightHand"] = std::map<std::string, std::string> {{ 
                    m_RightHand.GetItem()->get_uid(),
                    m_RightHand.GetItem()->GetPhysicalScriptPath()
                    }};
        }
        else
            j["rightHand"] = NULL;
            
        if( m_LeftHand.GetItem() )
           j["leftHand"] = std::map<std::string, std::string> {{ 
                    m_LeftHand.GetItem()->get_uid(),
                    m_LeftHand.GetItem()->GetPhysicalScriptPath(), 
                    }};
        else
            j["leftHand"] = NULL;
            
            
        std::vector< script_entity* > items = this->GetInventory();
        std::map< std::string, std::string > item_objs;
        for( auto i : items )
        {
            itemobj * obj = dynamic_cast< itemobj * > (i);
            item_objs[obj->get_uid()] = i->GetPhysicalScriptPath();
        }
 
        j["inventory"] = item_objs;
        
        j["cwd"] = this->cwd;
        
        if( this->get_gender() == EntityGender::MALE )
        {
            j["gender"] = "male";
        }
        else if( this->get_gender() == EntityGender::FEMALE )
        {
            j["gender"] = "female";
        }
        else if( this->get_gender() == EntityGender::UNKNOWN )
        {
            j["gender"] = "unknown";
        }
        
        
        if( this->GetEnvironment() )
        {
            j["roomPath"] = this->GetEnvironment()->GetPhysicalScriptPath();
            j["roomID"] = this->GetEnvironment()->instanceID;
        }

       // std::ofstream o(this->get_entityStorageLocation() + "/" + this->get_uid() );
       // o << std::setw(4) << j << std::endl;
            
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

bool playerobj::do_load()
{
    if( this->get_entityStorageLocation().size() == 0 )
        return false;
        
    try
    {
        std::ifstream i(this->get_entityStorageLocation() + "/player_save");
        json j;
        i >> j;
        const json& rh = j["rightHand"]; //<<<< this bit was hard to figure out
        for (auto& element : json::iterator_wrapper(rh)) {
            if( element.key().size() > 0 && element.value() != NULL )
            {
                //std::cout << element.key() << " maps to " << element.value() << std::endl;
                std::string s1 = element.key();
                std::string s2 = element.value();
                entity_manager::Instance().clone_item_to_hand( s2, &m_RightHand, s1);
            }

        }
       const json& lh = j["leftHand"]; //<<<< this bit was hard to figure out
       for (auto& element : json::iterator_wrapper(lh)) {
            if( element.key().size() > 0 && element.value() != NULL )
            {
               // std::cout << element.key() << " maps to " << element.value() << std::endl;
                std::string s1 = element.key();
                std::string s2 = element.value();
                entity_manager::Instance().clone_item_to_hand( s2, &m_LeftHand, s1);
            }

        }
                
        const json& inventory = j["inventory"]; //<<<< this bit was hard to figure out
        for (auto& element : json::iterator_wrapper(inventory)) {
            if( element.key().size() > 0 && element.value() != NULL )
            {
                //std::cout << element.key() << " maps to " << element.value() << std::endl;
                std::string s1 = element.key();
                std::string s2 = element.value();
                entity_manager::Instance().clone_item( s2, dynamic_cast<script_entity*>(this), s1 );
            }

        }
        
        if( j["gender"] == "male" )
        {
            this->set_gender(EntityGender::MALE);
        }
        else if( j["gender"] == "female" )
        {
            this->set_gender(EntityGender::FEMALE);
        }
        else if( j["gender"] == "unknown" )
        {
            this->set_gender(EntityGender::UNKNOWN);
        }
        
        if( this->GetEnvironment() )
        {
            this->roomPath = j["roomPath"];
            this->roomID = j["roomID"];
        }
    }
    catch(std::exception &ex)
    {
        this->debug(ex.what());
    }
    return true;
}
