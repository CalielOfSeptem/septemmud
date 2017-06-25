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
            this->remove_settings_file();
                
            // if this is a container, remove their files as well..
            
            for( auto it : this->GetItems() )
            {
                it->remove_settings_file();
                std::string l = "";
                it->set_entityStorageLocation(l);
            }

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
        
        if( !location.empty()  )
        {
            do_load();
            set_isInitialized(true);
            
            for( auto it : this->GetItems() )
            {
                it->set_entityStorageLocation( location );
                it->do_load();
            }
        }

        
        //do_save();
    }
}

bool itemobj::do_load()
{
    if( this->get_entityStorageLocation().empty())
        return false;
        
    try
    {
        if( !fs::exists(this->get_entityStorageLocation() + "/" + this->uid) )
        {
            this->do_save();
            return true;
        }
        std::ifstream i(this->get_entityStorageLocation() + "/" + this->uid);
        json j;
        i >> j;
        _load_from_json_(j);


    }
    catch(std::exception &ex)
    {
        this->debug(ex.what());
    }
    return true;
}

bool itemobj::_load_from_json_(json& j)
{
    SetLook(j["look"]);
    SetName(j["name"]);
    
    set_weight(j["weight"]);
    set_size(j["size"]);
    set_itemType(j["item_type"]);
    
    set_defaultStackSize(j["defaultStackSize"]);
    set_currentStackCount(j["currentStackCount"]);
    set_isWearable(j["isWearable"]);
    set_isStackable(j["isStackable"]);
    set_isContainer(j["isContainer"]);
    set_pluralName( j["pluralName"]);
    
    set_isOpen(j["isOpen"]);
    set_isLocked(j["isLocked"]);
    set_isLockable(j["isLockable"]);
    
    
    const json& inventory = j["inventory"]; //<<<< this bit was hard to figure out
    for (auto& element : json::iterator_wrapper(inventory)) {
        if( element.key().size() > 0 && element.value() != NULL )
        {
            //std::cout << element.key() << " maps to " << element.value() << std::endl;
            std::string s1 = element.key();
            std::string s2 = element.value();
            if( entity_manager::Instance().clone_item( s2, dynamic_cast<script_entity*>(this), s1 ) == NULL )
            {
                std::string err = "Error while attempting to clone item.";
                this->debug( err );
            }
        }

    }
    return true;
}

bool itemobj::do_save()
{
    if( this->get_entityStorageLocation().size() == 0 )
        return false;
 
    try
    {
        std::string j = Serialize();
        std::ofstream o(this->get_entityStorageLocation() + "/" + this->get_uid() );
        //o << std::setw(4) << j << std::endl;
        o << j << std::endl;
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

bool itemobj::do_json_load(std::string& j)
{
    if( j.empty() )
        return false;
    json js = json::parse(j);
    return _load_from_json_(js);
}

bool itemobj::AddEntityToInventory(script_entity* se)
{
    bool b = container_base::AddEntityToInventory(se);
    this->do_save();
    return b;
}

bool itemobj::RemoveEntityFromInventory(script_entity* se)
{
    bool b = container_base::RemoveEntityFromInventory(se);
    this->do_save();
    return b;
}

std::vector<itemobj*> itemobj::GetItems()
{
    std::vector<script_entity*> ents = GetInventory();
    std::vector<itemobj*> t_items;
    for( auto i : ents )
    {
        t_items.push_back(dynamic_cast<itemobj*>(i));
    }
    return t_items;
}

bool itemobj::remove_settings_file()
{
    if( this->get_entityStorageLocation().empty() )
        return false;
    try
    {
        if( fs::exists(this->get_entityStorageLocation() + "/" + this->get_uid() ) )
            fs::remove(this->get_entityStorageLocation() + "/" + this->get_uid() ); 
    }
    catch( std::exception& ex )
    {
        this->debug(ex.what());
        return false;
    }
    return true;
}

std::string itemobj::Serialize()
{
    json j;
    j["uid"] = this->get_uid();
    j["physical_script_path"] = GetPhysicalScriptPath();
    j["virtual_script_path"] = GetVirtualScriptPath();
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
    
    j["isOpen"] = get_isOpen();
    j["isLocked"] = get_isLocked();
    j["isLockable"] = get_isLockable();
    
    
    std::vector< script_entity*  > items = this->GetInventory();
    std::map< std::string, std::string > item_objs;
    for( auto i : items )
    {
        itemobj * obj = dynamic_cast< itemobj * > (i);
        item_objs[obj->get_uid()] = i->GetPhysicalScriptPath();
    }

    j["inventory"] = item_objs;
    return j.dump(4);
}
