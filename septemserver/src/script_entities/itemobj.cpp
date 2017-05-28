#include "script_entities/itemobj.h"
#include "entity_manager.h"
#include "string_utils.h"

itemobj::itemobj(sol::this_state ts, sol::this_environment te, std::string name, ItemType itype) :
    script_entity(ts, te, EntityType::ITEM, name)
{
    SetName(name);
    entity_manager::Instance().register_item(this);
}


