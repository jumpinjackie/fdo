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
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpRasterPropertyDictionary.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpRasterPropertyDictionary.h"
#include "FdoRfpBandRaster.h"
#include "FdoRfpGlobals.h"
#include "FdoRfpImage.h"
#include "FdoRfpDatasetCache.h"
#include <gdal.h>

FdoRfpRasterPropertyDictionay::FdoRfpRasterPropertyDictionay(FdoRfpBandRaster* bandRaster) :
        m_raster(bandRaster)
{
    FDO_SAFE_ADDREF(bandRaster);
}

FdoRfpRasterPropertyDictionay::~FdoRfpRasterPropertyDictionay()
{
}

/// <summary>Gets the names of all the properties that apply to this
/// image as a collection of Strings. The order of the property names
/// in the collection dictate the order in which they need to
/// be specified. This is especially important for the success of
/// the EnumeratePropertyValues method as properties that occur
/// earlier in the collection may be required for successful enumeration
/// of properties that appear later in the collection.</summary>
/// <returns>A collection of the names of properties in this dictionary.</returns>
FdoStringCollection* FdoRfpRasterPropertyDictionay::GetPropertyNames ()
{
    // now we have only two properties, one is "Palette" whichi is the palette data, 
    // the other is "NumOfPaletteEntries" which indicates the number of entries in palette data
    FdoPtr<FdoStringCollection> propertyNames = FdoStringCollection::Create();
    FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();

    if (dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
    {
        propertyNames->Add(FdoGrfpGlobals::PaletteAttrName);
        propertyNames->Add(FdoGrfpGlobals::NumOfPaletteEntriesAttrName);
    }

    return FDO_SAFE_ADDREF(propertyNames.p);
}

/// <summary>Gets the data type for the specified property.</summary>
/// <param name="name">The name of the property to get the data type of.</param>
/// <returns>The data type of the property.</returns>
FdoDataType FdoRfpRasterPropertyDictionay::GetPropertyDataType (FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));
    FdoDataType rv;
    FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();

    if (STRCASEEQ(name, FdoGrfpGlobals::PaletteAttrName) 
        && dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
        rv = FdoDataType_BLOB;
    else if (STRCASEEQ(name, FdoGrfpGlobals::NumOfPaletteEntriesAttrName) 
             && dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
        rv = FdoDataType_Int32;
    else
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_79_RASTER_PROPERTY_NOT_EXIST, 
                                                     "Requested raster property '%1$ls' does not exist.", name));

    return rv;
}

/// <summary>Gets the value of the specified property (or its default).</summary>
/// <param name="name">The name of the property to get the value of.</param>
/// <returns>The value of the property.</returns>
FdoDataValue* FdoRfpRasterPropertyDictionay::GetProperty (FdoString* name)
{
    if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

    FdoDataValue* rv;
    FdoPtr<FdoRfpImage> image = m_raster->GetImage();
    FdoGdalMutexHolder oHolder;
    GDALColorTableH hCT = GDALGetRasterColorTable( GDALGetRasterBand( image->GetDS(), image->m_bandList[0] ) );
    int numOfEntries = 0;


    if (STRCASEEQ(name, FdoGrfpGlobals::PaletteAttrName) && hCT != NULL )
    {
        int iEntry, numOfEntries = GDALGetColorEntryCount(hCT);
        FdoByte *palette = new FdoByte[4 * numOfEntries];
        
        for( iEntry = 0; iEntry < numOfEntries; iEntry++ )
        {
            GDALColorEntry sEntry;

            GDALGetColorEntryAsRGB( hCT, iEntry, &sEntry );
            palette[iEntry*4 + 0] = sEntry.c1;
            palette[iEntry*4 + 1] = sEntry.c2;
            palette[iEntry*4 + 2] = sEntry.c3;
            palette[iEntry*4 + 3] = sEntry.c4;
        }
        rv = FdoDataValue::Create((FdoByte*)palette, 4 * numOfEntries, FdoDataType_BLOB);
        delete[] palette;
    }
    else if (STRCASEEQ(name, FdoGrfpGlobals::NumOfPaletteEntriesAttrName) && hCT != NULL )
    {
        rv = FdoDataValue::Create(GDALGetColorEntryCount(hCT));
    }
    else
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_79_RASTER_PROPERTY_NOT_EXIST, "Requested raster property '%1$ls' does not exist.", name));

    return rv;

}

/// <summary>Sets the value of the specified property.</summary>
/// <param name="name">The name of the property to set the value of.</param>
/// <param name="value">The new value for the property.</param>
void FdoRfpRasterPropertyDictionay::SetProperty (FdoString* name, FdoDataValue* value)
{
    throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", "IRasterPropertyDictionary::SetProperty"));
}

/// <summary>Gets the default value for the specified property.</summary>
/// <param name="name">The name of the property to get the default value of.</param>
/// <returns>The default value of the property.</returns>
FdoDataValue* FdoRfpRasterPropertyDictionay::GetPropertyDefault (FdoString* name)
{
    return NULL;
}

/// <summary>Predicate to determine if the property is required to be set.</summary>
/// <param name="name">The name of the property to get the required status of.</param>
/// <returns>Returns true if the specified property is required, false if it is optional.</returns>
bool FdoRfpRasterPropertyDictionay::IsPropertyRequired (FdoString* name)
{
    return false;
}

/// <summary>Predicate to determine if the property is enumerable (has more than one value).</summary>
/// <param name="name">The name of the property to get the enumerable status of.</param>
/// <returns>Returns true if the possible values for the specified property
/// can be enumerated via the GetPropertyValues method.</returns>
bool FdoRfpRasterPropertyDictionay::IsPropertyEnumerable (FdoString* name)
{
    return false;
}

/// <summary>Access the values of the specified enumerable property.
/// The property must respond TRUE to IsPropertyEnumerable.</summary>
/// <param name="name">The name of the property to get the collection of values for.</param>
/// <returns>A collection of datavaluesfor the enumerable property.</returns>
FdoDataValueCollection* FdoRfpRasterPropertyDictionay::GetPropertyValues (FdoString* name)
{
    return NULL;
}

/// <summary>Set the values of the specified enumerable property.
/// The property must respond TRUE to IsPropertyEnumerable.
/// Clients will need to use SetPropertyValues when creating an image
/// with specific values for the enumerable property.
/// An example would be setting a table of wavelength ranges corresponding
/// to the false colour used to encode the image.</summary>
/// <param name="name">The name of the property to set the collection of values for.</param>
/// <param name="collection">The collection of values for the enumerable property.</param>
void FdoRfpRasterPropertyDictionay::SetPropertyValues (FdoString* name, FdoDataValueCollection* collection)
{
}
