AUTOMAKE_OPTIONS = foreign

CATDIR = ./nls/linux/en_US

CATALOGS = SDFMessage.msf

CATALOGS_CAT = $(patsubst %.msf,$(CATDIR)/%.cat,$(CATALOGS))

all : catalog_init inc_init $(CATALOGS_CAT)

clean :
	@rm -rf $(CATDIR)/*.cat  

catalog_init::
	@if [ ! -d $(CATDIR) ]; then mkdir -p $(CATDIR); fi

inc_init::
	@if [ ! -d Inc ]; then mkdir -p Inc; fi


$(CATALOGS_CAT) : $(CATDIR)/%.cat : %.msf
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

SDFMessage.msf : SDFMessage.mc
	$(FDO)/Unmanaged/Src/McToMsf/McToMsf SDFMessage.mc SDFMessage.msf SDF

INSTALL_DIR  = /usr/local/fdo-3.2.0

#install data
install :
	@if [ ! -d $(INSTALL_DIR) ]; then mkdir -p $(INSTALL_DIR); fi
	@if [ ! -d $(INSTALL_DIR)/nls ]; then mkdir -p $(INSTALL_DIR)/nls; fi
	@cp -u -f $(CATDIR)/SDFMessage.cat  $(INSTALL_DIR)/nls

