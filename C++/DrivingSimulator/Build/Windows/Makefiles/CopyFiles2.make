RUNTIME_DLL_PATH=$(MINGW_HOME)/bin

CONTENT_PATH=bin/$(PRODUCT)/$(CONFIGURATION)
DEP=.makedep/$(PRODUCT)/$(CONFIGURATION)

create_directories:
	$(MSYS_HOME)/bin/mkdir -p $(DEP)
	$(MSYS_HOME)/bin/mkdir -p $(CONTENT_PATH)/Resources

$(DEP)/copy_general_resources : $(shell $(MSYS_HOME)/bin/find ../../Resources)
	cp -R ../../Resources/* $(CONTENT_PATH)/Resources/
	touch $@

$(DEP)/copy_specific_resources : $(shell $(MSYS_HOME)/bin/find Resources)
	cp -R Resources/* $(CONTENT_PATH)/Resources/
	touch $@
	
$(DEP)/copy_user_resources : $(shell $(MSYS_HOME)/bin/find UserResources)
	cp -R UserResources/* $(CONTENT_PATH)/Resources/
	touch $@

$(DEP)/copy_runtime_dlls : $(RUNTIME_DLL_PATH)/libgcc_s_dw2-1.dll $(RUNTIME_DLL_PATH)/libstdc++-6.dll
	cp $(RUNTIME_DLL_PATH)/libgcc_s_dw2-1.dll $(CONTENT_PATH)
	cp $(RUNTIME_DLL_PATH)/libstdc++-6.dll $(CONTENT_PATH)
	touch $@

$(DEP)/copy_ogre_dlls : $(shell $(MSYS_HOME)/bin/find $(OGRE_HOME)/bin/$(CONFIGURATION))
	cp -R $(OGRE_HOME)/bin/$(CONFIGURATION)/*.dll $(CONTENT_PATH)
	touch $@
	
$(DEP)/copy_ode_dlls : $(shell $(MSYS_HOME)/bin/find $(OGREODE_HOME)/ode/lib/$(CONFIGURATION)SingleDLL)
	cp -R $(OGREODE_HOME)/ode/lib/$(CONFIGURATION)SingleDLL/*.dll $(CONTENT_PATH)
	touch $@
	
$(DEP)/copy_ogreode_dlls : $(shell $(MSYS_HOME)/bin/find $(OGREODE_HOME)/build/bin)
	cp -R $(OGREODE_HOME)/build/bin/*.dll $(CONTENT_PATH)
	touch $@

all: create_directories $(DEP)/copy_general_resources $(DEP)/copy_specific_resources $(DEP)/copy_user_resources $(DEP)/copy_runtime_dlls $(DEP)/copy_ogre_dlls $(DEP)/copy_ode_dlls $(DEP)/copy_ogreode_dlls

clean:
	rm -rf $(DEP)
	rm -rf $(CONTENT_PATH)
	
nothing:
	# do nothing

