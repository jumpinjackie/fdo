AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = /usr/local/fdo-3.2.0

#install doc files
install :
	@if [ ! -d $(INSTALL_DIR)/Docs ]; then mkdir -p $(INSTALL_DIR)/Docs; fi
	@if [ ! -d $(INSTALL_DIR)/Docs/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/Docs/XmlSchema; fi
	@$(FDO)/Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema $(INSTALL_DIR)/Docs/XmlSchema/XmlSchema

