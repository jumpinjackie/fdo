/*
 * Copyright (C) 2004-2006  Autodesk, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of version 2.1 of the GNU Lesser
 * General Public License as published by the Free Software Foundation.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpGeoRasterExtractor.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-02-01 14:42:04 -0500 (Wed, 01 Feb 2006) $
 * $Revision: 26 $
 *
 */
#include "FDORFP.h"
#include "FdoRfpGeoRasterExtractor.h"
#include "FdoRfpGeoRaster.h"
#include "FdoRfpGeoBandRasterImp.h"
#include "FdoRfpGeoRaster.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include "FdoRfpConnection.h"
#include "FdoCommonStringUtil.h"
#include "FdoRfpRasterUtil.h"
#include "FdoRfpGeoreference.h"
#include "FdoRfpSpatialContext.h"
#include "FdoCommonMiscUtil.h"
#include "FdoRfpDatasetCache.h"
#include <gdal.h>

#ifdef _WIN32
#include <io.h>
void FdoRfpGeoRasterExtractor::_getAllFiles(const char* path, std::vector<std::string>& files)
{
    std::string path1 = path;
    path1 += "\\*";
    struct _finddata_t c_file;
    intptr_t hFile;
    /* Find first .c file in current directory */
    if( (hFile = _findfirst( path1.c_str(), &c_file )) != -1L )
    {
        do 
        {
            if (c_file.size == 0)
                continue;
            files.push_back(c_file.name);
        }
        while( _findnext( hFile, &c_file ) == 0 );
        _findclose( hFile );
    }
}
#else // for linux
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <dirent.h>
void FdoRfpGeoRasterExtractor::_getAllFiles(const char* path, std::vector<std::string>& files)
{
	struct dirent* dptr;
	DIR* dirp;
	if ((dirp = opendir(path)) != NULL)
	{
		while (dptr = readdir(dirp))
		{
			//std::string path1 = path;
			//path1 += "/";
			//path1 += dptr->d_name;
			//struct stat s;
			//if (0 == stat(path1.c_str(), &s) && S_ISREG(s))
			files.push_back(dptr->d_name);
		}
		closedir(dirp);
	}

}
#endif

// extract all the georeferenced rasters from the specified location
void FdoRfpGeoRasterExtractor::ExtractRasters(FdoRfpConnection* conn,
                                              FdoString* location,
                                              const FdoPtr<FdoRfpGeoRasterCollection>& rasters, 
                                              FdoStringP& currentCoord, 
                                              FdoRfpRect& extent, 
                                              bool& bFirstRaster,
                                              FdoStringCollection* coordSystems)
{
    VALIDATE_ARGUMENT(location);
    VALIDATE_ARGUMENT(coordSystems);

    FdoBoolean bCoordinateSystemConflict = false;
    char* mbLocation = NULL;
    wide_to_multibyte(mbLocation, location);
    std::string location1 = mbLocation;
    std::vector<std::string> files;
    FdoPtr<FdoRfpDatasetCache>  datasetCache = conn->GetDatasetCache();

    // determine whethere location is a path 
    bool bPath = false;

#ifndef _WIN32
    struct stat my_stat;
	
    if (0 != stat (location1.c_str(), &my_stat))
        return;

    if ((0 == stat (location1.c_str(), &my_stat)) && S_ISDIR(my_stat.st_mode))
        bPath = true;

#else
    DWORD attributes = GetFileAttributes (location1.c_str());

    if ((INVALID_FILE_ATTRIBUTES == attributes))
        return;

    if ((INVALID_FILE_ATTRIBUTES != attributes) && (FILE_ATTRIBUTE_DIRECTORY & attributes))
        bPath = true;
		
#endif

    if (bPath)
    {
        _getAllFiles(location1.c_str(), files);
    }
    else
    {
        // extract the file name and path from location1
        // there are two kinds of delimiters, 
        // the last one appears would be selected to extract the file name.
        // e.g. location1="c:\\path1/test.bmp", index would be set at "/".
        int index = (int)location1.rfind("\\");
        int index2 = (int)location1.rfind("/");
        if (index2 > index)
            index = index2;

        if( index == -1 ) // no path, just filename.
        {
            std::string fileName = location1.substr(index + 1);
            files.push_back(fileName);

            //extract path from location1
            location1 = ".";
        }
        else // split path and filename.
        {
            std::string fileName = location1.substr(index + 1);
            files.push_back(fileName);

            //extract path from location1
            location1 = location1.substr(0, index);
        }

    }
	
    location1 += (const char*)(FdoStringP(FILE_PATH_DELIMITER));

    int width, height, frameNumber;
    int i, j;
    int count = (int)files.size();

    for (i = 0; i < count; i++)
    {
        std::string filePath = location1;
        filePath += files[i];

        FdoStringP fileName = filePath.c_str();

        GDALDatasetH hDS = datasetCache->LockDataset( fileName, true );
        if( hDS == NULL )
            continue;

        width = GDALGetRasterXSize( hDS );
        height = GDALGetRasterYSize( hDS );
        frameNumber = 1;

        FdoRfpGeoreferenceP geoRef = new FdoRfpGeoreference();

        // if there is no georeference associated, just ignore this image.
        if (FdoRfpRasterUtil::GetGeoReferenceInfo(hDS, geoRef) == false)
            continue;

        datasetCache->UnlockDataset( hDS );

        // coordinate system
        FdoStringP coord;
        if (geoRef->HasCoordSystem() && FdoRfpRasterUtil::IsCoordinateSystemValid(geoRef->GetCoorSystem())) // there is a coordinate system with the image
            coord = geoRef->GetCoorSystem();

        // Add the coordinate system names to the collection of Spatial Contexts to be returned to the caller
        // It is the callers responsibility to determine what constitutes an error if more than one spatial context
        // exists for a class definition
        if (coord != L"" && coordSystems->IndexOf(coord, false) == -1) {
            coordSystems->Add(coord);
        }

        FdoRasterType rasterType;
        if (frameNumber == 1)	// single file - single frame
            rasterType = RasterType_SFSB;
        else					// single file - multi frames
            rasterType = RasterType_SFMB;
		
        FdoRfpGeoRasterP geoRaster = FdoRfpGeoRaster::Create();
        for (j=0; j < frameNumber; j++)
        {
            FdoRfpGeoBandRasterP rasterFile = new FdoRfpGeoBandRasterRot(
                conn, fileName, j, 
                geoRef->GetXInsertion(), geoRef->GetYInsertion(),
                geoRef->GetXResolution(), geoRef->GetYResolution(),                                                  
                width, height,
                geoRef->GetXRotation(), geoRef->GetYRotation());
            geoRaster->AddBand(rasterFile);

            // for the extent in spatial context, 
            // it's calculated by union of all of the bounds appearing in this feature class
            if (bFirstRaster)
            {
                extent = rasterFile->GetBounds();
                bFirstRaster = false;
            }
            else
                extent = extent.Union(rasterFile->GetBounds());
        }

        geoRaster->SetRasterType(rasterType);
        geoRaster->SetId(fileName);
        rasters->Add(geoRaster);
    }
}



