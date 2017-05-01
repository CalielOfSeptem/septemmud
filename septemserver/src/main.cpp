#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <thread>
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <plog/Log.h>
#include <plog/Appenders/ConsoleAppender.h>
#include "entity_manager.h"
#include "global_settings.h"
#include "config.h"
#include "game_manager.h"

namespace ba = boost::asio;
namespace po = boost::program_options;

int main(int argc, char **argv)
{
    
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini("config.ini", pt);
    std::cout << pt.get<std::string>("options.port") << std::endl;
    
    //std::string log_path = std::string(DEFAULT_LOG_PATH) + "septem";
   // static plog::RollingFileAppender<plog::TxtFormatter> fileAppender(log_path.c_str(), 8000, 3); // Create the 1st appender.
    static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender; // Create the 2nd appender.
    plog::init(plog::verbose, &consoleAppender);//.addAppender(&consoleAppender); // Initialize the logger with the both appenders.

    
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
    */
    std::string blah;
        // our pho-player for expirementation purposes
    
        
    game_manager gm;
    gm.start();
   // entity_manager::Instance().load_player();
    while( true )
    {
        std::cout<<">";
        std::getline(std::cin, blah);
        if( blah == "stop" )
        {
            gm.stop();
        }
        else if ( blah == "test void" )
        {
            if( gm.get_void_room() != NULL )
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
            entity_manager::Instance().load_player();
        }
        else
        {
            gm.process_player_cmd(NULL, blah);
            // std::cout << "Invoking heartbeats.." << std::endl;
             //gm.do_heartbeats();
             //entity_manager::Instance().invoke_heartbeat();
        }
        
    }

    
	return 0;
}
