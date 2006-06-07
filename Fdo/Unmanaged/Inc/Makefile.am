AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = $(includedir)

#install header files
install :
	@mkdir -p $(INSTALL_DIR)
	@cp -r -u -f *  $(INSTALL_DIR)
	@rm -f  $(INSTALL_DIR)/Common/Xsl/TransformerXalan.h
	@rm -f  $(INSTALL_DIR)/FdoSpatial.h
	@rm -rf $(INSTALL_DIR)/Spatial
	@rm -rf $(INSTALL_DIR)/Makefile*

uninstall :
	@rm -f $(INSTALL_DIR)/*.h
	@rm -drf $(INSTALL_DIR)/Fdo
	@rm -drf $(INSTALL_DIR)/Common
	@rm -drf $(INSTALL_DIR)/Geometry


