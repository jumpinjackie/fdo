AUTOMAKE_OPTIONS = foreign

INSTALL_DIR  = /usr/local/fdo-3.2.0

#install Docs
install :
	@if [ ! -d $(INSTALL_DIR)/Docs ]; then mkdir -p $(INSTALL_DIR)/Docs; fi
	@if [ ! -d $(INSTALL_DIR)/Docs/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/Docs/XmlSchema; fi
	@if [ ! -d $(INSTALL_DIR)/Docs/XmlSchema/Gml ]; then mkdir -p $(INSTALL_DIR)/Docs/XmlSchema/Gml; fi
	@if [ ! -d $(INSTALL_DIR)/Docs/XmlSchema/Xlink ]; then mkdir -p $(INSTALL_DIR)/Docs/XmlSchema/Xlink; fi
	@if [ ! -d $(INSTALL_DIR)/Docs/XmlSchema/XmlSchema ]; then mkdir -p $(INSTALL_DIR)/Docs/XmlSchema/XmlSchema; fi
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema $(INSTALL_DIR)/Docs/XmlSchema
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema/Gml $(INSTALL_DIR)/Docs/XmlSchema/Gml
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema/Xlink $(INSTALL_DIR)/Docs/XmlSchema/Xlink
	@../Install/XsdInstallPaths/XsdInstallPaths ./XmlSchema/XmlSchema $(INSTALL_DIR)/Docs/XmlSchema/XmlSchema
	@cp -u -f XmlSchema/Gml/CopyRight.txt $(INSTALL_DIR)/Docs/XmlSchema/Gml
	@cp -u -f XmlSchema/Xlink/CopyRight.txt $(INSTALL_DIR)/Docs/XmlSchema/Xlink
	@cp -u -f XmlSchema/XmlSchema/base.css $(INSTALL_DIR)/Docs/XmlSchema/XmlSchema
