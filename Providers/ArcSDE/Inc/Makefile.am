AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/ArcSDE/Override ]; then mkdir -p $(INSTALL_DIR)/ArcSDE/Override; fi
	@cp -u -f ./ArcSDE/Override/*.h  $(INSTALL_DIR)/ArcSDE/Override

uninstall :
	@rm -drf $(INSTALL_DIR)/ArcSDE
