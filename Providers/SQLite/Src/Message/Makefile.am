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

EXTRA_DIST = resource.h SQLiteMessage.mc SQLiteMessage.vcproj SQLiteMessageStatic.rc

CATDIR = ./nls/linux/en_US

CATALOGS = SQLiteMessage.msf

CATALOGS_CAT = $(patsubst %.msf,$(CATDIR)/%.cat,$(CATALOGS))

all : catalog_init $(CATALOGS_CAT)

clean :
	@rm -rf $(CATDIR)/*.cat  
	@rm -f Inc/*.h
	@rm -f *.msf

catalog_init ::
	@if [ ! -d $(CATDIR) ]; then mkdir -p $(CATDIR); fi

$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
	@mkdir -p "Inc/"
	@rm -f "./Inc/$(basename $(notdir $<)).h"
	@echo "$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
    	$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
    	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
    	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	./Inc/$(addsuffix _msg.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
    	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv ./Inc/$(basename $(notdir $<))_msg.h ./Inc/$(basename $(notdir $<)).h

SQLiteMessage.msf : SQLiteMessage.mc
	$(FDO)/Unmanaged/Src/McToMsf/McToMsf SQLiteMessage.mc SQLiteMessage.msf SQLite

INSTALL_DIR  = $(prefix)/nls

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f $(CATDIR)/SQLiteMessage.cat  $(INSTALL_DIR)

uninstall :
	@rm -f $(INSTALL_DIR)/SQLiteMessage.cat

