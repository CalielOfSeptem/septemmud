#include "stdafx.h"
#include "account_manager.h"
#include "fs/fs_manager.h"



bool account_manager::get_accountExists(const std::string& name)
{
    return fs_manager::Instance().get_account_exists(name);
}

// key is now const&
std::string encrypt(std::string msg, std::string const& key)
{
    // Side effects if the following is not written:
    // In my case, division by 0.
    // In the other case, stuck in an infinite loop.
    if(!key.size())
        return msg;
    
    for (std::string::size_type i = 0; i < msg.size(); ++i)
        msg[i] ^= key[i%key.size()];
    return msg;
}

// Rewritten to use const& on both parameters
std::string decrypt(std::string const& msg, std::string const& key)
{
    return encrypt(msg, key); // lol
}

bool account_manager::create_account(const std::string& player_name, const std::string& pass, const std::string& email)
{
    // TODO: move this logic to the account object
    if( fs_manager::Instance().get_account_exists(player_name) )
    {
        return false; // account already exists.
    }
    std::string account_path;
    fs_manager::Instance().get_account_save_dir(boost::to_lower_copy(player_name), account_path, true); // this forces creation of the account folder
    

    try
    {
        json j;
        j[global_settings::Instance().GetSetting(ACCOUNT_PLAYERNAME)] = player_name;
       // std::string private_key_temp = "abc123456790";
        std::string penc = SHA256HashString(pass);
        j[global_settings::Instance().GetSetting(ACCOUNT_PASSWORD)] = penc;
        j[global_settings::Instance().GetSetting(ACCOUNT_EMAIL)] = email;
        j[global_settings::Instance().GetSetting(ACCOUNT_LASTLOGON)] = "";
        j[global_settings::Instance().GetSetting(ACCOUNT_TYPE)] = "player";
        j[global_settings::Instance().GetSetting(ACCOUNT_WORKSPACE)] = "";
        std::ofstream o( account_path + "/account" );
        o << j.dump(4) << std::endl;
    }
    catch( std::exception & ex )
    {
        auto log = spd::get("main");
        std::stringstream ss; 
        ss << "Error when attempting to create new account: " << ex.what();
        log->debug(ss.str());
        return false;
    }
 //   j["uid"] = this->get_uid();

    return true;
    
}

bool account_manager::load_account(const std::string& name, account& ac)
{
    ac._playername = name;
    if( ac.do_load() )
    {
        return true;
    }
    return false;
}
