#include "stdafx.h"
#include "script_entities/script_entity.h"


#include "entity_manager.h"
#include "script_entities/actionobj.h"
#include "script_entities/extcommandobj.h"

script_entity::script_entity(EntityType myType, std::string name)
    : environment_(NULL)
    , m_type(myType)
    , name(name)
    

{
}

script_entity::script_entity()
    : environment_(NULL)
    , m_type(EntityType::UNKNOWN)
    , name("")
    

{
}

script_entity::script_entity(sol::this_state ts, sol::this_environment te, EntityType myType, std::string name)
    : environment_(NULL)
    , m_type(myType)
    , name(name)
    
{
    lua_State* L = ts;

    if(!te) {
        std::cout << "function does not have an environment: exiting function early" << std::endl;
        return;
    }
    sol::environment& env = te;
    sol::optional<std::string> sp = env["_INTERNAL_SCRIPT_PATH_"];

    assert(sp);
    this->SetVirtualScriptPath(sp.value());

    sol::optional<std::string> spv = env["_INTERNAL_PHYSICAL_SCRIPT_PATH_"];

    assert(spv);
    this->SetPhysicalScriptPath(spv.value());

    // sol::optional<std::string> sp2 = env["_INTERNAL_SCRIPT_PATH_BASE_"];
    // assert( sp2 );
    // if( sp2 )
    //     this->SetBaseScriptPath( sp2.value() );
    // references the object that called this function
    // in constructors:

    m_userdata = std::shared_ptr<_sol_userdata_>(new _sol_userdata_);
    m_userdata->selfobj = sol::userdata(L, -2);

    // the -1 (NEGATIVE one) above
    // means "off the top fo the stack"
    // (-1 is the top, -2 is one below, etc...)

    // definitely the same
    //    script_entity& self = selfobj.as<script_entity>();
    // assert(&self == this);
    entity_manager::Instance().register_entity(this, sp.value(), myType);
	//invoke_on_load();
}

script_entity::~script_entity()
{
	on_destroy();
	actions.clear();
    if(virtual_script_path.size() > 0) {
        auto log = spd::get("main");
        std::stringstream ss;
        ss << "Destroyed object, script path= " << virtual_script_path;
        log->info(ss.str());
		entity_manager::Instance().deregister_entity(this);
    }
	
}
/*
void script_entity::debug(const std::string& msg)
{
    //lua_State* L = ts;
    //sol::userdata selfobj = sol::userdata(L, 1);
    //script_entity& self = selfobj.as<script_entity>();
   // entity_manager::Instance().debug_script(&self, msg);

    // if( !m_log )
    //{

    //                   global_settings::Instance().GetSetting(DEFAULT_ROOM_LOG_PATH);
    // m_log.reset(new entity_log( entityLog, m_type ));
   // auto rotating_logger = spd::rotating_logger_mt(s, entityLog, 1048576 * 5, 3);

    auto log = spd::get(strip_path_copy(script_path));//>info("loggers can be retrieved from a global registry using the
spdlog::get(logger_name) function");


    if( !log )
    {

        std::string entityLog = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH) +
                            global_settings::Instance().GetSetting(DEFAULT_LOGS_PATH);

        std::string s = strip_path_copy(script_path);
        //strip_path(s);
        entityLog += s;
        try
        {
            std::vector<spdlog::sink_ptr> sinks;
            //auto rotating_logger = spd::rotating_logger_mt(s, entityLog, 1048576 * 5, 3);
            auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt> (entityLog, "log", 1024*1024, 5,
true);
            auto stdout_sink = spdlog::sinks::stdout_sink_mt::instance();

            //auto color_sink = std::make_shared<spdlog::sinks::ansicolor_sink>(stdout_sink);

           // auto stdout_sink = spdlog::sinks::stdout_sink_mt::instance();
           // auto color_sink = std::make_shared<spdlog::sinks::ansicolor_sink>(stdout_sink);
            sinks.push_back(stdout_sink);
            sinks.push_back(rotating);

            auto combined_logger = std::make_shared<spdlog::logger>(s, begin(sinks), end(sinks));
            combined_logger->set_pattern("[%x %H:%M:%S:%e] %v");
            combined_logger->set_level( spdlog::level::debug );
            spdlog::register_logger(combined_logger);

            combined_logger->debug( "[" + this->GetInstancePath() + "] "+ msg);
            combined_logger->flush();

        }
        catch( std::exception& )
        {

        }

   }
   else
   {
       log->debug( "[" + this->GetInstancePath() + "] "+ msg);
       log->flush();
       // spd::drop( strip_path_copy(script_path) );
   }
    // for (int i = 0; i < 10; ++i)
    //rotating_logger->info(msg);
   // rotating_logger->flush();
    //}

    //  std::stringstream ss;
    //  ss << this->GetInstancePath() << " " << msg;
    // std::string d = ss.str();
    //  entity_manager::Instance().debug(d);
}
*/

void script_entity::debug(const std::string& msg)
{
    auto log = spd::get("main");
    std::stringstream ss;
    ss << msg; //"Destroyed object, script path= " << virtual_script_path;
    log->info(ss.str());
    return;
    /*
    auto log = spd::get(strip_path_copy(virtual_script_path));

    if(!log) {

        std::string entityLog = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH) +
                                global_settings::Instance().GetSetting(DEFAULT_LOGS_PATH);

        std::string s = strip_path_copy(virtual_script_path);

        entityLog += s;
        try {


            spd::set_pattern("[%x %H:%M:%S:%e] %v");


            std::vector<spdlog::sink_ptr> sinks;

            auto rotating =
                std::make_shared<spdlog::sinks::rotating_file_sink_mt>(entityLog, "log", 1024 * 1024, 5, true);
            auto stdout_sink = spdlog::sinks::stdout_sink_mt::instance();

            //auto s_tmp = my_sink();

            sinks.push_back(stdout_sink);
            sinks.push_back(rotating);
            //sinks.push_back(s_tmp);

            auto combined_logger = std::make_shared<spdlog::logger>(s, begin(sinks), end(sinks));
          //  combined_logger->set_pattern("[%x %H:%M:%S:%e] %v");
           // spdlog::set_pattern("[%x %H:%M:%S:%e] %v");
            combined_logger->set_level(spdlog::level::debug);
            spdlog::register_logger(combined_logger);

            combined_logger->debug("[" + this->GetInstancePath() + "] " + msg);
            combined_logger->flush();

        } catch(std::exception&) {
        }

    } else {

        log->debug("[" + this->GetInstancePath() + "] " + msg);
        log->flush();
    }
    */
}

void script_entity::SetVirtualScriptPath(std::string& path)
{
    virtual_script_path = path;
}

void script_entity::SetPhysicalScriptPath(std::string& path)
{
    physical_script_path = path;
}

extcommandobj* script_entity::AddCommand(sol::this_state ts,
                                         sol::protected_function func,
                                         const sol::as_table_t<std::vector<std::string> >& aliases,
                                         sol::object userData)
{
    lua_State* L = ts;
    // lua_stacktrace_ex(L);
    const auto& vex = aliases.source;

    std::vector<std::string> al;
    for(auto ex : vex) {
        al.push_back(ex);
    }

    if(al.empty())
        return NULL;

    std::shared_ptr<extcommandobj> co = std::shared_ptr<extcommandobj>(new extcommandobj(L, func, vex[0], userData));
    al.erase(al.begin());
    for(auto s : al) {
        co->AddAlias(s);
    }

    commands.push_back(co);
    return commands[commands.size() - 1].get();
}

actionobj* script_entity::AddAction(sol::this_state ts, sol::protected_function func, sol::object interval, sol::object userData)
{
	sol::type t = interval.get_type();
    switch(t) {
	case sol::type::number:
	{
		sol::optional<int> i = interval.as<int>();
		actions.push_back(std::shared_ptr<actionobj>(std::make_shared<actionobj>(func, i.value(), userData)));
		return actions.back().get();
		break;
	}
    default:

        break;
    }
	return NULL;
}

void script_entity::DoActions()
{
	std::vector< std::shared_ptr<actionobj> > a2;

	for( auto& r: GetActions() )
	{
		a2.push_back(r);
	}
	for( auto& r: a2 )
	{
		r->DoAction();
	}

}

sol::object script_entity::get_property_lua(const char* name, sol::this_state s)
{
    return props[name];
}

void script_entity::set_property_lua(const char* name, sol::stack_object object)
{
    props[name] = object.as<sol::object>();
}

void script_entity::clear_props()
{
    m_userdata.reset();
    props.clear();
}

script_entity* script_entity::GetEnvironment()
{
	if( environment_ == NULL )
		return NULL;
    return environment_; //.value();
}

void script_entity::SetEnvironment(script_entity* be)
{
    environment_ = be;
}

EntityType& script_entity::GetType()
{
    return m_type;
}

std::string script_entity::GetEntityTypeString()
{
    switch(m_type) {
    case EntityType::UNKNOWN:
        return "unknown";
    case EntityType::ROOM:
        return "room";
    case EntityType::ITEM:
        return "item";
    case EntityType::NPC:
        return "npc";
    case EntityType::PLAYER:
        return "player";
    default:
        return "unknown";
        break;
    }
}

std::string script_entity::GetInstancePath()
{
    return virtual_script_path + ":id=" + std::to_string(instanceID);
}

const std::string& script_entity::GetName()
{
    return name;
}

void script_entity::SetName(const std::string& name)
{
    this->name = name;
}

const std::string& script_entity::GetLook()
{
    return look;
}

void script_entity::SetLook(const std::string& look)
{
    this->look = look;
}

const bool script_entity::get_destroy()
{
    return m_destroy;
}

void script_entity::set_destroy(bool destroy)
{
    m_destroy = destroy;
    if(destroy)
        clear_props();
}

std::string script_entity::get_uid()
{
    return uid;
}

void script_entity::set_uid(std::string id)
{
    uid = id;
}

std::string script_entity::get_entityStorageLocation()
{
    return entity_storage_location;
}

void script_entity::set_entityStorageLocation(std::string& str)
{
    entity_storage_location = str;
}

bool script_entity::do_save()
{
    return true;
}

void script_entity::on_environment_change(EnvironmentChangeEvent evt, script_entity* env)
{
	if( evt == EnvironmentChangeEvent::ADDED )
		this->invoke_on_environment_change();
}

bool script_entity::do_load()
{
    return true;
}

bool script_entity::do_json_load(std::string& j)
{
    return true;
}

std::vector<std::shared_ptr<actionobj>>& script_entity::GetActions()
{
    return actions;
}

std::vector<std::shared_ptr<extcommandobj>>& script_entity::GetCommands()
{
    return commands;
}
void script_entity::RemoveAction(actionobj * a)
{
	if( a == NULL )
		return;
    actions.erase(std::remove_if(actions.begin(), actions.end(),
                          [a](const std::shared_ptr<actionobj> & i){ return &(*i) == a; }),
           actions.end());
	//std::cout << "ACTIONS = " << actions.size() << std::endl;
           
         /*  
    for( std::shared_ptr<actionobj> ai : actions )
    {
        if( &(*ai) == a )
        {
            actions.erase(ai);
            return;
        }
    }
     */
    
}

void script_entity::invoke_on_load()
{
	sol::optional<sol::table> self = m_userdata->selfobj;
	if(self) {
		sol::protected_function exec = self.value()["on_load"];
		// Regsiter anti-infinite loop detector
		entity_manager::Instance().register_hook(this);
		auto result = exec(self);
		if(!result.valid()) {
			// its optional to implement this interface
			//sol::error err = result;
			//auto log = spd::get("main");
			//std::stringstream ss;
			//ss << "Error calling process_command, entity = " << e->GetName() << ", error = " << err.what();
			//log->debug(ss.str());
			//e->debug(ss.str());
			entity_manager::Instance().register_hook(NULL); // clear the hook
		} 
	}
        
}

void script_entity::invoke_on_environment_change()
{
	sol::optional<sol::table> self = m_userdata->selfobj;
	if(self) {
		sol::protected_function exec = self.value()["on_environment_change"];
		// Regsiter anti-infinite loop detector
		entity_manager::Instance().register_hook(this);
		auto result = exec(self);
		if(!result.valid()) {
			// its optional to implement this interface
			
		} 
		entity_manager::Instance().register_hook(NULL); // clear the hook
	}
        
}

void script_entity::invoke_on_destroy()
{
	if( !m_userdata )
		return;
	sol::optional<sol::table> self = m_userdata->selfobj;
	if(self) {
		sol::protected_function exec = self.value()["on_destroy"];
		// Regsiter anti-infinite loop detector
		entity_manager::Instance().register_hook(this);
		auto result = exec(self);
		if(!result.valid()) {
			// its optional to implement this interface
			
		} 
		entity_manager::Instance().register_hook(NULL); // clear the hook
	}
        
}