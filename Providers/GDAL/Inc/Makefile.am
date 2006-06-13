AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = \
   GdalFile/FdoGrfpProvider.h \
   GdalFile/Override/FdoGrfpClassDefinition.h \
   GdalFile/Override/FdoGrfpOverrides.h \
   GdalFile/Override/FdoGrfpPhysicalSchemaMapping.h \
   GdalFile/Override/FdoGrfpRasterBandDefinition.h \
   GdalFile/Override/FdoGrfpRasterDefinition.h \
   GdalFile/Override/FdoGrfpRasterFeatureDefinition.h \
   GdalFile/Override/FdoGrfpRasterGeoreferenceLocation.h \
   GdalFile/Override/FdoGrfpRasterImageDefinition.h \
   GdalFile/Override/FdoGrfpRasterLocation.h

INSTALL_DIR  = $(includedir)

install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/GdalFile ]; then mkdir -p $(INSTALL_DIR)/GdalFile; fi
	@cp -urf ./GdalFile  $(INSTALL_DIR)/GdalFile

uninstall :
	@rm -drf $(INSTALL_DIR)/GdalFile
