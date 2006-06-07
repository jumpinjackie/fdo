AUTOMAKE_OPTIONS = foreign

CATDIR = ../../nls/linux/en_US

CATALOGS = Message.mc

CATALOGS_CAT = $(patsubst %.mc,$(CATDIR)/%.cat,$(CATALOGS))

all: catalog_init $(CATALOGS_CAT)

clean:
	@rm -rf $(CATDIR)/*.cat  
	@rm -rf *.msf

catalog_init::
	@if [ ! -d $(CATDIR) ]; then mkdir -p $(CATDIR); fi


$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
	@rm -f "../../Inc/$(basename $(notdir $<)).h"
	@echo "$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
    	$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
    	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
    	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	../../Inc/$(addsuffix _msg.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
    	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv ../../Inc/$(basename $(notdir $<))_msg.h ../../Inc/Fdo$(basename $(notdir $<)).h
	@mv $(CATDIR)/Message.cat $(CATDIR)/FDOMessage.cat

%.msf : %.mc
	@../McToMsf/McToMsf $(basename $(notdir $<)).mc $(basename $(notdir $<)).msf "*" FDO


INSTALL_DIR  = $(prefix)/nls

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f $(CATDIR)/FDOMessage.cat $(INSTALL_DIR)

uninstall :
	@rm -f $(INSTALL_DIR)/FDOMessage.cat

