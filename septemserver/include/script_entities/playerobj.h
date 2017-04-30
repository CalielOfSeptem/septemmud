#ifndef PLAYER_OBJ_H_
#define PLAYER_OBJ_H_
 
#include <memory>
#include "script_entities/livingentity.h"
#include "script_entities/container_base.h"
#include <stdio.h>

struct playerobj : living_entity, container_base
{
     playerobj(sol::this_state ts) :
        living_entity(ts, EntityType::PLAYER)
     {

     }
     
     void SendToEntity(const std::string& msg)  override
     {
        //if( client_obj )
        //    (*client_obj).send(msg);
        std::cout << msg << std::endl;
        living_entity::SendToEntity(msg);
     }
     
     void SendToEnvironment(const std::string& msg) override
     {
         // TODO: implement this
         living_entity::SendToEnvironment(msg);
     }
     
public:
    //shared_ptr< client > client_obj;
    std::string player_name; //for now. TODO: implement full player details and support
    
    
};
 
#endif