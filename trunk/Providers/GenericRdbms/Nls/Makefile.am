##
## Copyright (C) 2004-2006  Autodesk, Inc.
## 
## This library is free software; you can redistribute it and/or
## modify it under the terms of version 2.1 of the GNU Lesser
## General Public License as published by the Free Software Foundation.
## 
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## Lesser General Public License for more details.
## 
## You should have received a copy of the GNU Lesser General Public
## License along with this library; if not, write to the Free Software
## Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
##

AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = resource.h fdordbmsmsg.mc fdordbms.vcproj fdordbmsmsgStatic.rc

CATDIR = nls/linux/en_US

CATALOGS = \
fdordbmsmsg.mc

CATALOGS_CAT = $(patsubst %.mc,$(CATDIR)/%.cat,$(CATALOGS))

all: catalog_init $(CATALOGS_CAT)

clean:
	@rm -rf $(CATDIR)/*.cat  
	@rm -f ../Inc/Nls/fdordbmsmsg.h
	@rm -f *.msf

catalog_init::
	@if [ ! -d $(CATDIR) ]; then mkdir -p $(CATDIR); fi


$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
	@mkdir -p "../Inc/Nls/"
	@rm -f "../Inc/Nls/$(basename $(notdir $<)).h"
	@echo "$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
    	$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
    	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
    	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	../Inc/Nls/$(addsuffix _msg.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
    	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv ../Inc/Nls/$(basename $(notdir $<))_msg.h ../Inc/Nls/$(basename $(notdir $<)).h

%.msf : %.mc
	@echo "@$(FDO)/Unmanaged/Src/McToMsf/McToMsf $(basename $(notdir $<)).mc $(basename $(notdir $<)).msf "*" FDORDBMS"; \
	$(FDO)/Unmanaged/Src/McToMsf/McToMsf $(basename $(notdir $<)).mc $(basename $(notdir $<)).msf  "*"  FDORDBMS

INSTALL_DIR  = $(prefix)/nls

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f $(CATALOGS_CAT)  $(INSTALL_DIR)

uninstall :
	@rm -f $(INSTALL_DIR)/fdordbmsmsg.cat

