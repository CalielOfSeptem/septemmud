#include "script_entities/livingentity.h"

roomobj* living_entity::GetRoom()
{
    assert(GetEnvironment() != NULL);
    return dynamic_cast<roomobj*>(GetEnvironment());
}
