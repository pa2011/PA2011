OGRE_SDK=C:/SDKs/OgreSDK


copy_general_resources.txt : ../../Resources
	cp -R ../../Resources/* $(CONTENT_PATH)/Resources
	touch copy_general_resources.txt

copy_specific_resources.txt : $(shell find Resources)
	cp -R Resources/* $(CONTENT_PATH)/Resources
	touch copy_specific_resources.txt

copy_runtime_dlls.txt : $(RUNTIME_DLL_PATH)bin/libgcc_s_dw2-1.dll $(RUNTIME_DLL_PATH)bin/libstdc++-6.dll
	cp $(RUNTIME_DLL_PATH)bin/libgcc_s_dw2-1.dll $(CONTENT_PATH)
	cp $(RUNTIME_DLL_PATH)bin/libstdc++-6.dll $(CONTENT_PATH)
	touch copy_runtime_dlls.txt

copy_ogre_dlls.txt : $(OGRE_SDK)/bin/$(CONFIGURATION)
	cp -R $(OGRE_SDK)/bin/$(CONFIGURATION)/* $(CONTENT_PATH)
	touch copy_ogre_dlls.txt


all: copy_general_resources.txt copy_specific_resources.txt copy_runtime_dlls.txt copy_ogre_dlls.txt


clean:
	rm -rf $(CONTENT_PATH)/*

