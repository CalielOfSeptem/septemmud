#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
 
#include <memory>
#include "script_entities/livingentity.h"
#include "script_entities/container_base.h"
#include <stdio.h>
#include "json.hpp"
#include "account.h"
#include "server/client.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"
namespace pt = boost::posix_time;


struct playerobj : living_entity 
{
    playerobj()
    {
    
    }
    playerobj(sol::this_state ts, sol::this_environment te, std::string name);
     
    virtual void debug(const std::string& msg) override
    {
        if( this->isCreator() )
            SendToEntity(msg);
    }
    
     
     void SendToEntity(const std::string& msg)  override
     {
        //if( GetName() == "caliel" )
        //    std::cout << msg << std::endl;
        if( this->onOutput )
            this->onOutput(msg);
        living_entity::SendToEntity(msg);
     }
     
     void SendToEnvironment(const std::string& msg) override
     {
         // TODO: implement this
         living_entity::SendToEnvironment(msg);
     }
     
     bool isCreator()
     {
         if( _ac._accountType == AccountType::CREATOR || _ac._accountType == AccountType::ARCH )
         {
             return true;
         
         }
         else
             return false;
             
     }
     
     
     
     std::string& GetPlayerName()
     {
         return name;
     }
     
     void set_playerName( const std::string& aname )
     {
         name = aname;
         //_ac._playername = name;
     }
     
    bool verify_password( const std::string& aname, const std::string& maybe_password );
     
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override
    {
        if( evt == EnvironmentChangeEvent::ADDED )
        {
            this->do_save();
        }
    }
     
     
    AccountType get_accountType()
    {
        return _ac._accountType;
    }
    
    void set_accountType(AccountType atype)
    {
        _ac._accountType = atype;
    }
    
    std::string get_workspacePath()
    {
        return _ac._workspacePath;
    }
    
    bool get_loggedIn()
    {
        return blogged_in;
    }

    void set_loggedIn(bool b)
    {
        blogged_in = b;
    }
    
    
    boost::posix_time::ptime get_referenceTickCount()
    {
        return tickReference;
    }
    
        
    boost::posix_time::ptime set_referenceTickCount()
    {
        tickReference = boost::posix_time::second_clock::local_time();
    }
    
    void incrementCmdCount()
    {
        commandCount++;
    }
    
    void resetCmdCount()
    {
        commandCount = 0;
    }
    
    unsigned long get_commandCount()
    {
        return commandCount;
    }
    
    unsigned long get_maxCmdCount()
    {
        return maxCmdCnt;
    }
    
    void set_client(client* c)
    {
        client_ = c;
    }
    
    client* get_client()
    {
        return client_;
    }
    
    
    void disconnect_client()
    {
        if( client_ != NULL )
            client_->disconnect();
    }
    
    
    
    
    virtual bool do_save() override;
    virtual bool do_load() override;
    
    std::function<void(const std::string&)> onOutput;
    //Output onOutput;

    std::string roomPath; // current room the player is in
    unsigned long roomID;  // current ID of the room the player is in
    std::string cwd;
    account _ac;
    
    bool blogged_in;

private:
    boost::posix_time::ptime lastCmdTick;
    boost::posix_time::ptime tickReference;
    unsigned long commandCount;
    unsigned long maxCmdCnt;
    client * client_;
};
 
#endif
