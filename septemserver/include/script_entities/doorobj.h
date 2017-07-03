#ifndef DOOROBJ_H_
#define DOOROBJ_H_

#include "script_entities/itemobj.h"

struct roomobj;

struct doorobj : public itemobj
{
    doorobj()
    {
        
    }
    doorobj(const std::string& door_name, const std::string& door_path, bool open=false, bool locked=false)
        : door_path(door_path)
    {
        //this->door_path = door_path;
        this->set_isOpen(open);
        this->set_isLocked(locked);
        SetName(door_name);
    }
    
    const std::string& GetDoorPath()
    {
        return door_path;
    }
    
    
    void SetDoorPath( const std::string& door_path )
    {
        this->door_path = door_path;
    }
    
    void SetRoomOwner(roomobj* r)
    {
        m_roomOwner = r;
    }
    
    roomobj * GetRoom()
    {
        return m_roomOwner;
    }

private:
    std::string door_path; // path the script the door is linked to
    roomobj * m_roomOwner;
};

#endif