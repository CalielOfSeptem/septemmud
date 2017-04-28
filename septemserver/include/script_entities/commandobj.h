#ifndef COMMANDOBJ_H_
#define COMMANDOBJ_H_
#include "sol.hpp"
#include <string>
#include <vector>
#include "script_entities/script_entity.h"


struct commandobj : script_entity
{
    commandobj(sol::this_state ts)
        : script_entity(ts, EntityType::COMMAND)
    {
        this->priority = 10;
    }
    
    commandobj(sol::this_state ts, std::string& name)
        : script_entity(ts, EntityType::COMMAND)
    {
        this->priority = 10;
    }
    
    commandobj(sol::this_state ts, std::string& name, int& priority)
        : script_entity(ts, EntityType::COMMAND)
    {
        this->priority = priority;
        this->cmd_name = name;
    }
    

    void SetName(const std::string& name)
    {
        this->cmd_name = name;
    }
    const std::string& GetName()
    {
        return cmd_name;
    }
    void SetSynonyms(const sol::as_table_t<std::vector<std::string> >& cmd_syn)
    {
        // this->cmd_syn = cmd_syn;
        const auto& vex = cmd_syn.source;
        for(auto& s : vex) {
            this->cmd_syn.push_back(s);
        }
    }
    const std::vector<std::string>& GetSynonyms()
    {
        return cmd_syn;
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
    std::string cmd_name;
    std::vector<std::string> cmd_syn;
    int priority;

};

#endif