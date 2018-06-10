#include "stdafx.h"
#include "account.h"
#include "fs/fs_manager.h"
//#include "config.h"

std::string account::get_accountStorageLocation()
{
    std::string tpath = "";
    fs_manager::Instance().get_account_save_dir(boost::to_lower_copy(_playername), tpath);
    tpath += "/";
    return tpath;
}

bool account::do_compare(std::string pass)
{
    std::string tmp = SHA256HashString(pass);
    if( tmp == _secure_password )
    {
        return true;
    }
    return false;
}

bool account::do_load()
{
    try {
        // I'm not a fan of this particular method of account persistence, but it will work for now\
        // perhaps in the future a better method would include a database
        
        std::string s = this->get_accountStorageLocation();
 
        std::ifstream i(s + "account"); // name of the account file
        if( !i.is_open() )
        {
            return false;
        }
        i >> _j;
        std::string pname = _j["player_name"];
        if(boost::to_lower_copy(pname) != boost::to_lower_copy(_playername)) {
            std::stringstream ss;
            ss << "Player name does not match.  Expected: " << _playername << "got: " << pname;
            throw std::runtime_error(ss.str());
        }

        std::string ptype = _j[global_settings::Instance().GetSetting(ACCOUNT_TYPE)];
        if(boost::to_lower_copy(ptype) == "player") {
            _accountType = AccountType::PLAYER;
        } else if(boost::to_lower_copy(ptype) == "creator") {
            _accountType = AccountType::CREATOR;
        } else if(boost::to_lower_copy(ptype) == "arch") {
            _accountType = AccountType::ARCH;
        } else {
            _accountType = AccountType::UNKNOWN_TYPE;
        }
		
		std::string gtype = _j[global_settings::Instance().GetSetting(ACCOUNT_GENDER)];
        if(boost::to_lower_copy(gtype) == "male") {
			_entityGender = EntityGender::MALE;
        } else if(boost::to_lower_copy(gtype) == "female") {
            _entityGender = EntityGender::FEMALE;
        }
		else{
            _entityGender = EntityGender::UNKNOWN;
        }

        _email = _j[global_settings::Instance().GetSetting(ACCOUNT_EMAIL)];
        _lastLogon = _j[global_settings::Instance().GetSetting(ACCOUNT_LASTLOGON)];
        _secure_password = _j[global_settings::Instance().GetSetting(ACCOUNT_PASSWORD)];
        if( _j[global_settings::Instance().GetSetting(ACCOUNT_WORKSPACE)].is_null() || _j[global_settings::Instance().GetSetting(ACCOUNT_WORKSPACE)] == "" )
        {
            _workspacePath = global_settings::Instance().GetSetting(DEFAULT_WORKSPACES_PATH) + "/" + boost::to_lower_copy(pname);
        }
        else
        {
            _workspacePath = _j[global_settings::Instance().GetSetting(ACCOUNT_WORKSPACE)];
        }

    } catch(std::exception& ex) {
 
        std::stringstream ss;
        ss << "Error when attempting to load account " << _playername << ": " << ex.what();
        log_interface::Instance().log(LOGLEVEL::LOGLEVEL_CRITICAL, ss.str());
        return false;
    }
    return true;
}

std::string account::AccountTypeToString(AccountType at)
{
    switch( at )
    {
        case AccountType::ARCH:
        {
            return "arch";
            break;
        }
        case AccountType::CREATOR:
        {
            return "creator";
            break;
        }
        case AccountType::PLAYER:
        {
            return "player";
            break;
        }
        default:
        return "unknown";
    }
    return "";
}

bool account::do_save()
{

    json j;

    try {
        if(_accountType == AccountType::ARCH) {
            j[global_settings::Instance().GetSetting(ACCOUNT_TYPE)] = "arch";
        } else if(_accountType == AccountType::CREATOR) {
            j[global_settings::Instance().GetSetting(ACCOUNT_TYPE)] = "creator";
        } else if(_accountType == AccountType::PLAYER) {
            j[global_settings::Instance().GetSetting(ACCOUNT_TYPE)] = "player";
        } else {
            j[global_settings::Instance().GetSetting(ACCOUNT_TYPE)] = "unknown";
        }
		
		
        if( _entityGender == EntityGender::MALE) {
            j[global_settings::Instance().GetSetting(ACCOUNT_GENDER)] = "male";
        } else if(_entityGender == EntityGender::FEMALE) {
            j[global_settings::Instance().GetSetting(ACCOUNT_GENDER)] = "female";
        } else {
            j[global_settings::Instance().GetSetting(ACCOUNT_GENDER)] = "unknown";
        }

        //assert( !_email.empty() );
        j[global_settings::Instance().GetSetting(ACCOUNT_EMAIL)] = _email;
        
        
      //  assert( !_secure_password.empty() );
        j[global_settings::Instance().GetSetting(ACCOUNT_PASSWORD)] = _secure_password;
        
        
        j[global_settings::Instance().GetSetting(ACCOUNT_LASTLOGON)] = _lastLogon;
        
        //assert(!_playername.empty());
        j[global_settings::Instance().GetSetting(ACCOUNT_PLAYERNAME)] = _playername;
       
        
        j[global_settings::Instance().GetSetting(ACCOUNT_WORKSPACE)] = _workspacePath;

        // std::ofstream o(this->get_entityStorageLocation() + "/" + this->get_uid() );
        // o << std::setw(4) << j << std::endl;

        std::ofstream o(this->get_accountStorageLocation() + "/account");
        if( !o.is_open() )
        {
            return false;
        }
        o << std::setw(4) << j << std::endl;
    } catch(std::exception& ex) {
        
        std::stringstream ss;
        ss << "Error when attempting to save account " << _playername << ": " << ex.what();
        log_interface::Instance().log(LOGLEVEL::LOGLEVEL_CRITICAL, ss.str());

        return false;
    }

    return true;
}
