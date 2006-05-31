AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
   WMS/FdoWmsOvProvider.h \
   WMS/Override/FdoWmsOvClassCollection.h \
   WMS/Override/FdoWmsOvClassDefinition.h \
   WMS/Override/FdoWmsOvFormatType.h \
   WMS/Override/FdoWmsOvLayerCollection.h \
   WMS/Override/FdoWmsOvLayerDefinition.h \
   WMS/Override/FdoWmsOvOverrides.h \
   WMS/Override/FdoWmsOvPhysicalSchemaMapping.h \
   WMS/Override/FdoWmsOvRasterDefinition.h \
   WMS/Override/FdoWmsOvStyleDefinition.h

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/WMS ]; then mkdir -p $(INSTALL_DIR)/WMS; fi
	@cp -u -f ./WMS/*.h  $(INSTALL_DIR)/WMS
	@if [ ! -d $(INSTALL_DIR)/WMS/Override ]; then mkdir -p $(INSTALL_DIR)/WMS/Override; fi
	@cp -u -f ./WMS/Override/*.h  $(INSTALL_DIR)/WMS/Override

uninstall :
	@rm -drf $(INSTALL_DIR)/WMS

