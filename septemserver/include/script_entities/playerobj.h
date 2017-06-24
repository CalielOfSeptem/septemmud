#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
 
#include <memory>
#include "script_entities/livingentity.h"
#include "script_entities/container_base.h"
#include <stdio.h>

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
        if( GetName() == "caliel" )
            std::cout << msg << std::endl;
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
         return bIsCreator;
     }
     
     std::string& GetPlayerName()
     {
         return name;
     }
     
    virtual void on_environment_change(EnvironmentChangeEvent evt, script_entity * env) override
    {
        if( evt == EnvironmentChangeEvent::ADDED )
        {
            this->do_save();
        }
    }

     
     
     std::string cwd;
     std::string workspacePath;
     std::string roomPath;
     unsigned long roomID;



    bool bIsCreator;
    
    virtual bool do_save() override;
    virtual bool do_load() override;
    
    std::function<void(const std::string&)> onOutput;
    //Output onOutput;
    
};
 
#endif
