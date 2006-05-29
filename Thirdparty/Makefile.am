AUTOMAKE_OPTIONS = foreign

SUBDIRS =

DIST_SUBDIRS = apache linux cppunit util \
               Sqlite3.1.5 \
               boost_1_32_0 libcurl openssl \
               GDAL1.3

thdlibdir = $(prefix)/lib
thdlib_DATA = \
  apache/xml-xalan/c/lib/libxalan-c.so.17.0  \
  apache/xml-xalan/c/lib/libxalanMsg.so.17.0 \
  apache/xml-xerces/c/lib/libxerces-c.so.25.0 \
  GDAL1.3/linux/lib/optimized/libgdal.so.1.9.0

install-data-hook :
	@ln -s -f $(thdlibdir)/libxalan-c.so.17.0 $(thdlibdir)/libxalan-c.so.17
	@ln -s -f $(thdlibdir)/libxalan-c.so.17.0 $(thdlibdir)/libxalan-c.so
	@ln -s -f $(thdlibdir)/libxalanMsg.so.17.0 $(thdlibdir)/libxalanMsg.so.17
	@ln -s -f $(thdlibdir)/libxalanMsg.so.17.0 $(thdlibdir)/libxalanMsg.so
	@ln -s -f $(thdlibdir)/libxerces-c.so.25.0 $(thdlibdir)/libxerces-c.so.25
	@ln -s -f $(thdlibdir)/libxerces-c.so.25.0 $(thdlibdir)/libxerces-c.so
	@ln -s -f $(thdlibdir)/libgdal.so.1.9.0 $(thdlibdir)/libgdal.so.1
	@ln -s -f $(thdlibdir)/libgdal.so.1.9.0 $(thdlibdir)/libgdal.so
	@chmod a+x $(thdlibdir)/libxalan-c.so.17
	@chmod a+x $(thdlibdir)/libxalan-c.so
	@chmod a+x $(thdlibdir)/libxalanMsg.so.17
	@chmod a+x $(thdlibdir)/libxalanMsg.so
	@chmod a+x $(thdlibdir)/libxerces-c.so.25
	@chmod a+x $(thdlibdir)/libxerces-c.so
	@chmod a+x $(thdlibdir)/libgdal.so.1
	@chmod a+x $(thdlibdir)/libgdal.so

uninstall :
	@rm -f $(thdlibdir)/libxalan-c.so*
	@rm -f $(thdlibdir)/libxalanMsg.so*
	@rm -f $(thdlibdir)/libxerces-c.so*
	@rm -f $(thdlibdir)/libgdal.so*

