#ifndef COMMANDOBJ_H_
#define COMMANDOBJ_H_
#include "sol.hpp"
#include <string>
#include <vector>
#include "script_entities/script_entity.h"

struct commandobj : script_entity
{
    commandobj(sol::this_state ts, std::string name);
    
    commandobj(sol::this_state ts, std::string name, int priority);
    
    ~commandobj();
    

    void SetCommand(const std::string& name)
    {
        this->name = name;
        
    }
    const std::string& GetCommand()
    {
        return name;
    }
    
    void SetAliases(const sol::as_table_t<std::vector<std::string> >& aliases)
    {
        // this->cmd_syn = cmd_syn;
        const auto& vex = aliases.source;
        for(auto& s : vex) {
            this->command_aliases.push_back(s);
        }
    }
    const std::vector<std::string>& GetAliases()
    {
        return command_aliases;
    }
    
    void SetPriority(int priority)
    {
        this->priority = priority;
    }
    
    int GetPriority() const
    {
        return priority;
    }

private:
    std::vector<std::string> command_aliases;
    int priority;
    

};

#endif