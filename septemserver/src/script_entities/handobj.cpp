#include "stdafx.h"
#include "script_entities/handobj.h"
#include "script_entities/itemobj.h"

bool handobj::AddItem(itemobj* i)
{
    bool b = container_base::AddEntityToInventory(dynamic_cast<script_entity*>(i));
    // GetOwner()->do_save();
    do_save();
    return b;
}

bool handobj::RemoveItem(itemobj* i)
{
    return RemoveEntityFromInventory(dynamic_cast<script_entity*>(i));
    // return container_base::RemoveEntityFromInventory(se);
}

bool handobj::IsEmpty()
{
    return (this->GetInventory().size() == 0);
}

itemobj* handobj::GetItem()
{
    if(IsEmpty())
        return NULL;
    return dynamic_cast<itemobj*>(*std::next(this->GetInventory().begin(), 0));
}

script_entity* handobj::GetOwner()
{
    return this;
}

bool handobj::do_save()
{
    if(m_livingEntity != NULL)
        return m_livingEntity->do_save();
    return false;
}

bool handobj::AddEntityToInventory(script_entity* se)
{
    bool b = container_base::AddEntityToInventory(se);
    do_save();
    return b;
}

handobj::handobj()
{
    m_type = EntityType::HAND;
    // env = NULL;
}

bool handobj::RemoveEntityFromInventoryByID(const std::string& id)
{
    return container_base::RemoveEntityFromInventoryByID(id);
}

bool handobj::RemoveEntityFromInventory(script_entity* se)
{
    return container_base::RemoveEntityFromInventory(se);
}
