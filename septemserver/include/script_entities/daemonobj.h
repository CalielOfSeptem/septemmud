#ifndef DAEMONOBJ_H_
#define DAEMONOBJ_H_

#include <string>
#include <vector>
#include "script_entities/script_entity.h"

struct daemonobj : script_entity
{
    daemonobj(sol::this_state ts)
        : script_entity(ts, EntityType::DAEMON)
    {
        
    }
    
    ~daemonobj()
    {
        
    }
    
    const std::string& GetName()
    {
        return name;
    }
    
    void SetName(const std::string& name)
    {
        this->name = name;
    }
    
private:
    std::string name;
};

#endif