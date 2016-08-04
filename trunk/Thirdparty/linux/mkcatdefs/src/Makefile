#----------------------------------------------------------------------
# Super Makefile
#
# Things that can be made:
#		libs:		object library.
#		mkcatdefs:	executable application.
#		all:		mkcatdefs and man page.
#		print:		print sources.
#		print.man:	print mkcatdefs man page.
#		lint:		lint  sources.
#		clean:		purge objects and generated files.
#
# Modification history
#   Mnemonic    Release Date        Who     Review
#
#   NLSTOOLS    victor  92jan11     Mike Tardif
#       Created.
#
#   OSFPORT    	tools  	94jun30     Mike Tardif
#       Tweaked for OSF1 and Solaris ports.
#
#   HPUXPORT   	tools  	95mar17     Weibo Wang
#       For HP-UX ports.
#
#	ID: $RCSfile: Makefile $ $Revision: 4.3 $ $Date: 1997/02/11 18:52:47 $ $Author: greg $
#----------------------------------------------------------------------
SHELL		= /bin/sh
PRINTHOST	= beet
PRINTER		= 
PS_PRINTER	= -Pps
MANDIR		= $(ROOT)/man/man
M1TAG		= 1
M1DIR		= ${MANDIR}${M1TAG}
C_SOURCES	=	main.c		\
				msf.c		\
				msg.c		\
				args.c
ALL_SOURCES =	Makefile	\
				makefile  	\
				mkcatdefs.h	\
				$(C_SOURCES)

all: mkcatdefs mkcatdefs.${M1TAG}

.DEFAULT:
	if [ $(ARCH) = AIX ]; then												\
		echo "$@ is native on $(ARCH) platforms.";							\
	elif [ $(ARCH) = OSF1 ]; then											\
		echo "$@ is native on $(ARCH) platforms.";							\
	elif [ $(ARCH) = HPUX ]; then											\
		gmake -f makefile.mkcatdefs											\
			LD_OPTS='' $@;													\
	elif [ $(ARCH) = Sol ]; then											\
		gmake -f makefile.mkcatdefs											\
			LOCAL_DEV_ENV='-DSVR4'											\
			LD_OPTS='' $@;													\
	elif [ $(ARCH) = Sun4 ]; then											\
		gmake -f makefile.mkcatdefs											\
			CC=/usr/5bin/cc													\
			LOCAL_DEV_ENV='' 												\
			LD_OPTS='' $@;													\
	elif [ $(ARCH) = mips ]; then											\
		echo "$@ is native on $(ARCH) platforms.";							\
	fi

mkcatdefs.${M1TAG}:	mkcatdefs.man
	@if [ ! -d $(M1DIR) ]; then mkdir -p $(M1DIR); fi
	cat mkcatdefs.man > ${M1DIR}/mkcatdefs.${M1TAG}

help:
	@echo ""   
	@echo "Targets that can be made from this makefile:"
	@echo "    help:        this menu."
	@echo "    libs:        object library."
	@echo "    mkcatdefs:   executable application."
	@echo "    all:     	mkcatdefs and man page."
	@echo "    print:       print sources."
	@echo "    print.man:   print mkcatdefs man page."
	@echo "    lint:        lint sources."
	@echo "    clean:       purge objects and generated files."

print.man:	mkcatdefs.${M1TAG}
	psroff -t -man  mkcatdefs.${M1TAG} | rsh $(PRINTHOST) lpr $(PS_PRINTER)

print:
	@for file in $(ALL_SOURCES); do \
		pr -e4 -f $$file; \
	done | rsh $(PRINTHOST) lpr $(PRINTER)
	-@rsh $(PRINTHOST) lpq $(PRINTER)

lint:
	lint -ua -D_AIX -D_ALL_SOURCE -wP -I. $(C_SOURCES)

clean:
	-@rm -f *.o core *.1
