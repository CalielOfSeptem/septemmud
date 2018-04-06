#include "stdafx.h"
#include "luatypes.h"
#include "script_entities/roomobj.h"
#include "script_entities/livingentity.h"
#include "script_entities/playerobj.h"
#include "script_entities/daemonobj.h"
#include "script_entities/commandobj.h"
#include "script_entities/extcommandobj.h"
#include "script_entities/itemobj.h"
#include "script_entities/doorobj.h"
#include "script_entities/actionobj.h"
#include "script_entities/lookobj.h"
#include "script_entities/exitobj.h"
#include "script_entities/npcobj.h"

#include "fs/fs_manager.h"

#define SOL_CHECK_ARGUMENTS 1
#include <sol.hpp>


int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
	// L is the lua state, which you can wrap in a state_view if necessary
	// maybe_exception will contain exception, if it exists
	// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
	std::cout << "An exception occurred in a function, here's what it says ";
	if (maybe_exception) {
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	}
	else {
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(), description.size());
		std::cout << std::endl;
	}

	// you must push 1 element onto the stack to be 
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
	// so we push a single string (in our case, the description of the error)
	return sol::stack::push(L, description);
}


struct b
{
	b(sol::protected_function f)
	{
		func = f;
	}
	void do_action()
	{
        sol::protected_function_result result = func(this);
        if(!result.valid()) {
            sol::error err = result;
        }
	}
	sol::protected_function func;
};
struct a
{
	b * add_action( sol::protected_function func )
	{
		actions.push_back(std::shared_ptr<b>(std::make_shared<b>(func)));
		return actions.back().get();
	}
	
	void remove_action( b * a )
	{
		actions.erase(std::remove_if(actions.begin(), actions.end(),
					  [a](const std::shared_ptr<b> & i){ return &(*i) == a; }),
		actions.end());
	}
	
	void do_actions()
	{
		std::vector< std::shared_ptr<b> > a2;

		for( auto& r: actions )
		{
			a2.push_back(r);
		}
		for( auto& r: a2 )
		{
			r->do_action();
		}
	}
	
	sol::object get_property_lua(const char* name, sol::this_state s)
	{
		return props[name];
	}

	void set_property_lua(const char* name, sol::stack_object object)
	{
		props[name] = object.as<sol::object>();
	}
	
	std::vector< std::shared_ptr<b> > actions;
	std::unordered_map<std::string, sol::object> props;
};
void lua_test()
{
	sol::state lua;
	lua.open_libraries(sol::lib::base,
				   sol::lib::os,
				   sol::lib::string,
				   sol::lib::math,
				   sol::lib::table,
				   sol::lib::package,
				   sol::lib::debug);
				   
   lua.new_usertype<a>("a",
		  sol::meta_function::new_index,
		  &a::set_property_lua,
		  sol::meta_function::index,
		  &a::get_property_lua,
		  "AddAction",
		  &a::add_action,
		  "DoActions",
		  &a::do_actions,
		  "RemoveAction",
		  &a::remove_action);
		  
		lua.new_usertype<b>("b");
		lua.safe_script(R"(
		d1 = a.new()
		
		function sleep(n)
		  os.execute("sleep " .. tonumber(n))
		end

		function d1:foo()
			print('onfoo')
			d1:AddAction(d1.foo)
		end
		d1:AddAction(d1.foo)
		
		while true do
			d1:DoActions()
			sleep(2)
		end

		)");
		
}



void init_lua_state(sol::state& l)
{
	//lua_test();
    sol::state& lua = l;
	
    //m_state_internal.reset(new _internal_lua_);
    lua.open_libraries(sol::lib::base,
                       sol::lib::os,
                       sol::lib::string,
                       sol::lib::math,
                       sol::lib::table,
                       sol::lib::package,
                       sol::lib::debug);
					   
	lua.set_exception_handler(&my_exception_handler);

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
            "RemoveAction", &script_entity::RemoveAction,
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
                              sol::constructors<roomobj(sol::this_state, sol::this_environment, int)>(),
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
                              "GetNPCs",
                              &roomobj::GetNPCs,
                              "GetLooks",
                              &roomobj::GetLooks,
                              "SendToRoom",
                              &roomobj::SendToRoom,
                              "IsOutdoors",
                              &roomobj::GetIsOutdoor,
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
                                sol::base_classes,
                                sol::bases<script_entity>());

    lua.new_usertype<npcobj>("npc",
                                sol::constructors<npcobj(sol::this_state, sol::this_environment, std::string)>(),
                                sol::meta_function::new_index,
                                &npcobj::set_property_lua,
                                sol::meta_function::index,
                                &npcobj::get_property_lua,
                                "SendToEntity",
                                &npcobj::SendToEntity,
                                "SendToPlayer",
                                &npcobj::SendToEntity,
                                "GetEnvironment",
                                &npcobj::GetEnvironment,
                                "SetEnvironment",
                                &npcobj::SetEnvironment,
                                "SendToRoom",
                                &npcobj::SendToEnvironment,
                                "GetRoom",
                                &npcobj::GetRoom,
                                sol::base_classes,
                                sol::bases<living_entity, script_entity>());
                                
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
                                "IsArch",
                                &playerobj::isArch,
                                "IsCreator",
                                &playerobj::isCreator,
                                "GetAccountType",
                                &playerobj::get_accountType,
                                "cwd", sol::readonly(&playerobj::cwd),
                                //"workspacePath", sol::readonly(&playerobj::workspacePath),
                                // "Debug", &script_entity::debug,
                                "GetWorkspacePath",
                                &playerobj::get_workspacePath,
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
