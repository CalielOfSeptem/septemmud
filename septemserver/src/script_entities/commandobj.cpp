#include "stdafx.h"
#include "script_entities/commandobj.h"
#include "entity_manager.h"
#include "security_context.hpp"

commandobj::commandobj(sol::this_state ts, sol::this_environment te, std::string name)
    : script_entity(ts, te, EntityType::COMMAND, name)
    , priority(10)
{
    entity_manager::Instance().register_command(this);
    // this->priority = 10;
    // this->cmd_name = name;
}

commandobj::commandobj(sol::this_state ts, sol::this_environment te, std::string name, int priority)
    : script_entity(ts, te, EntityType::COMMAND, name)
    , priority(priority)
{
    entity_manager::Instance().register_command(this);
    // this->priority = priority;
    // this->cmd_name = name;
}

commandobj::~commandobj()
{
    entity_manager::Instance().deregister_command(this);
}

void commandobj::SetCommand(const std::string& name)
{
    this->name = name;
}

const std::string& commandobj::GetCommand()
{
    return name;
}

void commandobj::SetAliases(const sol::as_table_t<std::vector<std::string> >& aliases)
{
    const auto& vex = aliases.source;
    for(auto& s : vex) {
        this->command_aliases.push_back(s);
    }
}

void commandobj::AddAlias(const std::string& alias)
{
    this->command_aliases.push_back(alias);
}

const std::vector<std::string>& commandobj::GetAliases()
{
    return command_aliases;
}

void commandobj::SetPriority(int priority)
{
    this->priority = priority;
}

int commandobj::GetPriority() const
{
    return priority;
}
