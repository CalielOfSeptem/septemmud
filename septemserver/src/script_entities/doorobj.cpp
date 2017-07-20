#include "script_entities/doorobj.h"
#include "script_entities/roomobj.h"
#include "entity_manager.h"


roomobj* doorobj::get_otherRoom()
{
    return entity_manager::Instance().GetRoomByScriptPath(door_path);
}

doorobj* doorobj::get_otherDoor()
{
    if(roomobj* troom = get_otherRoom()) {
        return troom->GetDoorByID(m_doorID);
    }
    return NULL;
}
