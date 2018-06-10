#include "stdafx.h"

#include "entity_manager.h"

#include "game_manager.h"
#include "script_entities/playerobj.h"
#include "septem.hpp"
#include "io/db_interface.h"

#include "server/connectionsm.hpp"
#include "server/httpserv.h"

namespace ba = boost::asio;
namespace po = boost::program_options;

#define CONFIG_FILE_NAME "config.ini"

boost::mutex global_stream_lock;

static void run_io_service(boost::asio::io_service &io_service)
{
    io_service.run();
}

void TimerHandler(
	const boost::system::error_code & error, 
	boost::shared_ptr< boost::asio::deadline_timer > timer, 
	boost::shared_ptr< boost::asio::io_service::strand > strand
)
{
	if( error )
	{
		global_stream_lock.lock();
		std::cout << "[" << boost::this_thread::get_id()
			<< "] Error: " << error << std::endl;
		global_stream_lock.unlock();
	}
	else
	{
       // entity_manager::Instance().invoke_heartbeat();
		//std::cout << "[" << boost::this_thread::get_id()
		//	<< "] TimerHandler " << std::endl;
        std::unique_lock<std::recursive_mutex> lock(*entity_manager::Instance().GetLuaMutex());
        entity_manager::Instance().get_heartbeat_manager().do_heartbeats();
        entity_manager::Instance().invoke_actions();

		timer->expires_from_now( boost::posix_time::seconds( 1 ) );
		timer->async_wait( 
			strand->wrap( boost::bind( &TimerHandler, _1, timer, strand ) )
		);
	}
}


int main(int argc, char **argv)
{
    
    boost::property_tree::ptree pt;

    std::string working_directory = boost::filesystem::current_path().string();
    try
    {
    	boost::property_tree::ini_parser::read_ini(CONFIG_FILE_NAME, pt);
    }
    catch(std::exception & ex)
    {
    	std::cerr << "Failed to find file: " << CONFIG_FILE_NAME << " in directory working directory " << working_directory << std::endl;
    }
    std::cout << pt.get<std::string>("options.port") << std::endl;
    
 
    
    unsigned int port        = 4000;
    std::string  threads     = "";
    unsigned int concurrency = 0;
    
    po::options_description description("Available options");
    description.add_options()
        ( "help,h",                                       "show this help message"                            )
        ( "port,p",    po::value<unsigned int>(&port),    "port number"                                       )
        ( "threads,t", po::value<std::string>(&threads),  "number of threads of execution (0 for autodetect)" )
        ;

    
    po::positional_options_description pos_description;
    pos_description.add("port", -1);
    try
    {
        po::variables_map vm;
        po::store(
            po::command_line_parser(argc, argv)
                .options(description)
                .positional(pos_description)
                .run()
          , vm);
        
        po::notify(vm);
      
        if (vm.count("help") != 0)
        {
            throw po::error("");
        }
        else if (vm.count("port") == 0)
        {
            throw po::error("Port number must be specified");
        }

        if (vm.count("threads") == 0)
        {
            concurrency = 1;
        }
        else
        {
            try
            {
                concurrency = boost::lexical_cast<unsigned int>(threads);
            }
            catch(...)
            {
            	std::cerr << "Concurrency not found, setting to zero." << std::endl;
                // Failure is to be expected here, since it might be an empty
                // string.  In this case, concurrency will be a detectable 0.
            }
            
            if (concurrency == 0)
            {
                concurrency = std::thread::hardware_concurrency();
            }

            // According to the Boost docs, "thread::hardware_concurrency()" 
            // may return 0 on platforms that don't have information available
            // about cores/hyperthreading units, etc.  In this case, we will
            // default to one thread.
            if (concurrency == 0)
            {
                concurrency = 1;
            }
        }
    }
    catch(po::error &err)
    {
        if (strlen(err.what()) == 0)
        {
            std::cout << boost::format("USAGE: %s <port number>|<options>\n")
                        % argv[0]
                 << description
                 << std::endl;
                 
            return EXIT_SUCCESS;
        }
        else
        {
            std::cerr << boost::format("ERROR: %s\n\nUSAGE: %s <port number>|<options>\n")
                        % err.what()
                        % argv[0]
                 << description
                 << std::endl;
        }
        
        return EXIT_FAILURE;
    }
    std::string game_data_path =  working_directory + "/game_data/";
    std::string maybe_data_path = pt.get<std::string>("options.game_data");
    if( !maybe_data_path.empty() )
    {
        if( maybe_data_path[maybe_data_path.size()-1] != '/' )
            maybe_data_path += "/";
        maybe_data_path += "game_data/";
        game_data_path =  maybe_data_path;
    }
    global_settings::Instance().SetSetting( DEFAULT_GAME_DATA_PATH, game_data_path);
    global_settings::Instance().SetSetting( BASE_PLAYER_ENTITY_PATH, "entities/player");
    global_settings::Instance().SetSetting( DEFAULT_VOID_ROOM, "realms/void");
    global_settings::Instance().SetSetting( DEFAULT_DAEMON_PATH, "daemon");
    global_settings::Instance().SetSetting( DEFAULT_COMMANDS_PATH, "cmds");
    global_settings::Instance().SetSetting( DEFAULT_CREATOR_COMMANDS_PATH, "cmds/creator");
    global_settings::Instance().SetSetting( DEFAULT_ARCH_COMMANDS_PATH, "cmds/creator/arch");
    global_settings::Instance().SetSetting( DEFAULT_COMMAND_PROC, "daemon/command_proc");
    global_settings::Instance().SetSetting( DEFAULT_LOGON_PROC, "daemon/logon_proc");
    global_settings::Instance().SetSetting( DEFAULT_LIBS_PATH, "lib");
    global_settings::Instance().SetSetting( DEFAULT_LOGS_PATH, "logs/");
    global_settings::Instance().SetSetting( DEFAULT_ROOM_LOG_PATH, "roomlog");
    global_settings::Instance().SetSetting( DEFAULT_BASE_SAVE_PATH, "save");
    global_settings::Instance().SetSetting( DEFAULT_ACCOUNTS_PATH, "accounts");
    global_settings::Instance().SetSetting( DEFAULT_WORKSPACES_PATH, "workspaces");
    global_settings::Instance().SetSetting( DEFAULT_WORKROOM_NAME, "workroom");
    global_settings::Instance().SetSetting( DEFAULT_DEFAULT_WORKSPACE, "workspaces/default");
    global_settings::Instance().SetSetting( DEFAULT_ROOM_CACHE_PATH, "save");
    
    // account related things..
    global_settings::Instance().SetSetting( ACCOUNT_PLAYERNAME, "player_name");
    global_settings::Instance().SetSetting( ACCOUNT_PASSWORD, "password");
    global_settings::Instance().SetSetting( ACCOUNT_WORKSPACE, "workspace_path");
    global_settings::Instance().SetSetting( ACCOUNT_TYPE, "account_type");
    global_settings::Instance().SetSetting( ACCOUNT_LASTLOGON, "last_logon");
    global_settings::Instance().SetSetting( ACCOUNT_EMAIL, "email");
	global_settings::Instance().SetSetting( ACCOUNT_GENDER, "gender" );

    
    auto log = spd::get("main");
  
    if( !log )
    {
        spd::set_pattern("[%x %H:%M:%S:%e] %v");


        std::string entityLog = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH) +
                            global_settings::Instance().GetSetting(DEFAULT_LOGS_PATH);


        if(boost::filesystem::exists(entityLog) == false)
        {
        	bool attempt_create = boost::filesystem::create_directories(entityLog);
        	if(attempt_create == false)
        	{
        		std::cerr << "Could not create logging directories: " << entityLog << std::endl;
        		return EXIT_FAILURE;
        	}
        	else
        	{
        		std::clog << "Logging Directory Created: " << entityLog << std::endl;
        	}
        }
        else
        {
        	std::clog << "Logging Directory Found: " << entityLog << std::endl;
        }

        entityLog += "septem";
        
        try
        {

            std::vector<spdlog::sink_ptr> sinks;
            //PD rotating file sink only takes 4 arguments.
            auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt> (entityLog, "log", 1024*1024, 5);
            auto stdout_sink = spdlog::sinks::stdout_sink_mt::instance();

            sinks.push_back(stdout_sink);
            sinks.push_back(rotating);
            
            auto combined_logger = std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks));
            spdlog::register_logger(combined_logger);
            combined_logger->set_level( spdlog::level::debug );
            log = spd::get("main");
            
        }
        catch( std::exception & ex )
        {
            std::cerr << "Exception Setting up logging: " << ex.what() << std::endl;
            return EXIT_FAILURE;
        }
       
   }

    
    boost::asio::io_service io_service;
    boost::shared_ptr< boost::asio::io_service::strand > strand(
		new boost::asio::io_service::strand( io_service )
	);
	
	entity_manager::Instance().bind_io(&io_service);
    
    game_manager gm;
    gm.start();
	
    septem application(
        io_service
      , std::make_shared<boost::asio::io_service::work>(std::ref(io_service))
      , port);
       
       
  //  auto s = std::make_shared<boost::asio::io_service::work>(std::ref(io_service));
 		
    std::vector<std::thread> threadpool;

    for (unsigned int thr = 0; thr < concurrency; ++thr)
    {
        threadpool.emplace_back(&run_io_service, std::ref(io_service));
    }
    
    boost::shared_ptr< boost::asio::deadline_timer > timer(
		new boost::asio::deadline_timer( io_service )
	);
    
	timer->expires_from_now( boost::posix_time::seconds( 1 ) );
	timer->async_wait( 
		strand->wrap( boost::bind( &TimerHandler, _1, timer, strand ) )
	);
    
    std::thread http_thread =  std::thread(start_serv, 8090);
    
	// test database..
	//db_interface::Instance().test();
	std::string pname = "caliel";
    long l = db_interface::Instance().get_last_logon(pname);
	std::time_t temp = l;
	
	//std::time_t now = std::time(NULL);
	std::tm * ptm = std::localtime(&temp);
	char buffer[32];
	// Format: Mo, 15.06.2009 20:20:00
	std::strftime(buffer, 32, "%a, %d.%m.%Y %H:%M:%S", ptm);  
	std::cout << buffer << std::endl;

    std::string blah; // for console input
    while( true )
    {
        std::cout<<"SERVER [-h for help]>";
            
        std::getline(std::cin, blah);
        if( blah == "stop" )
        {
            gm.stop();
            
        }
        else if( blah == "start" )
        {
            gm.start();
            
        }
        if( blah == "hb" )
        {
            entity_manager::Instance().invoke_heartbeat();
        }
        
    }

    for (auto &pthread : threadpool)
    {
        pthread.join();
    }
    http_thread.join();
    
    io_service.stop();

    
	return 0;
}
