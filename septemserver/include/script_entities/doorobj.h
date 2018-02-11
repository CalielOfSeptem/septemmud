// ==========================================================================
// doorobj.h
//
// Copyright (C) 2018 Kenneth Thompson, All Rights Reserved.
// This file is covered by the MIT Licence:
//
// Permission is hereby granted, free of charge, to any person obtaining a 
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the 
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
// ==========================================================================
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