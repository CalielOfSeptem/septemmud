#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
//#include <cereal/archives/xml.hpp>
//#include <cereal/types/string.hpp>
#include <string.h>
//#include <cereal/types/vector.hpp>
#include "json.hpp"

using json = nlohmann::json;


enum AccountType { UNKNOWN_TYPE, PLAYER, CREATOR, ARCH };


class account
{
public:
    account(const std::string& playername);
    account() {};
    
    //~account();

    
    std::string get_accountStorageLocation();
   
    static std::string AccountTypeToString(AccountType at);
    
    bool do_load();
    
    bool do_save();
    
    std::string _playername;
    std::string _email;
    std::string _secure_password;
    
    std::string _lastLogon;
    AccountType _accountType;
    nlohmann::json _j;
    
    std::string _cwd; // current directory the player is targetting
    std::string _workspacePath;  // path to the player's workspace
};

#endif // ACCOUNT_HPP
