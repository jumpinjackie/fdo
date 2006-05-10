#!/bin/bash

if [ $FDO_SVN_ROOT = "" ]
then
echo "FDO_SVN_ROOT must be set";
exit 1;
fi

if [ $FDO_SVN_USERNAME = "" ]
then
echo "FDO_SVN_USERNAME must be set";
exit 1;
fi

if [ $FDO_SVN_PASSWORD = "" ]
then
echo "FDO_SVN_PASSWORD must be set";
exit 1;
fi

mkdir -p $FDO_SVN_ROOT
mkdir -p $FDO_SVN_ROOT/Providers/SDF
mkdir -p $FDO_SVN_ROOT/Providers/SHP
mkdir -p $FDO_SVN_ROOT/Providers/WMS
mkdir -p $FDO_SVN_ROOT/Providers/WFS
mkdir -p $FDO_SVN_ROOT/Providers/ArcSDE
mkdir -p $FDO_SVN_ROOT/Providers/GenericRdbms

svn checkout https://fdocore.osgeo.org/svn/fdocore/trunk $FDO_SVN_ROOT --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD
svn checkout https://fdosdf.osgeo.org/svn/fdosdf/trunk/Providers/SDF $FDO_SVN_ROOT/Providers/SDF --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD
svn checkout https://fdoshp.osgeo.org/svn/fdoshp/trunk/Providers/SHP $FDO_SVN_ROOT/Providers/SHP --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD
svn checkout https://fdowms.osgeo.org/svn/fdowms/trunk/Providers/WMS $FDO_SVN_ROOT/Providers/WMS --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD
svn checkout https://fdowfs.osgeo.org/svn/fdowfs/trunk/Providers/WFS $FDO_SVN_ROOT/Providers/WFS --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD
svn checkout https://fdoarcsde.osgeo.org/svn/fdoarcsde/trunk/Providers/ArcSDE $FDO_SVN_ROOT/Providers/ArcSDE --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD
svn checkout https://fdordbms.osgeo.org/svn/fdordbms/trunk/Providers/GenericRdbms $FDO_SVN_ROOT/Providers/GenericRdbms --username $FDO_SVN_USERNAME --password $FDO_SVN_PASSWORD

exit 0