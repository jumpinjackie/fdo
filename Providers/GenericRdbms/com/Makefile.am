AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = $(libdir)

#install com files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/com ]; then mkdir -p $(INSTALL_DIR)/com; fi
	@cp -u -f *.sql  $(INSTALL_DIR)/com

