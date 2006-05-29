AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = resource.h ShpMessage.mc ShpMessage.vcproj ShpMessageStatic.h ShpMessageStatic.rc

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
	@mkdir -p "Inc/"
	@rm -f "Inc/$(basename $(notdir $<)).h"
	@echo "$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
    	$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
    	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
    	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	Inc/$(addsuffix _msg.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
    	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv Inc/$(basename $(notdir $<))_msg.h Inc/$(basename $(notdir $<))Dynamic.h
	@cat $(basename $(notdir $<))Static.h Inc/$(basename $(notdir $<))Dynamic.h >Inc/$(basename $(notdir $<)).h
	@rm Inc/$(basename $(notdir $<))Dynamic.h

%.msf : %.mc
	$(FDO)/Unmanaged/Src/McToMsf/McToMsf $< $@ "*" SHPMESSAGE

INSTALL_DIR  = $(prefix)/nls

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f $(CATALOGS_CAT)  $(INSTALL_DIR)

uninstall :
	@rm -f $(INSTALL_DIR)/ShpMessage.cat

