# -*- CMakeLists.txt For septemserver.  (C) Caliel of Septem aka Ken Thompson, 2018 -*-
cmake_minimum_required(VERSION 2.8.11)

# Project name
project(septemserver)

# This setting is useful for providing JSON file used by CodeLite for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set default locations
set( SOL2_PATH /home/ken/git-repos/septemmud/sol2 )
set( LUA_PATH /home/ken/lua-5.3.5/src )
set( PLOG_PATH /home/ken/git-repos/septemmud/plog/include )    
set( JSON_PATH /home/ken/git-repos/septemmud/json/include )
set( TELNETPP_PATH /home/ken/git-repos/septemmud/telnetpp/include )   
set( TERMINALPP_PATH /home/ken/git-repos/septemmud/terminalpp/include )  
set( SQLITECPP_PATH /home/ken/git-repos/septemmud/SQLiteCpp/include)
set( SPDLOG_PATH /home/ken/git-repos/spdlog/include)
set( WEBSERVER_PATH  /home/ken/git-repos/septemmud/Simple-Web-Server)

MESSAGE( "CHECKING INCLUDE PATHS..")
IF(EXISTS "${SOL2_PATH}" AND IS_DIRECTORY "${SOL2_PATH}")
	MESSAGE( "FOUND SOL2_PATH : ${SOL2_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "SOL2_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${LUA_PATH}" AND IS_DIRECTORY "${LUA_PATH}")
	MESSAGE( "FOUND LUA_PATH : ${LUA_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "LUA_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${PLOG_PATH}" AND IS_DIRECTORY "${PLOG_PATH}")
	MESSAGE( "FOUND PLOG_PATH : ${PLOG_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "PLOG_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${JSON_PATH}" AND IS_DIRECTORY "${JSON_PATH}")
	MESSAGE( "FOUND JSON_PATH : ${JSON_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "JSON_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${TELNETPP_PATH}" AND IS_DIRECTORY "${TELNETPP_PATH}")
	MESSAGE( "FOUND TELNETPP_PATH : ${TELNETPP_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "TELNETPP_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${TERMINALPP_PATH}" AND IS_DIRECTORY "${TERMINALPP_PATH}")
	MESSAGE( "FOUND TERMINALPP_PATH : ${TERMINALPP_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "TERMINALPP_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${SQLITECPP_PATH}" AND IS_DIRECTORY "${SQLITECPP_PATH}")
	MESSAGE( "FOUND SQLITECPP_PATH : ${SQLITECPP_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "SQLITECPP_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${SPDLOG_PATH}" AND IS_DIRECTORY "${SPDLOG_PATH}")
	MESSAGE( "FOUND SPDLOG_PATH : ${SPDLOG_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "SPDLOG_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
IF(EXISTS "${WEBSERVER_PATH}" AND IS_DIRECTORY "${WEBSERVER_PATH}")
	MESSAGE( "FOUND WEBSERVER_PATH : ${WEBSERVER_PATH}")
	ELSE()
	MESSAGE( FATAL_ERROR "WEBSERVER_PATH IS UNDEFINED OR INCORRECT")
ENDIF()
MESSAGE( "DONE CHECKING INCLUDE PATHS.")
  
  # sudo killall -9 rsync
# Notes on CL_OUTPUT_DIRECOTORY, see https://cmake.org/cmake/help/v3.0/variable/CMAKE_CURRENT_LIST_DIR.html
#When CMake starts processing commands in a source file it sets this variable to the directory where this file is located. When CMake finishes processing commands from the file it restores the previous value. 
#Therefore the value of the variable inside a macro or function is the directory of the file invoking the bottom-most entry on the call stack, not the directory of the file containing the macro or function definition.
# CMAKE_CURRENT_LIST_DIR : Full directory of the listfile currently being processed.
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/../cmake-build-Debug/output)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY}) #stole this from codelite
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

set(CONFIGURATION_NAME "Debug")

# Projects
# Top project
# Define some variables
set(PROJECT_septemserver_PATH "${CMAKE_CURRENT_LIST_DIR}")
set(WORKSPACE_PATH "${CMAKE_CURRENT_LIST_DIR}/..")

set( SOL2_PATH /home/ken/git-repos/septemmud/sol2 )
set( LUA_PATH /home/ken/lua-5.3.5/src )
set( PLOG_PATH /home/ken/git-repos/septemmud/plog/include )    
set( JSON_PATH /home/ken/git-repos/septemmud/json/include )
set( TELNETPP_PATH /home/ken/git-repos/septemmud/telnetpp/include )   
set( TERMINALPP_PATH /home/ken/git-repos/septemmud/terminalpp/include )  
set( SQLITECPP_PATH /home/ken/git-repos/septemmud/SQLiteCpp/include)
set( SPDLOG_PATH /home/ken/git-repos/spdlog/include)
set( WEBSERVER_PATH  /home/ken/git-repos/septemmud/Simple-Web-Server)

#need to fix these hard-coded paths in the future
include_directories(
    .
    .
    include/
    ${SOL2_PATH}
    ${PLOG_PATH}
    ${LUA_PATH}
    ${JSON_PATH}
    ${TELNETPP_PATH}
    ${TERMINALPP_PATH}
	${SQLITECPP_PATH}
	${SPDLOG_PATH}
	${WEBSERVER_PATH}
)


# Compiler options
add_definitions(-g)
add_definitions(-O0)
add_definitions(-std=c++14)
add_definitions(-Wall)
add_definitions(-ftemplate-depth=10000)
add_definitions(-ftemplate-backtrace-limit=0)
add_definitions(-pipe)
add_definitions(-no-pie)

# Linker options
#set(LINK_OPTIONS -lz)
set(LINK_OPTIONS -ldl)
#set(LINK_OPTIONS ${LINK_OPTIONS} -Wl,-E)
#set(LINK_OPTIONS ${LINK_OPTIONS} -no-pie)

find_library(Z_LIB z)

if(WIN32)
    # Resource options
endif(WIN32)

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. -L/home/ken/lua-5.3.5/src -L../telnetpp/Debug -L../terminalpp/Debug -L/usr/local/lib -L../SQLiteCpp/Debug -L/usr/lib/x86_64-linux-gnu/ -L/opt/lib -lz")

# Define the C sources
set ( C_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/sqllite/sqlite3.c
)

# Define the CXX sources
set ( CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/lookobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/entity_ptr.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/fs/fs_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/heartbeat_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/security_context.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/global_settings.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/game_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/itemobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/account.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/server.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/entity_wrapper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/entity_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/septem.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/luatypes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/loghelper.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/livingentity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/inventory_slot.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/server/context_impl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/account_manager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/exitobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/handobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/roomobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/doorobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/script_entity.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/daemonobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/container_base.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/playerobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/actionobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/extcommandobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/server/httpserv.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/net/socket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/server/connection.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/npcobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/server/client.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/script_entities/commandobj.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/io/log_interface.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/io/db_interface.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -O0 -std=c++14 -Wall -ftemplate-depth=10000 -ftemplate-backtrace-limit=0 -pipe -no-pie")

set_source_files_properties(
    ${C_SRCS} PROPERTIES COMPILE_FLAGS 
    " -g -O0 -Wall")

if(WIN32)
    enable_language(RC)
    set(CMAKE_RC_COMPILE_OBJECT
        "<CMAKE_RC_COMPILER> ${RC_OPTIONS} -O coff -i <SOURCE> -o <OBJECT>")
endif(WIN32)



#{{{{ User Code 2
# Place your code here
#}}}}

link_directories(/home/ken/git-repos/septemmud/telnetpp/Debug /home/ken/git-repos/septemmud/terminalpp/Debug /home/ken/git-repos/septemmud/SQLiteCpp/Debug /usr/lib/x86_64-linux-gnu)

add_executable(septemserver ${RC_SRCS} ${CXX_SRCS} ${C_SRCS})
target_link_libraries(septemserver ${LINK_OPTIONS})

target_link_libraries(septemserver
    boost_system
    boost_program_options
    boost_filesystem
    lua
    pthread
    boost_date_time
    telnetpp
    terminalpp
    cryptopp
    sqlite3
    SQLiteCpp
	${Z_LIB}
)


# Adding POST_BUILD commands
add_custom_command(
    TARGET septemserver
    POST_BUILD
    COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/config.ini /home/ken/git-repos/septemmud/septemserver/Debug)



#{{{{ User Code 3
# Place your code here
#}}}}
