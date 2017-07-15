##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=septemserver
ConfigurationName      :=Debug
WorkspacePath          :=/home/ken/git-repos/septemmud
ProjectPath            :=/home/ken/git-repos/septemmud/septemserver
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=ken
Date                   :=14/07/17
CodeLitePath           :=/home/ken/.codelite
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="septemserver.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  -lz -ldl -Wl,-export-dynamic
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include/ $(IncludeSwitch)../sol2 $(IncludeSwitch)../plog/include $(IncludeSwitch)/home/ken/git-repos/lua-5.3.4/src $(IncludeSwitch)../../json/src $(IncludeSwitch)../telnetpp/include $(IncludeSwitch)../terminalpp/include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system $(LibrarySwitch)boost_program_options $(LibrarySwitch)boost_filesystem $(LibrarySwitch)lua $(LibrarySwitch)pthread $(LibrarySwitch)boost_date_time $(LibrarySwitch)telnetpp $(LibrarySwitch)terminalpp 
ArLibs                 :=  "boost_system" "boost_program_options" "boost_filesystem" "lua" "pthread" "boost_date_time" "telnetpp" "terminalpp" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/home/ken/git-repos/lua-5.3.4/src $(LibraryPathSwitch)../telnetpp/Debug $(LibraryPathSwitch)../terminalpp/Debug 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++14 -Wall -ftemplate-depth=10000 -ftemplate-backtrace-limit=0 $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_game_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_entity_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_heartbeat_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_luatypes.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_septem.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_script_entity.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_commandobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_roomobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_itemobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_container_base.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_playerobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_actionobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_doorobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_net_socket.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_net_server.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_server_context_impl.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_server_connection.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_server_client.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_server_httpserv.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

PostBuild:
	@echo Executing Post Build commands ...
	cp config.ini Debug
	@echo Done

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_game_manager.cpp$(ObjectSuffix): src/game_manager.cpp $(IntermediateDirectory)/src_game_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/game_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_game_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_game_manager.cpp$(DependSuffix): src/game_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_game_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_game_manager.cpp$(DependSuffix) -MM src/game_manager.cpp

$(IntermediateDirectory)/src_game_manager.cpp$(PreprocessSuffix): src/game_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_game_manager.cpp$(PreprocessSuffix) src/game_manager.cpp

$(IntermediateDirectory)/src_entity_manager.cpp$(ObjectSuffix): src/entity_manager.cpp $(IntermediateDirectory)/src_entity_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/entity_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_entity_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_entity_manager.cpp$(DependSuffix): src/entity_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_entity_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_entity_manager.cpp$(DependSuffix) -MM src/entity_manager.cpp

$(IntermediateDirectory)/src_entity_manager.cpp$(PreprocessSuffix): src/entity_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_entity_manager.cpp$(PreprocessSuffix) src/entity_manager.cpp

$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix): src/main.cpp $(IntermediateDirectory)/src_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main.cpp$(DependSuffix): src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main.cpp$(DependSuffix) -MM src/main.cpp

$(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix): src/main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main.cpp$(PreprocessSuffix) src/main.cpp

$(IntermediateDirectory)/src_heartbeat_manager.cpp$(ObjectSuffix): src/heartbeat_manager.cpp $(IntermediateDirectory)/src_heartbeat_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/heartbeat_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_heartbeat_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_heartbeat_manager.cpp$(DependSuffix): src/heartbeat_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_heartbeat_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_heartbeat_manager.cpp$(DependSuffix) -MM src/heartbeat_manager.cpp

$(IntermediateDirectory)/src_heartbeat_manager.cpp$(PreprocessSuffix): src/heartbeat_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_heartbeat_manager.cpp$(PreprocessSuffix) src/heartbeat_manager.cpp

$(IntermediateDirectory)/src_luatypes.cpp$(ObjectSuffix): src/luatypes.cpp $(IntermediateDirectory)/src_luatypes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/luatypes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_luatypes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_luatypes.cpp$(DependSuffix): src/luatypes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_luatypes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_luatypes.cpp$(DependSuffix) -MM src/luatypes.cpp

$(IntermediateDirectory)/src_luatypes.cpp$(PreprocessSuffix): src/luatypes.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_luatypes.cpp$(PreprocessSuffix) src/luatypes.cpp

$(IntermediateDirectory)/src_septem.cpp$(ObjectSuffix): src/septem.cpp $(IntermediateDirectory)/src_septem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/septem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_septem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_septem.cpp$(DependSuffix): src/septem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_septem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_septem.cpp$(DependSuffix) -MM src/septem.cpp

$(IntermediateDirectory)/src_septem.cpp$(PreprocessSuffix): src/septem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_septem.cpp$(PreprocessSuffix) src/septem.cpp

$(IntermediateDirectory)/src_script_entities_script_entity.cpp$(ObjectSuffix): src/script_entities/script_entity.cpp $(IntermediateDirectory)/src_script_entities_script_entity.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/script_entity.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_script_entity.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_script_entity.cpp$(DependSuffix): src/script_entities/script_entity.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_script_entity.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_script_entity.cpp$(DependSuffix) -MM src/script_entities/script_entity.cpp

$(IntermediateDirectory)/src_script_entities_script_entity.cpp$(PreprocessSuffix): src/script_entities/script_entity.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_script_entity.cpp$(PreprocessSuffix) src/script_entities/script_entity.cpp

$(IntermediateDirectory)/src_script_entities_commandobj.cpp$(ObjectSuffix): src/script_entities/commandobj.cpp $(IntermediateDirectory)/src_script_entities_commandobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/commandobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_commandobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_commandobj.cpp$(DependSuffix): src/script_entities/commandobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_commandobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_commandobj.cpp$(DependSuffix) -MM src/script_entities/commandobj.cpp

$(IntermediateDirectory)/src_script_entities_commandobj.cpp$(PreprocessSuffix): src/script_entities/commandobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_commandobj.cpp$(PreprocessSuffix) src/script_entities/commandobj.cpp

$(IntermediateDirectory)/src_script_entities_roomobj.cpp$(ObjectSuffix): src/script_entities/roomobj.cpp $(IntermediateDirectory)/src_script_entities_roomobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/roomobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_roomobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_roomobj.cpp$(DependSuffix): src/script_entities/roomobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_roomobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_roomobj.cpp$(DependSuffix) -MM src/script_entities/roomobj.cpp

$(IntermediateDirectory)/src_script_entities_roomobj.cpp$(PreprocessSuffix): src/script_entities/roomobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_roomobj.cpp$(PreprocessSuffix) src/script_entities/roomobj.cpp

$(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(ObjectSuffix): src/script_entities/daemonobj.cpp $(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/daemonobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(DependSuffix): src/script_entities/daemonobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(DependSuffix) -MM src/script_entities/daemonobj.cpp

$(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(PreprocessSuffix): src/script_entities/daemonobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(PreprocessSuffix) src/script_entities/daemonobj.cpp

$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(ObjectSuffix): src/script_entities/livingentity.cpp $(IntermediateDirectory)/src_script_entities_livingentity.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/livingentity.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(DependSuffix): src/script_entities/livingentity.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(DependSuffix) -MM src/script_entities/livingentity.cpp

$(IntermediateDirectory)/src_script_entities_livingentity.cpp$(PreprocessSuffix): src/script_entities/livingentity.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_livingentity.cpp$(PreprocessSuffix) src/script_entities/livingentity.cpp

$(IntermediateDirectory)/src_script_entities_itemobj.cpp$(ObjectSuffix): src/script_entities/itemobj.cpp $(IntermediateDirectory)/src_script_entities_itemobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/itemobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_itemobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_itemobj.cpp$(DependSuffix): src/script_entities/itemobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_itemobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_itemobj.cpp$(DependSuffix) -MM src/script_entities/itemobj.cpp

$(IntermediateDirectory)/src_script_entities_itemobj.cpp$(PreprocessSuffix): src/script_entities/itemobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_itemobj.cpp$(PreprocessSuffix) src/script_entities/itemobj.cpp

$(IntermediateDirectory)/src_script_entities_container_base.cpp$(ObjectSuffix): src/script_entities/container_base.cpp $(IntermediateDirectory)/src_script_entities_container_base.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/container_base.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_container_base.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_container_base.cpp$(DependSuffix): src/script_entities/container_base.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_container_base.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_container_base.cpp$(DependSuffix) -MM src/script_entities/container_base.cpp

$(IntermediateDirectory)/src_script_entities_container_base.cpp$(PreprocessSuffix): src/script_entities/container_base.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_container_base.cpp$(PreprocessSuffix) src/script_entities/container_base.cpp

$(IntermediateDirectory)/src_script_entities_playerobj.cpp$(ObjectSuffix): src/script_entities/playerobj.cpp $(IntermediateDirectory)/src_script_entities_playerobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/playerobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_playerobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_playerobj.cpp$(DependSuffix): src/script_entities/playerobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_playerobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_playerobj.cpp$(DependSuffix) -MM src/script_entities/playerobj.cpp

$(IntermediateDirectory)/src_script_entities_playerobj.cpp$(PreprocessSuffix): src/script_entities/playerobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_playerobj.cpp$(PreprocessSuffix) src/script_entities/playerobj.cpp

$(IntermediateDirectory)/src_script_entities_actionobj.cpp$(ObjectSuffix): src/script_entities/actionobj.cpp $(IntermediateDirectory)/src_script_entities_actionobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/actionobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_actionobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_actionobj.cpp$(DependSuffix): src/script_entities/actionobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_actionobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_actionobj.cpp$(DependSuffix) -MM src/script_entities/actionobj.cpp

$(IntermediateDirectory)/src_script_entities_actionobj.cpp$(PreprocessSuffix): src/script_entities/actionobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_actionobj.cpp$(PreprocessSuffix) src/script_entities/actionobj.cpp

$(IntermediateDirectory)/src_script_entities_doorobj.cpp$(ObjectSuffix): src/script_entities/doorobj.cpp $(IntermediateDirectory)/src_script_entities_doorobj.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/script_entities/doorobj.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_entities_doorobj.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_entities_doorobj.cpp$(DependSuffix): src/script_entities/doorobj.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_entities_doorobj.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_entities_doorobj.cpp$(DependSuffix) -MM src/script_entities/doorobj.cpp

$(IntermediateDirectory)/src_script_entities_doorobj.cpp$(PreprocessSuffix): src/script_entities/doorobj.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_entities_doorobj.cpp$(PreprocessSuffix) src/script_entities/doorobj.cpp

$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix): src/fs/fs_manager.cpp $(IntermediateDirectory)/src_fs_fs_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/fs/fs_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fs_fs_manager.cpp$(DependSuffix): src/fs/fs_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fs_fs_manager.cpp$(DependSuffix) -MM src/fs/fs_manager.cpp

$(IntermediateDirectory)/src_fs_fs_manager.cpp$(PreprocessSuffix): src/fs/fs_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fs_fs_manager.cpp$(PreprocessSuffix) src/fs/fs_manager.cpp

$(IntermediateDirectory)/src_net_socket.cpp$(ObjectSuffix): src/net/socket.cpp $(IntermediateDirectory)/src_net_socket.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/net/socket.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_net_socket.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_net_socket.cpp$(DependSuffix): src/net/socket.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_net_socket.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_net_socket.cpp$(DependSuffix) -MM src/net/socket.cpp

$(IntermediateDirectory)/src_net_socket.cpp$(PreprocessSuffix): src/net/socket.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_net_socket.cpp$(PreprocessSuffix) src/net/socket.cpp

$(IntermediateDirectory)/src_net_server.cpp$(ObjectSuffix): src/net/server.cpp $(IntermediateDirectory)/src_net_server.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/net/server.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_net_server.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_net_server.cpp$(DependSuffix): src/net/server.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_net_server.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_net_server.cpp$(DependSuffix) -MM src/net/server.cpp

$(IntermediateDirectory)/src_net_server.cpp$(PreprocessSuffix): src/net/server.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_net_server.cpp$(PreprocessSuffix) src/net/server.cpp

$(IntermediateDirectory)/src_server_context_impl.cpp$(ObjectSuffix): src/server/context_impl.cpp $(IntermediateDirectory)/src_server_context_impl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/server/context_impl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_server_context_impl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_server_context_impl.cpp$(DependSuffix): src/server/context_impl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_server_context_impl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_server_context_impl.cpp$(DependSuffix) -MM src/server/context_impl.cpp

$(IntermediateDirectory)/src_server_context_impl.cpp$(PreprocessSuffix): src/server/context_impl.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_server_context_impl.cpp$(PreprocessSuffix) src/server/context_impl.cpp

$(IntermediateDirectory)/src_server_connection.cpp$(ObjectSuffix): src/server/connection.cpp $(IntermediateDirectory)/src_server_connection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/server/connection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_server_connection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_server_connection.cpp$(DependSuffix): src/server/connection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_server_connection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_server_connection.cpp$(DependSuffix) -MM src/server/connection.cpp

$(IntermediateDirectory)/src_server_connection.cpp$(PreprocessSuffix): src/server/connection.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_server_connection.cpp$(PreprocessSuffix) src/server/connection.cpp

$(IntermediateDirectory)/src_server_client.cpp$(ObjectSuffix): src/server/client.cpp $(IntermediateDirectory)/src_server_client.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/server/client.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_server_client.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_server_client.cpp$(DependSuffix): src/server/client.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_server_client.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_server_client.cpp$(DependSuffix) -MM src/server/client.cpp

$(IntermediateDirectory)/src_server_client.cpp$(PreprocessSuffix): src/server/client.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_server_client.cpp$(PreprocessSuffix) src/server/client.cpp

$(IntermediateDirectory)/src_server_httpserv.cpp$(ObjectSuffix): src/server/httpserv.cpp $(IntermediateDirectory)/src_server_httpserv.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/server/httpserv.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_server_httpserv.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_server_httpserv.cpp$(DependSuffix): src/server/httpserv.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_server_httpserv.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_server_httpserv.cpp$(DependSuffix) -MM src/server/httpserv.cpp

$(IntermediateDirectory)/src_server_httpserv.cpp$(PreprocessSuffix): src/server/httpserv.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_server_httpserv.cpp$(PreprocessSuffix) src/server/httpserv.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


