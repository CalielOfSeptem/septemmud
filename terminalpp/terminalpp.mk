##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=terminalpp
ConfigurationName      :=Debug
WorkspacePath          :=/home/ken/git-repos/septemmud
ProjectPath            :=/home/ken/git-repos/septemmud/terminalpp
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Ken Thompson
Date                   :=06/11/18
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
OutputFile             :=$(IntermediateDirectory)/lib$(ProjectName).a
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="terminalpp.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)include/ 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -std=c++14 $(Preprocessors)
CFLAGS   :=  -g $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_canvas.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_terminal.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_canvas_view.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_string.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_screen.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_encoder.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_glyph.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_virtual_key.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ansi_control_sequence.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_ansi_charset.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_detail_parser.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_detail_terminal_control.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_detail_element_difference.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(IntermediateDirectory) $(OutputFile)

$(OutputFile): $(Objects)
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(AR) $(ArchiveOutputSwitch)$(OutputFile) @$(ObjectsFileList) $(ArLibs)
	@$(MakeDirCommand) "/home/ken/git-repos/septemmud/.build-debug"
	@echo rebuilt > "/home/ken/git-repos/septemmud/.build-debug/terminalpp"

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


./Debug:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_canvas.cpp$(ObjectSuffix): src/canvas.cpp $(IntermediateDirectory)/src_canvas.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/canvas.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_canvas.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_canvas.cpp$(DependSuffix): src/canvas.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_canvas.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_canvas.cpp$(DependSuffix) -MM src/canvas.cpp

$(IntermediateDirectory)/src_canvas.cpp$(PreprocessSuffix): src/canvas.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_canvas.cpp$(PreprocessSuffix) src/canvas.cpp

$(IntermediateDirectory)/src_terminal.cpp$(ObjectSuffix): src/terminal.cpp $(IntermediateDirectory)/src_terminal.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/terminal.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_terminal.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_terminal.cpp$(DependSuffix): src/terminal.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_terminal.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_terminal.cpp$(DependSuffix) -MM src/terminal.cpp

$(IntermediateDirectory)/src_terminal.cpp$(PreprocessSuffix): src/terminal.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_terminal.cpp$(PreprocessSuffix) src/terminal.cpp

$(IntermediateDirectory)/src_canvas_view.cpp$(ObjectSuffix): src/canvas_view.cpp $(IntermediateDirectory)/src_canvas_view.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/canvas_view.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_canvas_view.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_canvas_view.cpp$(DependSuffix): src/canvas_view.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_canvas_view.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_canvas_view.cpp$(DependSuffix) -MM src/canvas_view.cpp

$(IntermediateDirectory)/src_canvas_view.cpp$(PreprocessSuffix): src/canvas_view.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_canvas_view.cpp$(PreprocessSuffix) src/canvas_view.cpp

$(IntermediateDirectory)/src_string.cpp$(ObjectSuffix): src/string.cpp $(IntermediateDirectory)/src_string.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/string.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_string.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_string.cpp$(DependSuffix): src/string.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_string.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_string.cpp$(DependSuffix) -MM src/string.cpp

$(IntermediateDirectory)/src_string.cpp$(PreprocessSuffix): src/string.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_string.cpp$(PreprocessSuffix) src/string.cpp

$(IntermediateDirectory)/src_screen.cpp$(ObjectSuffix): src/screen.cpp $(IntermediateDirectory)/src_screen.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/screen.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_screen.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_screen.cpp$(DependSuffix): src/screen.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_screen.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_screen.cpp$(DependSuffix) -MM src/screen.cpp

$(IntermediateDirectory)/src_screen.cpp$(PreprocessSuffix): src/screen.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_screen.cpp$(PreprocessSuffix) src/screen.cpp

$(IntermediateDirectory)/src_encoder.cpp$(ObjectSuffix): src/encoder.cpp $(IntermediateDirectory)/src_encoder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/encoder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_encoder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_encoder.cpp$(DependSuffix): src/encoder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_encoder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_encoder.cpp$(DependSuffix) -MM src/encoder.cpp

$(IntermediateDirectory)/src_encoder.cpp$(PreprocessSuffix): src/encoder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_encoder.cpp$(PreprocessSuffix) src/encoder.cpp

$(IntermediateDirectory)/src_glyph.cpp$(ObjectSuffix): src/glyph.cpp $(IntermediateDirectory)/src_glyph.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/glyph.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_glyph.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_glyph.cpp$(DependSuffix): src/glyph.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_glyph.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_glyph.cpp$(DependSuffix) -MM src/glyph.cpp

$(IntermediateDirectory)/src_glyph.cpp$(PreprocessSuffix): src/glyph.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_glyph.cpp$(PreprocessSuffix) src/glyph.cpp

$(IntermediateDirectory)/src_virtual_key.cpp$(ObjectSuffix): src/virtual_key.cpp $(IntermediateDirectory)/src_virtual_key.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/virtual_key.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_virtual_key.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_virtual_key.cpp$(DependSuffix): src/virtual_key.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_virtual_key.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_virtual_key.cpp$(DependSuffix) -MM src/virtual_key.cpp

$(IntermediateDirectory)/src_virtual_key.cpp$(PreprocessSuffix): src/virtual_key.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_virtual_key.cpp$(PreprocessSuffix) src/virtual_key.cpp

$(IntermediateDirectory)/src_ansi_control_sequence.cpp$(ObjectSuffix): src/ansi/control_sequence.cpp $(IntermediateDirectory)/src_ansi_control_sequence.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/ansi/control_sequence.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ansi_control_sequence.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ansi_control_sequence.cpp$(DependSuffix): src/ansi/control_sequence.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ansi_control_sequence.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ansi_control_sequence.cpp$(DependSuffix) -MM src/ansi/control_sequence.cpp

$(IntermediateDirectory)/src_ansi_control_sequence.cpp$(PreprocessSuffix): src/ansi/control_sequence.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ansi_control_sequence.cpp$(PreprocessSuffix) src/ansi/control_sequence.cpp

$(IntermediateDirectory)/src_ansi_charset.cpp$(ObjectSuffix): src/ansi/charset.cpp $(IntermediateDirectory)/src_ansi_charset.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/ansi/charset.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_ansi_charset.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_ansi_charset.cpp$(DependSuffix): src/ansi/charset.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_ansi_charset.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_ansi_charset.cpp$(DependSuffix) -MM src/ansi/charset.cpp

$(IntermediateDirectory)/src_ansi_charset.cpp$(PreprocessSuffix): src/ansi/charset.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_ansi_charset.cpp$(PreprocessSuffix) src/ansi/charset.cpp

$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(ObjectSuffix): src/detail/terminal_charset_control.cpp $(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/detail/terminal_charset_control.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(DependSuffix): src/detail/terminal_charset_control.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(DependSuffix) -MM src/detail/terminal_charset_control.cpp

$(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(PreprocessSuffix): src/detail/terminal_charset_control.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_detail_terminal_charset_control.cpp$(PreprocessSuffix) src/detail/terminal_charset_control.cpp

$(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(ObjectSuffix): src/detail/well_known_virtual_key.cpp $(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/detail/well_known_virtual_key.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(DependSuffix): src/detail/well_known_virtual_key.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(DependSuffix) -MM src/detail/well_known_virtual_key.cpp

$(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(PreprocessSuffix): src/detail/well_known_virtual_key.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_detail_well_known_virtual_key.cpp$(PreprocessSuffix) src/detail/well_known_virtual_key.cpp

$(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(ObjectSuffix): src/detail/terminal_cursor_control.cpp $(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/detail/terminal_cursor_control.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(DependSuffix): src/detail/terminal_cursor_control.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(DependSuffix) -MM src/detail/terminal_cursor_control.cpp

$(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(PreprocessSuffix): src/detail/terminal_cursor_control.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_detail_terminal_cursor_control.cpp$(PreprocessSuffix) src/detail/terminal_cursor_control.cpp

$(IntermediateDirectory)/src_detail_parser.cpp$(ObjectSuffix): src/detail/parser.cpp $(IntermediateDirectory)/src_detail_parser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/detail/parser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_detail_parser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_detail_parser.cpp$(DependSuffix): src/detail/parser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_detail_parser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_detail_parser.cpp$(DependSuffix) -MM src/detail/parser.cpp

$(IntermediateDirectory)/src_detail_parser.cpp$(PreprocessSuffix): src/detail/parser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_detail_parser.cpp$(PreprocessSuffix) src/detail/parser.cpp

$(IntermediateDirectory)/src_detail_terminal_control.cpp$(ObjectSuffix): src/detail/terminal_control.cpp $(IntermediateDirectory)/src_detail_terminal_control.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/detail/terminal_control.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_detail_terminal_control.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_detail_terminal_control.cpp$(DependSuffix): src/detail/terminal_control.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_detail_terminal_control.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_detail_terminal_control.cpp$(DependSuffix) -MM src/detail/terminal_control.cpp

$(IntermediateDirectory)/src_detail_terminal_control.cpp$(PreprocessSuffix): src/detail/terminal_control.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_detail_terminal_control.cpp$(PreprocessSuffix) src/detail/terminal_control.cpp

$(IntermediateDirectory)/src_detail_element_difference.cpp$(ObjectSuffix): src/detail/element_difference.cpp $(IntermediateDirectory)/src_detail_element_difference.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/ken/git-repos/septemmud/terminalpp/src/detail/element_difference.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_detail_element_difference.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_detail_element_difference.cpp$(DependSuffix): src/detail/element_difference.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_detail_element_difference.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_detail_element_difference.cpp$(DependSuffix) -MM src/detail/element_difference.cpp

$(IntermediateDirectory)/src_detail_element_difference.cpp$(PreprocessSuffix): src/detail/element_difference.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_detail_element_difference.cpp$(PreprocessSuffix) src/detail/element_difference.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


