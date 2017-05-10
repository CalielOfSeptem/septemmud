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
    entity_manager::Instance().deregister_room(this);
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

void roomobj::debug(sol::this_state ts, const std::string& msg)
{
    for(auto obj : GetInventory()) {
        if(obj->GetType() == EntityType::PLAYER) {
            playerobj * p = dynamic_cast<playerobj*>(obj);
            if( p->isCreator() )
                p->SendToEntity("DEBUG: " + msg);
        }
    }
    script_entity::debug(ts, msg);
}
