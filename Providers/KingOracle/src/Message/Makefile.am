##
## Copyright (C) 2006  SL-King d.o.o
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

CATDIR = ../../nls/linux/en_US

CATALOGS = ShpMessage.mc

CATALOGS_CAT = $(patsubst %.mc,$(CATDIR)/%.cat,$(CATALOGS))

all : catalog_init $(CATALOGS_CAT)

clean :
	@rm -rf $(CATDIR)/*.cat
	@rm -f *.msf

catalog_init ::
	@if [ ! -d $(CATDIR) ]; then mkdir -p $(CATDIR); fi


$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
	@mkdir -p "inc/"
	@rm -f "inc/$(basename $(notdir $<)).h"
	@echo "../../../../thirdparty/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
    	../../../../thirdparty/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
    	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
    	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	inc/$(addsuffix _msg.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
    	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv inc/$(basename $(notdir $<))_msg.h inc/$(basename $(notdir $<))Dynamic.h
	@cat $(basename $(notdir $<))Static.h inc/$(basename $(notdir $<))Dynamic.h >inc/$(basename $(notdir $<)).h
	@rm inc/$(basename $(notdir $<))Dynamic.h

%.msf : %.mc
	../../../../fdo/Unmanaged/src/McToMsf/McToMsf $< $@ "*" SHPMESSAGE

INSTALL_DIR  = /usr/local/fdo-3.1.0

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/nls ]; then mkdir -p $(INSTALL_DIR)/nls; fi
	@cp -u -f $(CATALOGS_CAT)  $(INSTALL_DIR)/nls

