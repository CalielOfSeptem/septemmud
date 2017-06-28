#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "entity_manager.h"
#include "global_settings.h"
#include "config.h"
#include "game_manager.h"
#include "script_entities/playerobj.h"
#include "septem.hpp"

#include "server/connectionsm.hpp"
#include "server/httpserv.h"
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind.hpp>

namespace ba = boost::asio;
namespace po = boost::program_options;

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
        entity_manager::Instance().get_heartbeat_manager().do_heartbeats();

		timer->expires_from_now( boost::posix_time::seconds( 120 ) );
		timer->async_wait( 
			strand->wrap( boost::bind( &TimerHandler, _1, timer, strand ) )
		);
	}
}


int main(int argc, char **argv)
{
    
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);
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
    
    global_settings::Instance().SetSetting( DEFAULT_GAME_DATA_PATH, "/home/ken/git-repos/septemmud/game_data/");
    global_settings::Instance().SetSetting( BASE_PLAYER_ENTITY_PATH, "entities/player");
    global_settings::Instance().SetSetting( DEFAULT_VOID_ROOM, "realms/void");
    global_settings::Instance().SetSetting( DEFAULT_DAEMON_PATH, "daemon");
    global_settings::Instance().SetSetting( DEFAULT_COMMANDS_PATH, "cmds");
    global_settings::Instance().SetSetting( DEFAULT_COMMAND_PROC, "daemon/command_proc");
    global_settings::Instance().SetSetting( DEFAULT_LIBS_PATH, "lib");
    global_settings::Instance().SetSetting( DEFAULT_LOGS_PATH, "logs/");
    global_settings::Instance().SetSetting( DEFAULT_ROOM_LOG_PATH, "roomlog");
    global_settings::Instance().SetSetting( DEFAULT_BASE_SAVE_PATH, "save");
    //chdir( global_settings::Instance().GetSetting( DEFAULT_GAME_DATA_PATH).c_str() );
    
   // std::string roomLog = global_settings::Instance().GetSetting( DEFAULT_GAME_DATA_PATH ) + global_settings::Instance().GetSetting( DEFAULT_LOGS_PATH ) + global_settings::Instance().GetSetting( DEFAULT_ROOM_LOG_PATH );
    
    
   // static plog::RollingFileAppender<plog::MyFormatter> fileAppender(roomLog.c_str(), 8000, 3);
    
   // static plog::ConsoleAppender<plog::MyFormatter> consoleAppenderA; // Create the 2nd appender.
    //static plog::RollingFileAppender<plog::MyFormatter> fileAppender;
    //fileAppend = plog::RollingFileAppender(const util::nchar* fileName, size_t maxFileSize = 0, int maxFiles = 0);
    //plog::init(plog::debug, &fileAppender).addAppender(&consoleAppender); // Initialize the logger with the both appenders.
   // plog::init<RoomLog>(plog::debug, &fileAppender).addAppender(&consoleAppenderA);
    
    /*
	std::string test_room = "/home/ken/git-repos/septemmud/game_data/realms/void";
    std::string reason;
    entity_manager::Instance().compile_script(test_room, reason);
    entity_manager::Instance().compile_script(test_room, reason);
    */
    /*
    for ( int x = 0; x< 2; x++)
    {
        test_room = "/home/ken/git-repos/septemmud/game_data/realms/void";
        entity_manager::Instance().compile_script(test_room, reason);
        
        test_room = "/home/ken/git-repos/septemmud/game_data/realms/test/room1";
        entity_manager::Instance().compile_script(test_room, reason);
        
        test_room = "/home/ken/git-repos/septemmud/game_data/realms/test/room2";
        entity_manager::Instance().compile_script(test_room, reason);
    
    }
     * 
     * 
    */
    
    auto log = spd::get("main");//>info("loggers can be retrieved from a global registry using the spdlog::get(logger_name) function");
  
    if( !log )
    {
        spd::set_pattern("[%x %H:%M:%S:%e] %v");
        std::string entityLog = global_settings::Instance().GetSetting(DEFAULT_GAME_DATA_PATH) +
                            global_settings::Instance().GetSetting(DEFAULT_LOGS_PATH);
        entityLog += "septem";
        
        try
        {
            std::vector<spdlog::sink_ptr> sinks;
            auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt> (entityLog, "log", 1024*1024, 5, true);
            auto stdout_sink = spdlog::sinks::stdout_sink_mt::instance();

            sinks.push_back(stdout_sink);
            sinks.push_back(rotating);
            
            auto combined_logger = std::make_shared<spdlog::logger>("main", begin(sinks), end(sinks));
            spdlog::register_logger(combined_logger);
            combined_logger->set_level( spdlog::level::debug );
            log = spd::get("main");
            //combined_logger->info(msg);
            //combined_logger->flush();
            
        }
        catch( std::exception& )
        {
            
        }
       
   }

    std::string blah;
        // our pho-player for expirementation purposes
    
        
    game_manager gm;
    gm.start();
    
    boost::asio::io_service io_service;
    boost::shared_ptr< boost::asio::io_service::strand > strand(
		new boost::asio::io_service::strand( io_service )
	);
    
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

    while( true )
    {
        if( entity_manager::Instance().get_player("caliel") != NULL )
        {
            playerobj * p = entity_manager::Instance().get_player("caliel");
            std::cout<< "[/" << p->cwd << "] >";
        }
        else
            std::cout<<">";
            
        std::getline(std::cin, blah);
        if( blah == "stop" )
        {
            gm.stop();
        }
        else if ( blah == "test void" )
        {
            if( entity_manager::Instance().get_void_room() != NULL )
            {
                std::cout << "VOID TEST: OK." << std::endl;
            }
        }
        else if( blah == "start" )
        {
            gm.start();
            
        }
        else if( blah == "login" )
        {
           // entity_manager::Instance().load_player("caliel");
        }
        else if( blah == "hb" )
        {
            entity_manager::Instance().invoke_heartbeat();
        }
        else
        {
            playerobj * p = entity_manager::Instance().get_player("caliel");
            if( p == NULL )
            {
                entity_manager::Instance().load_player("caliel");
                p = entity_manager::Instance().get_player("caliel");
            }
            assert( p != NULL );
            gm.process_player_cmd(p, blah);
            
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
