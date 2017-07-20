#include "script_entities/roomobj.h"
#include "entity_manager.h"
#include <boost/algorithm/string/case_conv.hpp>

roomobj::roomobj(sol::this_state ts, sol::this_environment te)
    : script_entity(ts, te, EntityType::ROOM, "")
{
    entity_manager::Instance().register_room(this);
}

roomobj::~roomobj()
{
    // entity_manager::Instance().deregister_room(this);
}

std::vector<script_entity*> roomobj::GetPlayers(const std::string& name)
{
    std::vector<script_entity*> players;
    for(auto obj : GetInventory()) {
        if(obj->GetType() == EntityType::PLAYER) {
            // playerobj * p = dynamic_cast<playerobj*>(obj);
            if(boost::to_lower_copy(obj->GetName()) != boost::to_lower_copy(name)) {
                players.push_back(obj);
            }
        }
    }
    return players;
}

std::vector<itemobj*> roomobj::GetItems()
{
    std::vector<itemobj*> items;
    for(auto obj : GetInventory()) {
        if(obj->GetType() == EntityType::ITEM) {
            // playerobj * p = dynamic_cast<playerobj*>(obj);
            // if(boost::to_lower_copy(obj->GetName()) != boost::to_lower_copy(name)) {
            items.push_back(static_cast<itemobj*>(obj));
            // }
        }
    }
    return items;
}

void roomobj::debug(const std::string& msg)
{
    for(auto obj : GetInventory()) {
        if(obj->GetType() == EntityType::PLAYER) {
            playerobj* p = dynamic_cast<playerobj*>(obj);
            if(p->isCreator())
                p->SendToEntity("DEBUG: " + msg);
        }
    }
    script_entity::debug(msg);
}

void roomobj::SendToRoom(const std::string& msg)
{
    for(auto obj : GetInventory()) {
        if(obj->GetType() == EntityType::PLAYER || obj->GetType() == EntityType::NPC) {
            living_entity* p = dynamic_cast<living_entity*>(obj);
            p->SendToEntity(msg);
            // if( p->isCreator() )
            //    p->SendToEntity("DEBUG: " + msg);
        }
    }
}

doorobj* roomobj::AddDoor(const std::string& door_name, const std::string& door_path, const unsigned int door_id, bool open, bool locked)
{
    // TODO: add in validation code
    std::shared_ptr<doorobj> new_door = std::shared_ptr<doorobj>(new doorobj(door_name, door_path, door_id, open, locked));
    std::string doorp = door_path;
    roomobj* maybe_otherside = entity_manager::Instance().GetRoomByScriptPath(doorp);
    if(maybe_otherside != NULL) {
       // new_door->set_otherSide(maybe_otherside);
        //new_door->set_isMaster(false);
        
        if( doorobj * dobj = maybe_otherside->GetDoorByID(door_id) )
        {
           // dobj->set_otherSide(this);
           
        }
        else
        {
            // add it in.
            doorobj * tdoor = maybe_otherside->AddDoor(door_name, this->GetInstancePath(), door_id, open, locked);
        
        }
        
        //maybe_otherside->set_otherSide(new_door); // first door intantiated wins!
        //maybe_otherside->set_isMaster(true);
    } else {
        //new_door->set_otherSide(NULL);
        //new_door->set_isMaster(true);
    }

    new_door->SetRoomOwner(this);
    doors.push_back(new_door);

    return new_door.get();
}
