#ifndef DAEMONOBJ_H_
#define DAEMONOBJ_H_

#include <string>
#include <vector>
#include "script_entities/script_entity.h"
//#include "entity_manager.h"

struct daemonobj : script_entity
{
    daemonobj(sol::this_state ts);
    daemonobj(sol::this_state ts, std::string s);
    
    ~daemonobj();
    
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