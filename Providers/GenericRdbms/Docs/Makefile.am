AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = /usr/local/fdo-3.2.0

SUBDIRS = doc_src

DIST_SUBDIRS = doc_src

EXTRA_DIST = XmlSchema/FdoMySQLOverride.xsd XmlSchema/FdoRdbmsOverride.xsd XmlSchema/FdoSQLServerOverride.xsd

#install doc files
install :
	@if [ ! -d $(INSTALL_DIR)/Docs ]; then mkdir -p $(INSTALL_DIR)/Docs; fi
	@if [ ! -d $(INSTALL_DIR)/Docs/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/Docs/XmlSchema; fi
	@$(FDO)/Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema $(INSTALL_DIR)/Docs/XmlSchema/XmlSchema

