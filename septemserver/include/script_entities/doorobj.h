#ifndef DOOROBJ_H_
#define DOOROBJ_H_

#include "script_entities/itemobj.h"
//#include "script_entities/roomobj.h"

struct roomobj;

struct doorobj : public itemobj
{
    doorobj()
    {
        
    }
    doorobj(const std::string& door_name, const std::string& door_path, unsigned int door_id, bool open=false, bool locked=false)
        : door_path(door_path), m_doorID(door_id)
    {
        this->set_isOpen(open);
        this->set_isLocked(locked);
        SetName(door_name);
    }
    
    virtual void set_isOpen(bool b)
    {
        if( get_isOpen() != b )
        {
            itemobj::set_isOpen(b);
            if( doorobj * dobj = this->get_otherDoor() )
            {
                dobj->set_isOpen(b);
            }
        }
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
 
    unsigned int get_doorID()
    {
        return m_doorID;
    }
    
    void set_doorID(unsigned int doorid)
    {
        m_doorID = doorid;
    }
    
    roomobj * get_otherRoom();
    
    doorobj * get_otherDoor()
    ;

private:
    std::string door_path; // path the script the door is linked to
    roomobj * m_roomOwner;
    unsigned int m_doorID;
};

#endif