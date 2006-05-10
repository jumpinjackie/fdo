AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/OWS ]; then mkdir -p $(INSTALL_DIR)/OWS; fi
	@cp -u -f ./OWS/*.h  $(INSTALL_DIR)/OWS

uninstall :
	@rm -drf $(INSTALL_DIR)/OWS

