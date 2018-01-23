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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpRasterUtil.h $
 *   $Author: fwarmerdam $
 *     $Date: 2006-02-01 14:42:04 -0500 (Wed, 01 Feb 2006) $
 * $Revision: 26 $
 *
 */
#ifndef FDORFPRASTERUTIL_H
#define FDORFPRASTERUTIL_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <FdoRfpDefines.h>

//
// forward declarations
//
class FdoRfpGeoreference;
class FdoRfpSpatialContext;
class FdoRfpKeyColorCollection;
typedef void *GDALDatasetH;

// raster util class
class FdoRfpRasterUtil
{
public:
    
    // Get the geo reference infomation associated with the raster image
    static bool GetGeoReferenceInfo(GDALDatasetH hDS, FdoPtr<FdoRfpGeoreference>& geoRef);

    // Get the key color list from a image file
    static bool GetKeyColor(FdoStringP filePath, FdoPtr<FdoRfpKeyColorCollection>& keyColorColl);

    // Determines whether two spatial contexts are equal
    static bool IsContextEqual(FdoPtr<FdoRfpSpatialContext>& context1, FdoPtr<FdoRfpSpatialContext>& context2);

    // Determines whether a coordinate system name is valid
    static bool IsCoordinateSystemValid(FdoString* coordSysName);
};

#endif

