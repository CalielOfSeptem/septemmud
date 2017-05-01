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
    

    void SetVerb(const std::string& name)
    {
        this->cmd_verb = name;
        
    }
    const std::string& GetVerb()
    {
        return cmd_verb;
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
    std::string cmd_verb;
    std::vector<std::string> cmd_syn;
    int priority;
    

};

#endif