#!/bin/bash
./buildfdocore.sh
./buildfdoprovider.sh sdf
./buildfdoprovider.sh shp
./buildfdoprovider.sh wfs
./buildfdoprovider.sh wms
# ArcSDE provider currently disabled due to missing libraries
#./buildfdoprovider.sh arcsde
./buildfdoprovider.sh rdbms
./buildfdoprovider.sh gdal
./buildfdoprovider.sh ogr
./buildfdoprovider.sh postgis
./buildfdoprovider.sh kingoracle
./buildfdoprovider.sh sqlite
