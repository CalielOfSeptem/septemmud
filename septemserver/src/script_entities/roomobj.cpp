#include "script_entities/roomobj.h"
#include "entity_manager.h"

roomobj::roomobj(sol::this_state ts)
        : script_entity(ts, EntityType::ROOM)
{
    entity_manager::Instance().register_room(this);
}

roomobj::~roomobj()
{
    entity_manager::Instance().deregister_room(this);
    LOG_DEBUG << "Destroyed room object.";
}
    
