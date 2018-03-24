.PHONY: clean All

All:
	@echo "----------Building project:[ septemserver - Debug ]----------"
	@cd "septemserver" && "$(MAKE)" -f  "septemserver.mk" ./include/stdafx.h.gch && "$(MAKE)" -f  "septemserver.mk" && "$(MAKE)" -f  "septemserver.mk" PostBuild
clean:
	@echo "----------Cleaning project:[ septemserver - Debug ]----------"
	@cd "septemserver" && "$(MAKE)" -f  "septemserver.mk" clean
