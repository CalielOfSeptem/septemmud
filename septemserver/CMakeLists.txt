cmake_minimum_required (VERSION 3.6)
project (Septemmud)

find_package(spdlog)

set(LUA_HEADER_DIR /usr/include/lua5.3)

set(JSON_HEADER_DIR ../json/src)
set(SCRIPT_ENTITIES src/script_entities)
set(SRC src)
set(INCLUDE include)
set(FILESYSTEM src/fs)
set(NET src/net)
set(SERVER src/server)
set(SCRIPT_ENTITIES_INCLUDE_DIR ${INCLUDE}/script_entities)
set(SOL_HEADER_DIR ../sol2)

list(APPEND DIRECTORIES 
 ${SCRIPT_ENTITIES} 
 ${SCRIPT_ENTITIES_INCLUDE_DIR} 
 ${SRC} 
 ${INCLUDE} 
 ${FILESYSTEM} 
 ${NET} 
 ${SERVER} 
 ${SOL_HEADER_DIR}
 ${LUA_HEADER_DIR}
 ${JSON_HEADER_DIR}
 )

include_directories(${DIRECTORIES})

set(SCRIPT_ENTITIES_SOURCES
	${SCRIPT_ENTITIES}/script_entity.cpp
	${SCRIPT_ENTITIES}/commandobj.cpp
	${SCRIPT_ENTITIES}/roomobj.cpp
	${SCRIPT_ENTITIES}/daemonobj.cpp
	${SCRIPT_ENTITIES}/livingentity.cpp
	${SCRIPT_ENTITIES}/itemobj.cpp
	${SCRIPT_ENTITIES}/container_base.cpp
	${SCRIPT_ENTITIES}/playerobj.cpp
	${SCRIPT_ENTITIES}/actionobj.cpp
	${SCRIPT_ENTITIES}/doorobj.cpp)
	
set(SOURCES
	${SRC}/game_manager.cpp
	${SRC}/entity_manager.cpp
	${SRC}/main.cpp
	${SRC}/heartbeat_manager.cpp
	${SRC}/luatypes.cpp
	${SRC}/septem.cpp)
	
set(FILESYSTEM_SOURCES
	${FILESYSTEM}/fs_manager.cpp)
	
set(NET_SOURCES
	${NET}/socket.cpp
	${NET}/server.cpp)

set(SERVER_SOURCES
	${SERVER}/context_impl.cpp
	${SERVER}/connection.cpp
	${SERVER}/client.cpp
	${SERVER}/httpserv.cpp)

list(APPEND SRC  
			${SCRIPT_ENTITIES_SOURCES}
			${SOURCES}
			${FILESYSTEM_SOURCES}
			${NET_SOURCES}
			${SERVER_SOURCES})
			
set(SCRIPT_ENTITIES_HEADERS
	${SCRIPT_ENTITIES_INCLUDE_DIR}/script_entity.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/roomobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/playerobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/container_base.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/livingentity.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/daemonobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/commandobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/itemobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/doorobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/actionobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/lookobj.h
	${SCRIPT_ENTITIES_INCLUDE_DIR}/exitobj.h)
	
set(BASE_HEADERS
	${INCLUDE}/global_settings.h
	${INCLUDE}/string_utils.h
	${INCLUDE}/entity_wrapper.h
	${INCLUDE}/heartbeat_manager.h
	${INCLUDE}/config.h
	${INCLUDE}/game_manager.h
	${INCLUDE}/loghelper.h
	${INCLUDE}/entity_manager.h
	${INCLUDE}/luatypes.h
	${INCLUDE}/world_context.hpp
	${INCLUDE}/septem.hpp
	${INCLUDE}/welcome_screen.hpp)  
  
set(IO_HEADERS
	${INCLUDE}/io/datastream.hpp
	${INCLUDE}/io/input_datastream.hpp
	${INCLUDE}/io/output_datastream.hpp)

set(FS_HEADERS
	${INCLUDE}/fs/fs_manager.h)

set(NET_HEADERS
	${INCLUDE}/net/socket.hpp
	${INCLUDE}/net/server.hpp)
	
set(SERVER_HEADERS
	${INCLUDE}/server/connectionsm.hpp
	${INCLUDE}/server/context_impl.hpp
	${INCLUDE}/server/httpserv.h
	${INCLUDE}/server/client.hpp
	${INCLUDE}/server/connection.hpp)
	
list(APPEND HEADERS 
				${SCRIPT_ENTITIES_HEADERS} 
				${BASE_HEADERS}
				${FS_HEADERS}
				${IO_HEADERS}
				${NET_HEADERS}
				${SERVER_HEADERS})
				
add_executable(septemmud
			${SRC}
			${HEADERS})
