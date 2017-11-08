#include "script_entities/playerobj.h"
#include "fs/fs_manager.h"
#include <iomanip>
#include "entity_manager.h"

using json = nlohmann::json;

playerobj::playerobj(sol::this_state ts, sol::this_environment te, std::string name) :
living_entity(ts, te, EntityType::PLAYER, name)
{
    std::string p;
   // p += "/player_save";
    blogged_in = false;
    maxCmdCnt = 2;
    commandCount = 0;
    tickReference = boost::posix_time::second_clock::local_time();
    fs_manager::Instance().get_player_save_dir( name, p);
    this->set_entityStorageLocation( p );
    m_RightHand.set_entityStorageLocation(p);
    m_LeftHand.set_entityStorageLocation(p);
    do_load();
}
     

bool playerobj::do_save()
{
 
    if( !get_loggedIn() )
    {
        return true;
    }   
    
    try
    {

        json j;
        _ac.do_save();
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
       
        std::string p;
        fs_manager::Instance().get_player_save_dir( GetPlayerName(), p, true); // make sure the directory gets created
        //this->set_entityStorageLocation( p );
    
            
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
    if( !get_loggedIn() )
    {
        return true;
    }
    if( this->get_entityStorageLocation().size() == 0 )
        return false;
        
    try
    {

        _ac._playername = GetPlayerName(); // maybe put this elsewhere.. undecided.
        _ac.do_load();
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
        //if( j["cwd"] )
        this->cwd = j["cwd"];
        
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
        

        this->roomPath = j["roomPath"];
        this->roomID = j["roomID"];
        
    }
    catch(std::exception &ex)
    {
        this->debug(ex.what());
    }
    return true;
}

bool playerobj::verify_password(const std::string& aname, const std::string& maybe_password)
{
    account _taccount;
    _taccount._playername = aname;
    if( !_taccount.do_load() )
    {
        return false;
    }
    if( _taccount.do_compare(maybe_password) )
    {
        return true;
    }
    else
    {
        return false;
    }
}





/*
bool playerobj::capture_input(sol::this_state ts, sol::protected_function pf)
{
    sol::state_view lua_state = sol::state_view(ts);
    sol::environment target_env(sol::env_key, pf);
    sol::optional<std::string> script_path = target_env["_INTERNAL_SCRIPT_PATH_"];
    //sol::optional<std::string> script_path = target_env["_INTERNAL_PHYSICAL_SCRIPT_PATH_"];
   // sol::optional<std::string> e_type = target_env["_INTERNAL_ENTITY_TYPE_"];
   
    if( script_path )
    {
        std::cout << script_path.value() << std::endl;
        if( script_path.value().empty() )
            return false; // yeah.. don't go trying to use an empty value

        pf_capture_input = pf;
        capture_input_script_path = script_path.value();
        //binputCaptured = true;
        return true;
    }
    else
    {
        return false;
    }
}

bool playerobj::execute_input_capture(sol::this_state ts, const std::string& input)
{
    sol::state_view lua_state = sol::state_view(ts);
    if( !binputCaptured || capture_input_script_path.empty() )
    {
        return false;
    }
    try 
    {

        lua_state.set_function("debug_hook", [this, lua_state](sol::object o) -> void 
        { 
            sol::optional<std::string> spath = capture_input_script_path;
            sol::optional<std::string> err = o.as<std::string>();
            if( err && err.value() == "count" )
            {
                std::string s = "Infinite loop detected. Operation terminated.";
                lua_pushstring( lua_state.lua_state(), s.c_str());
                lua_error( lua_state.lua_state() );
               // return false;
            }
            else
            {
                std::string s = "Error. Reason = " + err.value();
                lua_pushstring( lua_state.lua_state(), s.c_str());
                lua_error( lua_state.lua_state() );
                //return false;
            }

        });

        
        lua_state.script("debug.sethook (debug_hook, '', 10000)");
        auto result = pf_capture_input(input);
        if(!result.valid()) {
            sol::error err = result;
            std::cout << err.what() << std::endl;
            // TODO: log this to file
        }
        lua_state.script("debug.sethook ()");
    } 
    catch(std::exception& ex) {
        std::cout << ex.what();
        // TODO: log this to file
        return false;
    }
    return true;
}
*/
    /*
    unsigned int register_heartbeat_func(sol::state_view lua_state, sol::protected_function pf)
    {
        sol::environment target_env(sol::env_key, pf);
        sol::optional<std::string> script_path = target_env["_INTERNAL_SCRIPT_PATH_"];
        //sol::optional<std::string> script_path = target_env["_INTERNAL_PHYSICAL_SCRIPT_PATH_"];
        sol::optional<std::string> e_type = target_env["_INTERNAL_ENTITY_TYPE_"];
       
        if( script_path )
        {
             std::cout << script_path.value() << std::endl;
            if( script_path.value().empty() )
                return -1; // yeah.. don't go trying to use an empty value
           // std::cout << script_path.value();
            bindings.insert( {++bindingId, state_wrapper (lua_state, pf, script_path.value(), e_type.value())});
        }
        else
        {
            return -1;
        }
        
        return bindingId;
    }
     */
