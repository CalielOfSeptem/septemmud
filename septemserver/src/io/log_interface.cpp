#include "stdafx.h"
#include "io/log_interface.h"
#include "io/db_interface.h"


void log_interface::log(LOGLEVEL log_level, std::string script_path, std::string msg)
{
	auto main_log = spd::get("main");
	//std::stringstream ss;
	//ss << "Error accessing database in function test <<  " << e.what() ; // Destroyed object, script path= " << virtual_script_path;
	switch( log_level )
	{
		case LOGLEVEL::LOGLEVEL_ERROR:
		{
			main_log->error(msg);
			break;
		}
		case LOGLEVEL::LOGLEVEL_CRITICAL:
		{
			main_log->critical(msg);
			break;
		}
		case LOGLEVEL::LOGLEVEL_INFO:
		{
			main_log->info(msg);
			break;
		}
		case LOGLEVEL::LOGLEVEL_DEBUG:
		{
			main_log->debug(msg);
			break;
		}
	}
	db_interface::Instance().log_event(log_level, script_path, msg);
}
void log_interface::log(LOGLEVEL log_level, std::string msg)
{
	log(log_level, "", msg);
}
void log_interface::log_error(std::string msg)
{
	log(LOGLEVEL::LOGLEVEL_ERROR, "", msg);
}
