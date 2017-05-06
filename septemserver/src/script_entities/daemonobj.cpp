#include "script_entities/daemonobj.h"
#include "entity_manager.h"

daemonobj::daemonobj(sol::this_state ts, std::string name)
    : script_entity(ts, EntityType::DAEMON, name)
{
    name = "unknown";
    entity_manager::Instance().register_daemon(this);
}

daemonobj::daemonobj(sol::this_state ts)
    : script_entity(ts, EntityType::DAEMON, "")
{
    entity_manager::Instance().register_daemon(this);
}

daemonobj::~daemonobj()
{
    entity_manager::Instance().deregister_daemon(this);
}
    