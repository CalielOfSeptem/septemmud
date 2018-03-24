#include "stdafx.h"
#include "entity_wrapper.h"

std::string entity_wrapper::get_object_uid()
{
    // example: /home/ken/git-repos/septem/game_data/realms/void:obj=room1:id=0
    return script_ent->GetInstancePath(); // script_path + ":id=" + std::to_string(instance_id);
}

void entity_wrapper::clear()
{
    script_ent->clear_props();
    script_ent = NULL;
}
