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
#include "script_entities/doorobj.h"
#include "script_entities/lookobj.h"
#include "script_entities/exitobj.h"

//#include "script_entities/playerobj.h"
#include <sol.hpp>

struct itemobj;




class roomobj : public script_entity, public container_base
{
public:
    roomobj(sol::this_state ts, sol::this_environment te);
    
    ~roomobj();

    exitobj * AddExit(sol::as_table_t<std::vector<std::string>> exit, const std::string& exit_path, bool obvious)
    {
        // TODO: add in validation code
        obvious_exits.push_back(std::shared_ptr<exitobj>(new exitobj(exit, exit_path, obvious)));
        return obvious_exits[obvious_exits.size()-1].get();
    }
    
    lookobj * AddLook(sol::as_table_t<std::vector<std::string>> look, const std::string& description)
    {
        looks.push_back(std::shared_ptr<lookobj>(new lookobj(look, description)));
        return looks[looks.size()-1].get();
    }
    
    doorobj * AddDoor(const std::string& door_name, const std::string& door_path, const unsigned int door_id, bool open=true, bool locked=false);
    
    
    virtual script_entity * GetOwner() override
    {
        return this;
    }
    
    
    std::vector<std::shared_ptr<exitobj>>& GetExits()
    {
        return obvious_exits;
    }
    
    std::vector<std::shared_ptr<doorobj>>& GetDoors()
    {
        return doors;
    }
    
    doorobj * GetDoorByID(const unsigned int did)
    {
        for( auto d : this->doors )
        {
            if( d->get_doorID() == did )
                return d.get();
        }
        return NULL;
    }
    
    std::vector<std::shared_ptr<lookobj>>& GetLooks()
    {
        return looks;
    }


    void SetTitle(const std::string& title)
    {
        this->title = title;
    }
    const std::string& GetTitle()
    {
        return title;
    }

    void SetDescription(const std::string& description)
    {
        this->description = description;
    }
    const std::string& GetDescription()
    {
        return description;
    }
    
    void SetShortDescription(const std::string& short_description)
    {
        this->short_description = short_description;
    }
    const std::string& GetShortDescription()
    {
        return short_description;
    }
    
    virtual bool AddEntityToInventory(script_entity * se) override
    {
         return container_base::AddEntityToInventory(se);
        // se->SetEnvironment( static_cast<script_entity*>(this) );
         //se->SetEnvironment(static_cast<script_entity*>(this)); // this must be called AFTER the above (to remove the se from its previous owner)
    }
     
    virtual bool RemoveEntityFromInventoryByID( const std::string& id  ) override
    {
        return container_base::RemoveEntityFromInventoryByID(id);
         // TODO: implement this and be sure to nuke a removed items environment_ pointer..
         
    }
    
    virtual bool RemoveEntityFromInventory( script_entity * se ) override
    {
        //se->SetEnvironment(NULL);
        return container_base::RemoveEntityFromInventory(se);
    }
    
    void SendToRoom(const std::string& msg);
    
    std::vector<script_entity*> GetPlayers(const std::string& name);
    
    std::vector<itemobj*> GetItems();
    
    virtual void debug( const std::string& msg ) override;
    
        


    
private:
    std::string title;
    std::string description;
    std::string short_description;

    std::vector<std::shared_ptr<exitobj>> obvious_exits;
    std::vector<std::shared_ptr<lookobj>> looks;
    std::vector<std::shared_ptr<doorobj>> doors;
    

};

#endif