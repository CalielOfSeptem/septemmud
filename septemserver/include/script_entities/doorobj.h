#ifndef DOOROBJ_H_
#define DOOROBJ_H_

#include "script_entities/itemobj.h"

struct roomobj;

struct doorobj : public itemobj
{
    doorobj()
    {
        
    }
    doorobj(sol::as_table_t<std::vector<std::string>>& door, const std::string& door_path, bool open=false, bool locked=false)
        : door_path(door_path)
    {
        this->door_path = door_path;
        this->set_isOpen(open);
        this->set_isLocked(locked);
        const auto& vex = door.source;
        for( auto ex : vex )
        {
            this->door.push_back(ex);
        }
    }
    
    const std::vector<std::string>& GetDoor()
    {
        return door;
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
    std::vector<std::string> door;
    roomobj * m_roomOwner;
};

#endif