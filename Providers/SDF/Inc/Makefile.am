AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
   SDF/ICreateSDFFile.h \
   SDF/SdfCommandType.h

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/SDF ]; then mkdir -p $(INSTALL_DIR)/SDF; fi
	@cp -u -f ./SDF/*.h  $(INSTALL_DIR)/SDF

uninstall :
	@rm -drf $(INSTALL_DIR)/SDF

