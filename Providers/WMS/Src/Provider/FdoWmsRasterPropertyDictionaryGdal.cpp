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
 */

#include "stdafx.h"
#include <FdoWmsRasterPropertyDictionaryGdal.h>
#include <FdoWmsBandRasterGdal.h>
#include <FdoWmsGlobals.h>
#include <FdoWmsImage.h>

FdoWmsRasterPropertyDictionayGdal::FdoWmsRasterPropertyDictionayGdal(FdoWmsBandRasterGdal* bandRaster) : m_raster(bandRaster)
{
	FDO_SAFE_ADDREF(bandRaster);
}

FdoWmsRasterPropertyDictionayGdal::~FdoWmsRasterPropertyDictionayGdal()
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
FdoStringCollection* FdoWmsRasterPropertyDictionayGdal::GetPropertyNames ()
{
	// now we have only two properties, one is "Palette" whichi is the palette data, 
	// the other is "NumOfPaletteEntries" which indicates the number of entries in palette data
	FdoPtr<FdoStringCollection> propertyNames = FdoStringCollection::Create();

	propertyNames->Add(FdoWmsGlobals::PaletteAttrName);
	propertyNames->Add(FdoWmsGlobals::NumOfPaletteEntriesAttrName);

	return FDO_SAFE_ADDREF(propertyNames.p);
}

/// <summary>Gets the data type for the specified property.</summary>
/// <param name="name">The name of the property to get the data type of.</param>
/// <returns>The data type of the property.</returns>
FdoDataType FdoWmsRasterPropertyDictionayGdal::GetPropertyDataType (FdoString* name)
{
	if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	FdoDataType rv;
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();

	if (STRCASEEQ(name, FdoWmsGlobals::PaletteAttrName) && dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
		rv = FdoDataType_BLOB;
	else if (STRCASEEQ(name, FdoWmsGlobals::NumOfPaletteEntriesAttrName) && dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
		rv = FdoDataType_Int32;
	else
	    throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11003_RASTER_PROPERTY_NOT_EXIST, "Requested raster property '%1$ls' does not exist.", name));

	return rv;
}

/// <summary>Gets the value of the specified property (or its default).</summary>
/// <param name="name">The name of the property to get the value of.</param>
/// <returns>The value of the property.</returns>
FdoDataValue* FdoWmsRasterPropertyDictionayGdal::GetProperty (FdoString* name)
{
	if (name == NULL)
        throw FdoException::Create(FdoException::NLSGetMessage(FDO_2_BADPARAMETER, "Bad parameter to method."));

	FdoDataValue* rv;
	FdoPtr<FdoRasterDataModel> dataModel = m_raster->GetDataModel();
	
	RgbColor*	palette   = m_raster->GetPalette();
	FdoInt32 numOfEntries = m_raster->GetNumberOfPaletteEntries();

	if (STRCASEEQ(name, FdoWmsGlobals::PaletteAttrName) && dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
		rv = FdoDataValue::Create((FdoByte*)palette, 4 * numOfEntries, FdoDataType_BLOB);
	else if (STRCASEEQ(name, FdoWmsGlobals::NumOfPaletteEntriesAttrName) && dataModel->GetDataModelType() == FdoRasterDataModelType_Palette)
		rv = FdoDataValue::Create(numOfEntries);
	else
	    throw FdoCommandException::Create(NlsMsgGet(FDOWMS_11003_RASTER_PROPERTY_NOT_EXIST, "Requested raster property '%1$ls' does not exist.", name));

	return rv;

}

/// <summary>Sets the value of the specified property.</summary>
/// <param name="name">The name of the property to set the value of.</param>
/// <param name="value">The new value for the property.</param>
void FdoWmsRasterPropertyDictionayGdal::SetProperty (FdoString* name, FdoDataValue* value)
{
	throw FdoCommandException::Create(NlsMsgGet(FDOWMS_OPERATION_NOT_SUPPORTED, "FDO Operation '%1$ls' is not supported.", "IRasterPropertyDictionary::SetProperty"));
}

/// <summary>Gets the default value for the specified property.</summary>
/// <param name="name">The name of the property to get the default value of.</param>
/// <returns>The default value of the property.</returns>
FdoDataValue* FdoWmsRasterPropertyDictionayGdal::GetPropertyDefault (FdoString* name)
{
	return NULL;
}

/// <summary>Predicate to determine if the property is required to be set.</summary>
/// <param name="name">The name of the property to get the required status of.</param>
/// <returns>Returns true if the specified property is required, false if it is optional.</returns>
bool FdoWmsRasterPropertyDictionayGdal::IsPropertyRequired (FdoString* name)
{
	return false;
}

/// <summary>Predicate to determine if the property is enumerable (has more than one value).</summary>
/// <param name="name">The name of the property to get the enumerable status of.</param>
/// <returns>Returns true if the possible values for the specified property
/// can be enumerated via the GetPropertyValues method.</returns>
bool FdoWmsRasterPropertyDictionayGdal::IsPropertyEnumerable (FdoString* name)
{
	return false;
}

/// <summary>Access the values of the specified enumerable property.
/// The property must respond TRUE to IsPropertyEnumerable.</summary>
/// <param name="name">The name of the property to get the collection of values for.</param>
/// <returns>A collection of datavaluesfor the enumerable property.</returns>
FdoDataValueCollection* FdoWmsRasterPropertyDictionayGdal::GetPropertyValues (FdoString* name)
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
void FdoWmsRasterPropertyDictionayGdal::SetPropertyValues (FdoString* name, FdoDataValueCollection* collection)
{
	throw FdoCommandException::Create(NlsMsgGet(FDOWMS_OPERATION_NOT_SUPPORTED, "FDO Operation '%1$ls' is not supported.", "IRasterPropertyDictionary::SetPropertyValues"));
}
