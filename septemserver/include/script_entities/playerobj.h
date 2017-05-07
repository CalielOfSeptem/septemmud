#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
 
#include <memory>
#include "script_entities/livingentity.h"
#include "script_entities/container_base.h"
#include <stdio.h>

struct playerobj : living_entity, container_base
{
     playerobj(sol::this_state ts, std::string name) :
        living_entity(ts, EntityType::PLAYER, name)
     {
         bIsCreator = true; // just for now, and for debug purposes
     }
     
     void SendToEntity(const std::string& msg)  override
     {
        //if( client_obj )
        //    (*client_obj).send(msg);
        if( GetName() == "caliel" )
            std::cout << msg << std::endl;
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
public:

    bool bIsCreator;
    
};
 
#endif