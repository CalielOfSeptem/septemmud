#include "script_entities/itemobj.h"
#include "entity_manager.h"

itemobj::itemobj(sol::this_state ts, sol::this_environment te, std::string name) :
    script_entity(ts, te, EntityType::ITEM, name)
{
    //std::cout << random_string() << "\n";
   // std::cout << random_string(10, "abcdefghijklmnopqrstuvwxyz");
   entity_manager::Instance().register_item(this);
   
}
     

itemobj::itemobj(sol::this_state ts, sol::this_environment te) :
    script_entity(ts, te, EntityType::ITEM, "")
{
    //std::cout << random_string() << "\n";
   // std::cout << random_string(10, "abcdefghijklmnopqrstuvwxyz");
   entity_manager::Instance().register_item(this);
}
     