AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = $(includedir)

#install header files
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms ]; then mkdir -p $(INSTALL_DIR)/Rdbms; fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms/Override ]; then mkdir -p $(INSTALL_DIR)/Rdbms/Override; fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms/Override/MySql ]; then mkdir -p $(INSTALL_DIR)/Rdbms/Override/MySql; fi
	@if [ ! -d $(INSTALL_DIR)/Rdbms/Override/Odbc ]; then mkdir -p $(INSTALL_DIR)/Rdbms/Override/Odbc; fi

	@cp -u -f  Rdbms/*.h $(INSTALL_DIR)/Rdbms 
	@cp -u -f  Rdbms/Override/*.h $(INSTALL_DIR)/Rdbms/Override
	@cp -u -f  Rdbms/Override/MySQL/*.h $(INSTALL_DIR)/Rdbms/Override/MySql
	@cp -u -f  Rdbms/Override/ODBC/*.h $(INSTALL_DIR)/Rdbms/Override/Odbc

