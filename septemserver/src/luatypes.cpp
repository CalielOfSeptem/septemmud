#include "luatypes.h"
#include "script_entities/roomobj.h"
#include "script_entities/livingentity.h"
#include "script_entities/playerobj.h"
#include "script_entities/daemonobj.h"
#include "script_entities/commandobj.h"
#include "script_entities/extcommandobj.h"
#include "script_entities/itemobj.h"
#include "script_entities/doorobj.h"
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
            "GetPhysicalScriptPath", &script_entity::GetPhysicalScriptPath,
            "GetVirtualScriptPath", &script_entity::GetVirtualScriptPath,
            "Reset",  &script_entity::clear_props,
            "GetType", &script_entity::GetEntityTypeString,
            "GetEnvironment", &script_entity::GetEnvironment,
            "GetUID", &script_entity::get_uid, 
            "SetUID", &script_entity::set_uid,
            "Debug", &script_entity::debug,
            "AddAction", &script_entity::AddAction,
            "AddCommand", &script_entity::AddCommand,
            "GetCommands", &script_entity::GetCommands,
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
                            

    lua.new_usertype<inventory_slot>("inventory_slot",
                                "GetInventorySlot",
                                &inventory_slot::get_inventorySlot,
                                "GetMaxItems",
                                &inventory_slot::get_maxItems,
                                "GetMaxItemSize",
                                &inventory_slot::get_maxItemSize,
                                "GetItemCount",
                                &inventory_slot::get_itemCount
                                );
                           
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
                            
                            "GetIsWearable", &itemobj::get_isWearable,
                            //"SetWearable", &itemobj::set_isWearable,

                            "GetIsStackable", &itemobj::get_isStackable,
                            //"SetStackable", &itemobj::set_isStackable,
                            
                            "GetCurrentStackCount", &itemobj::get_currentStackCount,
                            "SetCurrentStackCount", &itemobj::set_currentStackCount,
                            
                            "IncrementStackCount", &itemobj::incrementStackCount,
                            "DecrementStackCount", &itemobj::decrementStackCount,
                            
                            "GetDefaultStackSize", &itemobj::get_defaultStackSize,
                            "SetDefaultStackSize", &itemobj::set_defaultStackSize,
                            
                            "GetIsContainer", &itemobj::get_isContainer,
                            "SetIsContainer", &itemobj::set_isContainer,
                            
                            "GetIsLockable", &itemobj::get_isLockable,
                            "SetIsLockable", &itemobj::set_isLockable,
                            
                            "GetIsLocked", &itemobj::get_isLocked,
                            "SetIsLocked", &itemobj::set_isLocked,
                            
                            "GetIsOpen", &itemobj::get_isOpen,
                            "SetIsOpen", &itemobj::set_isOpen,
                            
                            "GetInventorySlot", &itemobj::get_inventorySlot,
                            "SetInventorySlot", &itemobj::set_inventorySlot,
                            
                            "GetPluralName", &itemobj::get_pluralName,
                            "SetPluralName", &itemobj::set_pluralName,
                            "GetItemPluralNoun", &itemobj::get_itemPluralNoun,
                            
                            "GetItemNoun", &itemobj::get_itemNoun,
                            "GetItemArticle", &itemobj::get_itemArticle,
                            "GetItemAdjectives", &itemobj::get_itemAdjectives,
                            
                            "SaveProperty", &itemobj::SaveProperty,
                            "GetProperty", &itemobj::GetProperty,
                            
                            "weight", sol::property(&itemobj::get_weight, &itemobj::set_weight),
                            
                            // Following two were removed due to a bug/limitation in the most-recent version of SOL2
                          //  "isWearable", sol::readonly(&itemobj::get_isWearable),
                          //  "isStackable", sol::readonly(&itemobj::get_isStackable),
                            "isContainer", sol::property(&itemobj::get_isContainer, &itemobj::set_isContainer),
                            
                            "isCloseable", sol::property(&itemobj::get_isCloseable, &itemobj::set_isCloseable),
                            "isLockable", sol::property(&itemobj::get_isLockable, &itemobj::set_isLockable),
                            "isLocked", sol::property(&itemobj::get_isLocked, &itemobj::set_isLocked),
                            "isOpen", sol::property(&itemobj::get_isOpen, &itemobj::set_isOpen),
                            
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
                              "GetDoors",
                              &roomobj::GetDoors,
                              "AddDoor",
                              &roomobj::AddDoor,
                              "AddLook",
                              &roomobj::AddLook,
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
                              "GetLooks",
                              &roomobj::GetLooks,
                              "SendToRoom",
                              &roomobj::SendToRoom,
                              sol::base_classes,
                              sol::bases<script_entity, container_base>());
                              
                    lua.new_usertype<doorobj>("doorobj",
                            "GetDoorPath",
                            &doorobj::GetDoorPath,
                            "SetDoorPath",
                            &doorobj::SetDoorPath,
                            "GetRoom",
                            &doorobj::GetRoom,
                            "GetOtherRoom",
                            &doorobj::get_otherRoom,
                            "GetOtherDoor",
                            &doorobj::get_otherDoor,
                            sol::base_classes,
                            sol::bases<itemobj, script_entity>()
                              );
                              
                    lua.new_usertype<lookobj>("lookobj",
                            "GetLook",
                            &lookobj::GetLook,
                            "GetDescription",
                            &lookobj::GetDescription
                              );


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
                                "SetPlayerName",
                                &playerobj::set_playerName,
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
                                "GetAccountType",
                                &playerobj::get_accountType,
                                "cwd", sol::readonly(&playerobj::cwd),
                                //"workspacePath", sol::readonly(&playerobj::workspacePath),
                                // "Debug", &script_entity::debug,
                                "GetWorkspacePath",
                                &playerobj::get_workspacePath,
                                "GetName",
                                &script_entity::GetName,
                                "DoCommand",
                                &living_entity::DoCommand,
                                "DoSave",
                                &playerobj::do_save,
                                "VerifyPassword",
                                &playerobj::verify_password,
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
                                    "GetGender",
                                    &living_entity::get_gender,
                                    "SetGender",
                                    &living_entity::set_gender,
                                    "GetBodyPosition",
                                    &living_entity::get_body_position,
                                    "SetBodyPosition",
                                    &living_entity::set_body_position,
                                    
                                    "AddInventorySlot",
                                    &living_entity::AddInventorySlot,
                                    
                                    "GetInventorySlots",
                                    &living_entity::GetInventorySlots,
                                    
                                    "GetItemsAtInventorySlot",
                                    &living_entity::GetItemsInInventorySlot,
                                    
                                    "SafeAddItem",
                                    &living_entity::SafeAddItemToInventory,
                                    
                                    sol::base_classes,
                                    sol::bases<script_entity>());

    lua.new_usertype<commandobj>(
        "command",
        sol::constructors<commandobj(sol::this_state, sol::this_environment, std::string), 
            commandobj(sol::this_state, sol::this_environment, std::string, int)>(),
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
        
        
        
        
        
        lua.new_usertype<extcommandobj>(
        "extcommand",
        sol::constructors<extcommandobj(sol::state_view lua_state, sol::protected_function func, const std::string& name, sol::object userData)>(),
        "GetCommand",
        &extcommandobj::GetCommand,
        "SetCommand",
        &extcommandobj::SetCommand,
        "GetAliases",
        &extcommandobj::GetAliases,
        "ExecuteCommand",
        &extcommandobj::ExecuteFunc,
        "SetAliases",
        &extcommandobj::SetAliases);
        

}
