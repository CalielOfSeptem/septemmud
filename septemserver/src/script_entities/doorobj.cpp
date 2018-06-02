#include "stdafx.h"
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

doorobj::doorobj(const std::string& door_name,
                 const std::string& door_path,
                 unsigned int door_id,
                 bool open,
                 bool locked)
            : door_path(door_path), m_doorID(door_id)
{
    this->set_isOpen(open);
    this->set_isLocked(locked);
    SetName(door_name);
}

void doorobj::set_isOpen(bool b)
{
    if(get_isOpen() != b) {
        itemobj::set_isOpen(b);
        if(doorobj* dobj = this->get_otherDoor()) {
            dobj->set_isOpen(b);
        }
    }
}

const std::string& doorobj::GetDoorPath()
{
    return door_path;
}

void doorobj::SetDoorPath(const std::string& door_path)
{
    this->door_path = door_path;
}

void doorobj::SetRoomOwner(roomobj* r)
{
    m_roomOwner = r;
}

roomobj* doorobj::GetRoom()
{
    return m_roomOwner;
}

unsigned int doorobj::get_doorID()
{
    return m_doorID;
}

void doorobj::set_doorID(unsigned int doorid)
{
    m_doorID = doorid;
}
