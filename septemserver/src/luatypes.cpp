#include "luatypes.h"
#include "script_entities/roomobj.h"
#include "script_entities/livingentity.h"
#include "script_entities/playerobj.h"
#include "script_entities/daemonobj.h"
#include "script_entities/commandobj.h"
#include "script_entities/itemobj.h"
#include "fs/fs_manager.h"
void init_lua_state(sol::state& l)
{
    sol::state& lua = l;
    //m_state_internal.reset(new _internal_lua_);
    lua.open_libraries(sol::lib::base,
                       sol::lib::string,
                       sol::lib::math,
                       sol::lib::table,
                       sol::lib::package,
                       sol::lib::debug);

    lua.new_usertype<script_entity>("script_entity", 
            "GetName",  &script_entity::GetName,
            "SetName", &script_entity::SetName,
            "GetBaseScriptPath", &script_entity::GetBaseScriptPath,
            "Reset",  &script_entity::clear_props,
            "GetType", &script_entity::GetEntityTypeString,
            "GetUID", &script_entity::get_uid, 
            "SetUID", &script_entity::set_uid,
            "destroy", sol::property(&script_entity::get_destroy, &script_entity::set_destroy)
            );

    lua.new_usertype<exitobj>("exitobj",
                              "GetExitPath",
                              &exitobj::GetExitPath,
                              "SetExit",
                              &exitobj::SetExit,
                              "SetExitDesc",
                              &exitobj::SetExitDesc,
                              "SetExitPath",
                              &exitobj::SetExitPath,
                              "SetObvious",
                              &exitobj::SetObvious,
                              "GetExit",
                              &exitobj::GetExit);

    lua.new_usertype<handobj>("hand",
                            "GetInventory",
                            &container_base::GetInventory,
                            "IsEmpty",
                            &handobj::IsEmpty,
                            "AddToInventory",
                            &handobj::AddEntityToInventory,
                            "AddItem",
                            &handobj::AddItem,
                            "RemoveItem",
                            &handobj::RemoveItem,
                            "GetItem",
                            &handobj::GetItem,
                            sol::base_classes,
                            sol::bases<container_base>() );
                            
/*
    lua.new_usertype<inventory_slot>("inventory_slot",
                                sol::base_classes,
                                sol::bases<container_base>());
*/                        
                            
    lua.new_usertype<itemobj>("item",
                            sol::constructors<itemobj(sol::this_state, sol::this_environment, std::string, ItemType)>(),
                            sol::meta_function::new_index,
                            &itemobj::set_property_lua,
                            sol::meta_function::index,
                            &itemobj::get_property_lua,
                            "GetInventory",
                            &container_base::GetInventory,
                            "GetItems",
                            &itemobj::GetItems,
                            "AddToInventory",
                            &container_base::AddEntityToInventory,
                            "AddItem",
                            &container_base::AddEntityToInventory,
                            
                            "GetSize", &itemobj::get_size,
                            "SetSize", &itemobj::set_size,
                            
                            "GetWeight", &itemobj::get_weight,
                            "SetWeight", &itemobj::set_weight,
                            
                            "GetWearable", &itemobj::get_isWearable,
                            "SetWearable", &itemobj::set_isWearable,

                            "GetStackable", &itemobj::get_isStackable,
                            "SetStackable", &itemobj::set_isStackable,
                            
                            "GetCurrentStackCount", &itemobj::get_currentStackCount,
                            "SetCurrentStackCount", &itemobj::set_currentStackCount,
                            
                            "IncrementStackCount", &itemobj::incrementStackCount,
                            "DecrementStackCount", &itemobj::decrementStackCount,
                            
                            "GetDefaultStackSize", &itemobj::get_defaultStackSize,
                            "SetDefaultStackSize", &itemobj::set_defaultStackSize,
                            
                            "GetIsContainer", &itemobj::get_isContainer,
                            "SetIsContainer", &itemobj::set_isContainer,
                            
                            "GetInventorySlot", &itemobj::get_inventorySlot,
                            "SetInventorySlot", &itemobj::set_inventorySlot,
                            
                            "GetPluralName", &itemobj::get_pluralName,
                            "SetPluralName", &itemobj::set_pluralName,
                            "GetItemPluralNoun", &itemobj::get_itemPluralNoun,
                            
                            "GetItemNoun", &itemobj::get_itemNoun,
                            "GetItemArticle", &itemobj::get_itemArticle,
                            "GetItemAdjectives", &itemobj::get_itemAdjectives,
                            
                            "weight", sol::property(&itemobj::get_weight, &itemobj::set_weight),
                            "isWearable", sol::property(&itemobj::get_isWearable, &itemobj::set_isWearable),
                            "isStackable", sol::property(&itemobj::get_isStackable, &itemobj::set_isStackable),
                            "isContainer", sol::property(&itemobj::get_isContainer, &itemobj::set_isContainer),
                            "size", sol::property(&itemobj::get_size, &itemobj::set_size),
                            
                            
                            sol::base_classes,
                            sol::bases<script_entity, container_base>() );
                            
                            
     //                            double m_weight; // item weight in stones
    // ItemSize m_size = ItemSize::TINY;
    // bool bisWearable;
    // bool bisStackable;
    // bool bisContainer;
    
    lua.new_usertype<roomobj>("room",
                              sol::constructors<roomobj(sol::this_state, sol::this_environment)>(),
                              sol::meta_function::new_index,
                              &roomobj::set_property_lua,
                              sol::meta_function::index,
                              &roomobj::get_property_lua,
                              "GetTitle",
                              &roomobj::GetTitle,
                              "SetTitle",
                              &roomobj::SetTitle,
                              "GetDescription",
                              &roomobj::GetDescription,
                              "SetDescription",
                              &roomobj::SetDescription,
                              "GetShortDescription",
                              &roomobj::GetShortDescription,
                              "SetShortDescription",
                              &roomobj::SetShortDescription,
                              "GetExits",
                              &roomobj::GetExits,
                              "AddExit",
                              &roomobj::AddExit,
                              "GetType",
                              &script_entity::GetEntityTypeString,
                              "Debug",
                              &script_entity::debug,
                              "GetInventory",
                              &container_base::GetInventory,
                              "AddToInventory",
                              &roomobj::AddEntityToInventory,
                              "RemoveFromInventory",
                              &roomobj::RemoveEntityFromInventory,
                              "GetPlayers",
                              &roomobj::GetPlayers,
                              "GetItems",
                              &roomobj::GetItems,
                              sol::base_classes,
                              sol::bases<script_entity, container_base>());


    lua.new_usertype<daemonobj>("daemon",
                                sol::constructors<daemonobj(sol::this_state ts, sol::this_environment, std::string)>(),
                                sol::meta_function::new_index,
                                &daemonobj::set_property_lua,
                                sol::meta_function::index,
                                &daemonobj::get_property_lua,
                                "GetName",
                                &daemonobj::GetName,
                                "SetName",
                                &daemonobj::SetName,
                                "GetType",
                                &script_entity::GetEntityTypeString,
                                //  "Debug", &script_entity::debug,
                                sol::base_classes,
                                sol::bases<script_entity>());

    lua.new_usertype<playerobj>("player",
                                sol::constructors<playerobj(sol::this_state, sol::this_environment, std::string)>(),
                                sol::meta_function::new_index,
                                &playerobj::set_property_lua,
                                sol::meta_function::index,
                                &playerobj::get_property_lua,
                                "GetPlayerName",
                                &playerobj::GetPlayerName,
                                "SendToEntity",
                                &playerobj::SendToEntity,
                                "SendToPlayer",
                                &playerobj::SendToEntity,
                                "GetEnvironment",
                                &playerobj::GetEnvironment,
                                "SetEnvironment",
                                &playerobj::SetEnvironment,
                                "SendToRoom",
                                &playerobj::SendToEnvironment,
                                "GetRoom",
                                &playerobj::GetRoom,
                                "GetType",
                                &script_entity::GetEntityTypeString,
                                "cwd", sol::readonly(&playerobj::cwd),
                                "workspacePath", sol::readonly(&playerobj::workspacePath),
                                // "Debug", &script_entity::debug,
                                "GetName",
                                &script_entity::GetName,
                                "DoCommand",
                                &living_entity::DoCommand,
                                "DoSave",
                                &playerobj::do_save,
                                sol::base_classes,
                                sol::bases<living_entity, script_entity>());
                                
    lua.new_usertype<file_entity>("file_entity", 
            "Name", sol::readonly(&file_entity::name),
            "isDirectory", sol::readonly(&file_entity::isDirectory));

    lua.new_usertype<living_entity>("living_entity",
                                    "GetType",
                                    &script_entity::GetEntityTypeString,
                                    // "Debug", &script_entity::debug,
                                    "DoCommand",
                                    &living_entity::DoCommand,
                                    "SendToEntity",
                                    &living_entity::SendToEntity,
                                    "SendToPlayer",
                                    &living_entity::SendToEntity,
                                    "SendToRoom",
                                    &living_entity::SendToEnvironment,
                                    "GetRoom",
                                    &living_entity::GetRoom,
                                    "GetName",
                                    &script_entity::GetName,
                                    "RightHand",
                                    &living_entity::GetRightHand,
                                    "LeftHand",
                                    &living_entity::GetLeftHand,
                                    "DoSave",
                                    &living_entity::do_save,
                                    "AddToInventory",
                                    &container_base::AddEntityToInventory,
                                    "AddItem",
                                    &container_base::AddEntityToInventory,
                                    "GetItems",
                                    &living_entity::GetItems,
                                    /*
                                    "AddInventorySlot",
                                    &living_entity::AddInventorySlot,
                                    "GetInventorySlots",
                                    &living_entity::GetInventorySlots,
                                    */
                                    sol::base_classes,
                                    sol::bases<script_entity>());

    lua.new_usertype<commandobj>(
        "command",
        sol::constructors<commandobj(sol::this_state, sol::this_environment, std::string), commandobj(sol::this_state, sol::this_environment, std::string, int)>(),
        sol::meta_function::new_index,
        &playerobj::set_property_lua,
        sol::meta_function::index,
        &playerobj::get_property_lua,
        "GetCommand",
        &commandobj::GetCommand,
        "SetCommand",
        &commandobj::SetCommand,
        "GetAliases",
        &commandobj::GetAliases,
        "SetAliases",
        &commandobj::SetAliases,
        "SetPriority",
        &commandobj::SetPriority,
        "GetPriority",
        &commandobj::GetPriority,
        "GetType",
        &script_entity::GetEntityTypeString,
        // "Debug", &script_entity::debug,
        sol::base_classes,
        sol::bases<script_entity>());
}
