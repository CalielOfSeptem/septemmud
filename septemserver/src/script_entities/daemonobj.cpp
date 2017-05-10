#include "script_entities/daemonobj.h"
#include "entity_manager.h"

daemonobj::daemonobj(sol::this_state ts, sol::this_environment te, std::string name)
    : script_entity(ts, te, EntityType::DAEMON, name)
{
    name = "unknown";
    entity_manager::Instance().register_daemon(this);
}

daemonobj::daemonobj(sol::this_state ts, sol::this_environment te )
    : script_entity(ts, te, EntityType::DAEMON, "")
{
    entity_manager::Instance().register_daemon(this);
}

daemonobj::~daemonobj()
{
    entity_manager::Instance().deregister_daemon(this);
}
    