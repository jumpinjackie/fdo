AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = /usr/local/fdo-3.2.0/include

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/SHP ]; then mkdir -p $(INSTALL_DIR)/SHP; fi
	@if [ ! -d $(INSTALL_DIR)/SHP/Override ]; then mkdir -p $(INSTALL_DIR)/SHP/Override; fi
	@cp -u -f ./SHP/*.h  $(INSTALL_DIR)/SHP
	@cp -u -f ./SHP/Override/*.h  $(INSTALL_DIR)/SHP/Override

