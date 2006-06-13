AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = resource.h GRFPMessage.mc RFPMessage.vcproj RFPMessageStatic.rc_ MSG00001.bin

CATDIR = ../../Nls/Linux/en_US

CATALOGS = GRFPMessage.mc

CATALOGS_CAT = $(patsubst %.mc,$(CATDIR)/%.cat,$(CATALOGS))

all: catalog_init $(CATALOGS_CAT)

clean :
	@rm -rf $(CATDIR)/GRFPMessage.cat  
	@rm -f Inc/GRFPMessage.h
	@rm -f GRFPMessage.msf

catalog_init ::
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
	@mv ../../Inc/$(basename $(notdir $<))_msg.h ../../Inc/$(basename $(notdir $<)).h

%.msf : %.mc
	$(FDO)/Unmanaged/Src/McToMsf/McToMsf $< $@ "*" GRFPMESSAGE

INSTALL_DIR  = $(prefix)/nls

install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@cp -u -f $(CATDIR)/GRFPMessage.cat  $(INSTALL_DIR)

uninstall :
	@rm -f $(INSTALL_DIR)/GRFPMessage.cat

