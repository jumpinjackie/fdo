AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = /usr/local/fdo-3.2.0

#install header files
install :
	@mkdir -p $(INSTALL_DIR)/include
	@cp -r -u -f *  $(INSTALL_DIR)/include
	@rm -f  $(INSTALL_DIR)/include/Common/Xsl/TransformerXalan.h
	@rm -f  $(INSTALL_DIR)/include/Spatial.h
	@rm -rf $(INSTALL_DIR)/include/Spatial
