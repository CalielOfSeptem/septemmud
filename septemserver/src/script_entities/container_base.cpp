#include "script_entities/container_base.h"
#include "script_entities/itemobj.h"
#include "entity_manager.h"

void container_base::AddEntityToInventory(script_entity* se)
{
    if(se == NULL)
        return;

    script_entity* env = se->GetEnvironment();

    if(env != NULL && env == GetOwner())
        return;
        /*
    if(itemobj* item = dynamic_cast<itemobj*>(se)) {
        if(item->get_isStackable()) {
           if( item->get_currentStackCount() > 1 )
           {
               // clone item, decrement count..
                std::string tpath = item->GetBaseScriptPath();
                entity_manager::Instance().clone_item( tpath, GetOwner() );
                itemobj * clone_item = entity_manager::Instance().GetItemByScriptPath(tpath);
                script_entity * se2 = static_cast<script_entity*>(clone_item);
                inventory.insert( se2 );
                item->decrementStackCount();
                auto t = GetOwner();
                if(t == NULL)
                    return;
                se2->SetEnvironment(GetOwner());
                return;
           }
        }
    }
     */

    if(env != NULL) {
        // remove entity from inventory
        if(container_base* cb = dynamic_cast<container_base*>(env)) {
            cb->RemoveEntityFromInventory(se);
        }
    }

    if(itemobj* item = dynamic_cast<itemobj*>(se)) {
        if(item->get_isStackable()) {
            for(script_entity* se_t : inventory) {
                if(itemobj* itemb = dynamic_cast<itemobj*>(se_t)) {
                    ///std::cout << itemb->GetScriptPath() << std::endl;
                   // std::cout << item->GetScriptPath() << std::endl;
                    if(itemb->GetBaseScriptPath() == item->GetBaseScriptPath() && itemb->get_currentStackCount() < itemb->get_defaultStackSize()) {
                       //[ std::cout << itemb->get_currentStackCount() << std::endl;
                        itemb->incrementStackCount();
                        item->set_destroy(true);
                        return; // no reason to add it, we just added to a stack.
                    }
                }
            }
        }
    }

    inventory.insert(se);
    auto t = GetOwner();
    if(t == NULL)
        return;
    se->SetEnvironment(GetOwner());
}
