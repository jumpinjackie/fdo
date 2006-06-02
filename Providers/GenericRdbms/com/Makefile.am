AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = $(libdir)

EXTRA_DIST = fdo_sys.sql fdo_sys_idx.sql fdosys_sys.sql

#install com files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/com ]; then mkdir -p $(INSTALL_DIR)/com; fi
	@cp -u -f *.sql  $(INSTALL_DIR)/com

uninstall :
	@rm -f $(INSTALL_DIR)/com/*.sql
