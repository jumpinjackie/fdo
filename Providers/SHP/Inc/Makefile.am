AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
   SHP/FdoShpOverrides.h \
   SHP/FdoShpProvider.h \
   SHP/Override/FdoShpClassCollection.h \
   SHP/Override/FdoShpClassDefinition.h \
   SHP/Override/FdoShpColumnDefinition.h \
   SHP/Override/FdoShpPhysicalSchemaMapping.h \
   SHP/Override/FdoShpPropertyDefinition.h \
   SHP/Override/FdoShpPropertyDefinitionCollection.h

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/SHP ]; then mkdir -p $(INSTALL_DIR)/SHP; fi
	@if [ ! -d $(INSTALL_DIR)/SHP/Override ]; then mkdir -p $(INSTALL_DIR)/SHP/Override; fi
	@cp -u -f ./SHP/*.h  $(INSTALL_DIR)/SHP
	@cp -u -f ./SHP/Override/*.h  $(INSTALL_DIR)/SHP/Override

uninstall :
	@rm -drf $(INSTALL_DIR)/SHP

