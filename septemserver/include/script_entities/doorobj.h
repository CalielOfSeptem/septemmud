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
struct roomobj;

struct doorobj : public itemobj
{
    doorobj()
    {
        
    }
    doorobj(const std::string& door_name, const std::string& door_path, unsigned int door_id, bool open=false, bool locked=false);
 
    
    virtual void set_isOpen(bool b);
    
    const std::string& GetDoorPath();
    
    
    void SetDoorPath( const std::string& door_path );
    
    void SetRoomOwner(roomobj* r);
    
    roomobj * GetRoom();
 
    unsigned int get_doorID();
    
    void set_doorID(unsigned int doorid);
    
    roomobj * get_otherRoom();
    
    doorobj * get_otherDoor();

private:
    std::string door_path; // path the script the door is linked to
    roomobj * m_roomOwner;
    unsigned int m_doorID;
};

#endif