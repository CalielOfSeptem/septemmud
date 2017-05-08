#include "script_entities/script_entity.h"
#include "entity_manager.h"
#include "spdlog/spdlog.h"
#include "global_settings.h"
#include "config.h"
#include "string_utils.h"
namespace spd = spdlog;
script_entity::script_entity(sol::this_state ts, EntityType myType, std::string name)
    : m_type(myType)
    , environment_(NULL)
    , name(name)
{
    lua_State* L = ts;
    // references the object that called this function
    // in constructors:

    m_userdata = std::shared_ptr<_sol_userdata_>(new _sol_userdata_);
    m_userdata->selfobj = sol::userdata(L, -1);

    // the -1 (NEGATIVE one) above
    // means "off the top fo the stack"
    // (-1 is the top, -2 is one below, etc...)

    // definitely the same
    //    script_entity& self = selfobj.as<script_entity>();
    // assert(&self == this);
    entity_manager::Instance().register_entity(this, myType);
}

script_entity::~script_entity()
{
    //  LOG_DEBUG << "Destroyed object, script path= " << script_path;
    entity_manager::Instance().deregister_entity(this, m_type);
}

void script_entity::debug(sol::this_state ts, const std::string& msg)
{
    lua_State* L = ts;
    sol::userdata selfobj = sol::userdata(L, 1);
    script_entity& self = selfobj.as<script_entity>();
    entity_manager::Instance().debug_script(&self, msg);

    // if( !m_log )
    //{

    //                   global_settings::Instance().GetSetting(DEFAULT_ROOM_LOG_PATH);
    // m_log.reset(new entity_log( entityLog, m_type ));
   // auto rotating_logger = spd::rotating_logger_mt(s, entityLog, 1048576 * 5, 3);
   
    auto log = spd::get(strip_path_copy(script_path));//>info("loggers can be retrieved from a global registry using the spdlog::get(logger_name) function");
  
   
    if( !log )
    {
        spd::set_pattern("[%x %H:%M:%S:%e] %v");
        std::string entityLog = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH) +
                            global_settings::Instance().GetSetting(DEFAULT_LOGS_PATH);

        std::string s = strip_path_copy(script_path);
        //strip_path(s);
        entityLog += s;
        try
        {
            auto rotating_logger = spd::rotating_logger_mt(s, entityLog, 1048576 * 5, 3);
            //rotating_logger->flush_on(spd::level::debug);
            rotating_logger->info(msg);
            rotating_logger->flush();
            
        }
        catch( std::exception& )
        {
            
        }
       
   }
   else
   {
       log->info(msg);
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

void script_entity::SetScriptPath(std::string& path)
{
    script_path = path;

    
}
