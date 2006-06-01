AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = resource.h ArcSDEMessage.mc ArcSDEMessage.vcproj ArcSDEMessageStatic.rc

CATDIR = ../nls/linux/en_US

CATALOGS = ArcSDEMessage.msf

CATALOGS_CAT = $(patsubst %.msf,$(CATDIR)/%.cat,$(CATALOGS))

all: init_dirs $(CATALOGS_CAT)

clean:
	@rm -rf $(CATDIR)/*.cat  
	@rm -f Inc/*.h
	@rm -f *.msf

init_dirs::
	@mkdir -p $(CATDIR)
	@mkdir -p ./Inc


$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
	@mkdir -p "Inc/"
	@rm -f "./$(basename $(notdir $<)).h"
	@echo "$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< | gencat $@"; \
	$(FDOTHIRDPARTY)/linux/mkcatdefs/mkcatdefs $(basename $(notdir $<)) $< > $(addsuffix .tmp, $(basename $(notdir $<))); \
	gencat $@ $(addsuffix .tmp, $(basename $(notdir $<))); \
	sed -e '/#include/d' -e '/MF_/s/"\(.*\)\.cat"/\1_cat/' \
	$(addsuffix _msg.h, $(basename $(notdir $<))) > \
	./$(addsuffix _msg2.h, $(basename $(notdir $<)))
	@rm $(addsuffix _msg.h, $(basename $(notdir $<))) \
	$(addsuffix .tmp, $(basename $(notdir $<)))
	@mv ./$(basename $(notdir $<))_msg2.h ./Inc/$(basename $(notdir $<)).h

ArcSDEMessage.msf : ArcSDEMessage.mc
	$(FDO)/Unmanaged/Src/McToMsf/McToMsf $< $@ "*" ARCSDEMESSAGE

INSTALL_DIR  = $(prefix)

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/nls ]; then mkdir -p $(INSTALL_DIR)/nls; fi
	@cp -u -f $(CATALOGS_CAT)  $(INSTALL_DIR)/nls

uninstall :
	@rm -f $(INSTALL_DIR)/nls/ArcSDEMessage.cat
