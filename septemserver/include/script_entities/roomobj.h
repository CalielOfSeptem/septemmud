#ifndef ROOM_OBJ_H_
#define ROOM_OBJ_H_
#include "script_entity.h"
#include "script_entities/container_base.h"
//#include "script_entities/playerobj.h"
#include <sol.hpp>
#include <plog/Log.h>


struct exitobj
{
    exitobj()
    {
        
    }
    exitobj(sol::as_table_t<std::vector<std::string>>& exit, const std::string& exit_path, bool bobvious)
        : exit_path(exit_path)
    {
        this->bobvious = true;
        const auto& vex = exit.source;
        for( auto ex : vex )
        {
            this->exit.push_back(ex);
        }
    }
    
    void SetExit( const sol::as_table_t<std::vector<std::string>>& v, const std::string& exit_path, bool bobvious)
    {
        this->bobvious = bobvious;
        const auto& vex = v.source;
        for( auto ex : vex )
        {
            this->exit.push_back(ex);
        }
    }
    const std::vector<std::string>& GetExit()
    {
        return exit;
    }
    
    const std::string& GetExitPath()
    {
        return exit_path;
    }
    
    void SetObvious(const bool b)
    {
        bobvious = b;
    }
    
    void SetExitPath( const std::string& exit_path )
    {
        this->exit_path = exit_path;
    }
    
    void SetExitDesc( const sol::as_table_t<std::vector<std::string>>& v )
    {
        this->exit_path = exit_path;
        const auto& vex = v.source;
        for( auto ex : vex )
        {
            this->exit.push_back(ex);
        }
    }
    


private:
    std::string exit_path; // path the script the exit is linked to
    std::string bobvious; // whether the exit is shown by default
    std::vector<std::string> exit;
};

class roomobj : public script_entity, public container_base
{
public:
    roomobj(sol::this_state ts);
    
    ~roomobj();

    bool AddExit(sol::as_table_t<std::vector<std::string>> exit, const std::string& exit_path, bool obvious)
    {
        // TODO: add in validation code
        obvious_exits.push_back(exitobj(exit, exit_path, obvious));
        return true;
    }
    
    
    std::vector<exitobj>& GetExits()
    {
        return obvious_exits;
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
    
    virtual void AddEntityToInventory(script_entity * se)
    {
         se->SetEnvironment(static_cast<script_entity*>(this));
         container_base::AddEntityToInventory(se);
    }
     
    virtual bool RemoveEntityFromInventoryByID( const std::string& id  )
    {
        return container_base::RemoveEntityFromInventoryByID(id);
         // TODO: implement this and be sure to nuke a removed items environment_ pointer..
         
    }
    
    virtual bool RemoveEntityFromInventory( script_entity * se )
    {
        se->SetEnvironment(NULL);
        return container_base::RemoveEntityFromInventory(se);
    }
    
    std::vector<script_entity*> GetPlayers();
     

    
private:
    std::string title;
    std::string description;
    std::string short_description;

    std::vector<exitobj> obvious_exits;
};

#endif