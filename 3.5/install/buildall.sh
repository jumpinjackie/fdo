#!/bin/bash
REVISION=`svn info .. | perl revnum.pl`
FDOBUILD=3.5.0
FDOINST=/usr/local/fdo-${FDOBUILD}

verifylib()
{
  libname=$1-${FDOBUILD}.so
  libpath=${FDOINST}/lib/${libname}
  if [ ! -e ${libpath} ]; then
    echo "Error Building ${libname}"
    exit 1
  fi
}

./buildfdocore.sh
verifylib libFDO
verifylib libExpressionEngine

./buildfdoprovider.sh sdf
verifylib libSDFProvider

./buildfdoprovider.sh shp
verifylib libSHPProvider
verifylib libSHPOverrides

./buildfdoprovider.sh wfs
verifylib libWFSProvider

./buildfdoprovider.sh wms
verifylib libWMSProvider
verifylib libWMSOverrides

# ArcSDE provider currently disabled due to missing libraries
#./buildfdoprovider.sh arcsde
#verifylib libArcSDEProvider

./buildfdoprovider.sh rdbms
verifylib libFdoMySQL
verifylib libFdoPostgreSQL
verifylib libFdoODBC
verifylib libSchemaMgr_OV

./buildfdoprovider.sh gdal
verifylib libGRFPProvider
verifylib libGRFPOverrides

./buildfdoprovider.sh ogr
verifylib libOGRProvider

./buildfdoprovider.sh postgis
verifylib libPostGISProvider
verifylib libPostGISOverrides

./buildfdoprovider.sh kingoracle
verifylib libKingOracleProvider
verifylib libKingOracleOverrides

./buildfdoprovider.sh sqlite
verifylib libSQLiteProvider

# Remove .la files from lib directory
rm -f ${FDOINST}/lib/*.la

# Remove unneeded symbols from files in the lib directory
# and make them non-executable
for file in `find ${FDOINST}/lib/lib*.so* -type f -print`
do
  strip --strip-unneeded ${file}
  chmod a-x ${file}
done

# Create a binary tar ball for FDO
tar -zcf fdosdk-centos5-${FDOBUILD}.${REVISION}.tgz ${FDOINST}

