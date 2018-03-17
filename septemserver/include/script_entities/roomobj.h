// ==========================================================================
// roomobj.h
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
#ifndef ROOM_OBJ_H_
#define ROOM_OBJ_H_
#include "script_entity.h"
#include "script_entities/container_base.h"

struct exitobj;
struct lookobj;
struct doorobj;
struct itemobj;

enum RoomType : int { INDOOR=0, OUTDOOR };

class roomobj : public script_entity, public container_base
{
public:
    roomobj(sol::this_state ts, sol::this_environment te, int rt);
    
    ~roomobj();

    exitobj * AddExit(sol::as_table_t<std::vector<std::string>> exit, const std::string& exit_path, bool obvious);
    
    lookobj * AddLook(sol::as_table_t<std::vector<std::string>> look, const std::string& description);
    
    doorobj * AddDoor(const std::string& door_name, const std::string& door_path, const unsigned int door_id, bool open=true, bool locked=false);
    
    
    virtual script_entity * GetOwner() override;
    
    std::vector<std::shared_ptr<exitobj>>& GetExits();
    
    std::vector<std::shared_ptr<doorobj>>& GetDoors();
    
    doorobj * GetDoorByID(const unsigned int did);
    
    std::vector<std::shared_ptr<lookobj>>& GetLooks();

    void SetTitle(const std::string& title);
    
    const std::string& GetTitle();

    void SetDescription(const std::string& description);
    
    const std::string& GetDescription();
    
    void SetShortDescription(const std::string& short_description);
    
    const std::string& GetShortDescription();
    
    virtual bool AddEntityToInventory(script_entity * se) override;
     
    virtual bool RemoveEntityFromInventoryByID( const std::string& id  ) override;
    
    virtual bool RemoveEntityFromInventory( script_entity * se ) override;
    
    void SendToRoom(const std::string& msg);
    
    std::vector<script_entity*> GetPlayers(const std::string& name);
    
    std::vector<itemobj*> GetItems();
    
    virtual void debug( const std::string& msg ) override;
    
        
    bool GetIsOutdoor();

    
private:
    std::string title;
    std::string description;
    std::string short_description;

    std::vector<std::shared_ptr<exitobj>> obvious_exits;
    std::vector<std::shared_ptr<lookobj>> looks;
    std::vector<std::shared_ptr<doorobj>> doors;
    
    RoomType room_type;
};

#endif