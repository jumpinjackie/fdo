AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = /usr/local/fdo-3.1.0/include

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/Shp ]; then mkdir -p $(INSTALL_DIR)/Shp; fi
	@if [ ! -d $(INSTALL_DIR)/Shp/Override ]; then mkdir -p $(INSTALL_DIR)/Shp/Override; fi
	@cp -u -f ./Shp/*.h  $(INSTALL_DIR)/Shp
	@cp -u -f ./Shp/Override/*.h  $(INSTALL_DIR)/Shp/Override

