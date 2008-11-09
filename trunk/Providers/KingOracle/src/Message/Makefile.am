##
## Copyright (C) 2004-2007  Autodesk, Inc.
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

EXTRA_DIST = resource.h \
             KgOraMessage.mc \
             KingOracleMessage.vcproj \
             KgOraMessageStatic.rc \
             KgOraMessageStatic.h 

CATDIR = ../../Nls/Linux/en_US

CATALOGS = KgOraMessage.mc

CATALOGS_CAT = $(patsubst %.mc,$(CATDIR)/%.cat,$(CATALOGS))

all: catalog_init $(CATALOGS_CAT)

clean :
	@rm -rf $(CATDIR)/KgOraMessage.cat  
	@rm -f ../../inc/KgOraMessage.h
	@rm -f KgOraMessage.msf

catalog_init ::
	@if [ ! -d $(CATDIR) ]; then mkdir -p $(CATDIR); fi

$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
	@rm -f "../../inc/$(basename $(notdir $<)).h"
	@echo "../../../../Thirdparty/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
	../../../../Thirdparty/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	../../inc/$(addsuffix _msg.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv ../../inc/$(basename $(notdir $<))_msg.h ../../inc/$(basename $(notdir $<)).h

%.msf : %.mc
	../../../../Fdo/Unmanaged/Src/McToMsf/McToMsf $< $@ "*" KINGORACLEMESSAGE

INSTALL_DIR  = $(prefix)/nls

install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f $(CATDIR)/KgOraMessage.cat  $(INSTALL_DIR)

uninstall :
	@rm -f $(INSTALL_DIR)/KgOraMessage.cat

