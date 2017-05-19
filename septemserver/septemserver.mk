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
Date                   :=19/05/17
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
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include/ $(IncludeSwitch)../sol2 $(IncludeSwitch)../plog/include $(IncludeSwitch)/home/ken/git-repos/lua-5.3.4/src $(IncludeSwitch)../../json/src 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)boost_system $(LibrarySwitch)boost_program_options $(LibrarySwitch)boost_filesystem $(LibrarySwitch)lua $(LibrarySwitch)pthread $(LibrarySwitch)boost_date_time 
ArLibs                 :=  "boost_system" "boost_program_options" "boost_filesystem" "lua" "pthread" "boost_date_time" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)/home/ken/git-repos/lua-5.3.4/src 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++14 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_game_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_entity_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_heartbeat_manager.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_script_entity.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_commandobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_roomobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_daemonobj.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_livingentity.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_entities_itemobj.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix) 



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

$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix): src/fs/fs_manager.cpp $(IntermediateDirectory)/src_fs_fs_manager.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/septemserver/src/fs/fs_manager.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_fs_fs_manager.cpp$(DependSuffix): src/fs/fs_manager.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_fs_fs_manager.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_fs_fs_manager.cpp$(DependSuffix) -MM src/fs/fs_manager.cpp

$(IntermediateDirectory)/src_fs_fs_manager.cpp$(PreprocessSuffix): src/fs/fs_manager.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_fs_fs_manager.cpp$(PreprocessSuffix) src/fs/fs_manager.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


