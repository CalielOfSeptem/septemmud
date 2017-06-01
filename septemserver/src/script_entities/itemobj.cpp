#include "script_entities/itemobj.h"
#include "entity_manager.h"
#include "string_utils.h"
#include "fs/fs_manager.h"
#include <iomanip>
#include "json.hpp"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

using json = nlohmann::json;


itemobj::itemobj(sol::this_state ts, sol::this_environment te, std::string name, ItemType itype) :
    script_entity(ts, te, EntityType::ITEM, name)
{
    SetName(name);
    entity_manager::Instance().register_item(this);
}

void itemobj::on_environment_change(EnvironmentChangeEvent evt, script_entity * env)
{
    if( evt == EnvironmentChangeEvent::REMOVED )
    {
        try
        {
            if( fs::exists(this->get_entityStorageLocation() + "/" + this->get_uid() ) )
                fs::remove(this->get_entityStorageLocation() + "/" + this->get_uid());
        }
        catch(std::exception& ex)
        {
            debug(ex.what());
        }
    }
    else if( evt == EnvironmentChangeEvent::ADDED )
    {
        std::string location = env->get_entityStorageLocation();
        this->set_entityStorageLocation(location);
        do_save();
    }
}

bool itemobj::do_save()
{
    if( this->get_entityStorageLocation().size() == 0 )
        return false;
        
    json j;
    
    try
    {
        j["uid"] = this->get_uid();
        j["base_script_path"] = GetBaseScriptPath();
        j["script_path"] = GetScriptPath();
        j["look"] = GetLook();
        j["name"] = script_entity::GetName();
        
        j["weight"] = get_weight();
        j["size"] = get_size();
        j["item_type"] = get_itemType();
        j["defaultStackSize"] = get_defaultStackSize();
        j["currentStackCount"] = get_currentStackCount();
        j["isWearable"] = get_isWearable();
        j["isStackable"] = get_isStackable();
        j["isContainer"] = get_isContainer();
        j["pluralName"] = get_pluralName();

        std::ofstream o(this->get_entityStorageLocation() + "/" + this->get_uid() );
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

