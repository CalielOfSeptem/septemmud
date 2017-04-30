#include "script_entities/commandobj.h"
#include "entity_manager.h"

commandobj::commandobj(sol::this_state ts, std::string name)
     :  script_entity(ts, EntityType::COMMAND), cmd_verb(name), priority(10)
{
    entity_manager::Instance().register_command(this);
    // this->priority = 10;
    // this->cmd_name = name;
}

commandobj::commandobj(sol::this_state ts, std::string name, int priority)
    :  script_entity(ts, EntityType::COMMAND), cmd_verb(name), priority(priority)
{
    entity_manager::Instance().register_command(this);
    //this->priority = priority;
   // this->cmd_name = name;
}

commandobj::~commandobj()
{
    entity_manager::Instance().deregister_command(this);
}
