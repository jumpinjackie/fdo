AUTOMAKE_OPTIONS = foreign

SUBDIRS =

DIST_SUBDIRS = apache linux cppunit util \
               Sqlite3.1.5 \
               boost_1_32_0 libcurl openssl \
               GDAL1.3

xmllibdir = /usr/local/fdo-3.2.0/lib
xmllib_DATA = \
  apache/xml-xalan/c/lib/libxalan-c.so.17.0  \
  apache/xml-xalan/c/lib/libxalanMsg.so.17.0 \
  apache/xml-xerces/c/lib/libxerces-c.so.25.0 \
  libcurl/lib/linux/libcurl.so.3.0.0  \
  openssl/lib/linux/libssl.so.0.9.7 \
  openssl/lib/linux/libcrypto.so.0.9.7 \
  GDAL1.3/linux/lib/optimized/libgdal.so.1.9.0

install-data-hook :
	@ln -s /usr/local/fdo-3.2.0/lib/libxalan-c.so.17.0 /usr/local/fdo-3.2.0/lib/libxalan-c.so.17
	@ln -s /usr/local/fdo-3.2.0/lib/libxalan-c.so.17.0 /usr/local/fdo-3.2.0/lib/libxalan-c.so
	@ln -s /usr/local/fdo-3.2.0/lib/libxalanMsg.so.17.0 /usr/local/fdo-3.2.0/lib/libxalanMsg.so.17
	@ln -s /usr/local/fdo-3.2.0/lib/libxalanMsg.so.17.0 /usr/local/fdo-3.2.0/lib/libxalanMsg.so
	@ln -s /usr/local/fdo-3.2.0/lib/libxerces-c.so.25.0 /usr/local/fdo-3.2.0/lib/libxerces-c.so.25
	@ln -s /usr/local/fdo-3.2.0/lib/libxerces-c.so.25.0 /usr/local/fdo-3.2.0/lib/libxerces-c.so
	@ln -s /usr/local/fdo-3.2.0/lib/libcurl.so.3.0.0 /usr/local/fdo-3.2.0/lib/libcurl.so.3
	@ln -s /usr/local/fdo-3.2.0/lib/libcurl.so.3.0.0 /usr/local/fdo-3.2.0/lib/libcurl.so
	@ln -s /usr/local/fdo-3.2.0/lib/libssl.so.0.9.7 /usr/local/fdo-3.2.0/lib/libssl.so.0
	@ln -s /usr/local/fdo-3.2.0/lib/libssl.so.0.9.7 /usr/local/fdo-3.2.0/lib/libssl.so
	@ln -s /usr/local/fdo-3.2.0/lib/libcrypto.so.0.9.7 /usr/local/fdo-3.2.0/lib/libcrypto.so.0
	@ln -s /usr/local/fdo-3.2.0/lib/libcrypto.so.0.9.7 /usr/local/fdo-3.2.0/lib/libcrypto.so
	@ln -s /usr/local/fdo-3.2.0/lib/libgdal.so.1.9.0 /usr/local/fdo-3.2.0/lib/libgdal.so.1
	@ln -s /usr/local/fdo-3.2.0/lib/libgdal.so.1.9.0 /usr/local/fdo-3.2.0/lib/libgdal.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libxalan-c.so.17
	@chmod a+x /usr/local/fdo-3.2.0/lib/libxalan-c.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libxalanMsg.so.17
	@chmod a+x /usr/local/fdo-3.2.0/lib/libxalanMsg.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libxerces-c.so.25
	@chmod a+x /usr/local/fdo-3.2.0/lib/libxerces-c.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libcurl.so.3
	@chmod a+x /usr/local/fdo-3.2.0/lib/libcurl.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libssl.so.0
	@chmod a+x /usr/local/fdo-3.2.0/lib/libssl.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libcrypto.so.0
	@chmod a+x /usr/local/fdo-3.2.0/lib/libcrypto.so
	@chmod a+x /usr/local/fdo-3.2.0/lib/libgdal.so.1
	@chmod a+x /usr/local/fdo-3.2.0/lib/libgdal.so

