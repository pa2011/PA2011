OGRE_SDK=C:/SDKs/OgreSDK

all:
	# create directories (if they don't exist)
	mkdir -p $(CONTENT_PATH)/Resources
	 	
	# copy platform independent resources
	cp -R ../../Resources/* $(CONTENT_PATH)/Resources
	
	# copy platform specific resources
	cp -R Resources/* $(CONTENT_PATH)/Resources
	
	# copy runtime DLLs
	cp $(RUNTIME_DLL_PATH)bin/libgcc_s_dw2-1.dll $(CONTENT_PATH)
	cp $(RUNTIME_DLL_PATH)bin/libstdc++-6.dll $(CONTENT_PATH)
	
	# copy Ogre DLLs
	cp -R $(OGRE_SDK)/bin/$(CONFIGURATION)/* $(CONTENT_PATH)

	
clean:
	rm -rf $(CONTENT_PATH)/*
