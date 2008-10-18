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
#include "FdoWmsUtils.h"
#include "FdoWmsRasterGdal.h"
#include "FdoWmsFeatureReader.h"
#include "FdoWmsGlobals.h"

#include <malloc.h>
#include <string.h>

FdoWmsFeatureReader::FdoWmsFeatureReader ()
	: mFeatureIdx (-1), mBounds(0, 0, 0, 0)
{
}

FdoWmsFeatureReader::FdoWmsFeatureReader (FdoIoStream* stream, const FdoWmsRect& bounds, FdoClassDefinition* featClass, FdoClassDefinition* classDef)
	: mStream (FDO_SAFE_ADDREF (stream)), mFeatureIdx (-1), mBounds(bounds)
{
	mFeatClass = FDO_SAFE_ADDREF (featClass);
	mClassDefPruned = FDO_SAFE_ADDREF (classDef);
}

FdoWmsFeatureReader::~FdoWmsFeatureReader (void)
{
}

/** Do not implement the copy constructor. **/
//FdoWmsFeatureReader::FdoWmsFeatureReader (const FdoWmsFeatureReader &right) { }

void FdoWmsFeatureReader::Dispose ()
{
    delete this;
}

/// <summary>Gets the definition of the object currently being read. If the user
/// has requested only a subset of the class properties, the class 
/// definition reflects what the user has asked, rather than the full class 
/// definition.</summary>
/// <returns>Returns the class definition object.</returns> 
FdoClassDefinition* FdoWmsFeatureReader::GetClassDefinition ()
{
	return FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(mClassDefPruned);
}

/// <summary>Gets a value indicating the depth of nesting for the current reader.
/// The depth value increases each time GetFeatureObject is called and a new 
/// reader is returned. The outermost reader has a depth of 0.</summary>
/// <returns>Returns the depth</returns> 
FdoInt32 FdoWmsFeatureReader::GetDepth ()
{
    return (0);
}

/// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
/// the object or object collection property. If the property is not an
/// object property, an exception is thrown.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the nested feature reader</returns> 
FdoIFeatureReader* FdoWmsFeatureReader::GetFeatureObject (FdoString* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGet(FDOWMS_OBJECT_PROPERTIES_NOT_SUPPORTED, "Object properties are not supported."));
}

FdoString* FdoWmsFeatureReader::GetString (FdoString* identifier)
{
	// Verify the property
	FdoPtr<FdoPropertyDefinitionCollection> props;
	props = mClassDefPruned->GetProperties ();
	if (props->GetCount () == 0)
	{
		props = mFeatClass->GetProperties ();
		if (props->GetCount () ==0 )
		{
			props = FdoPropertyDefinitionCollection::Create (NULL);
			FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = mFeatClass->GetBaseProperties ();
			for (FdoInt32 i=0; i<baseProps->GetCount(); i++)
				props->Add (baseProps->GetItem (i));
		}
	}

	if (props->IndexOf (identifier) == -1)
		throw FdoCommandException::Create (NlsMsgGet (FDOWMS_COMMAND_INVALID_PROPERTY_NAME, 
				"Invalid property name '%1$ls'.", 
				identifier));

	FdoPtr<FdoPropertyDefinition> prop = props->GetItem (identifier);
	FdoDataPropertyDefinition* dataProp = static_cast<FdoDataPropertyDefinition *> (prop.p);
	if (dataProp == NULL || dataProp->GetDataType () != FdoDataType_String)
	{
		throw FdoCommandException::Create (NlsMsgGet (FDOWMS_COMMAND_INCORRECT_PROPERTY_DATA_TYPE,
				"Property '%1$ls' is not the proper data type '%2$ls'.",
				identifier,
				L"FdoString*"));
	}

	// There is only one feature to be read
	if (mFeatureIdx != 0)
		return NULL;
		
	return mFeatClass->GetName ();
}

FdoIRaster* FdoWmsFeatureReader::GetRaster(FdoString* identifier)
{
	// Verify the property
	FdoPtr<FdoPropertyDefinitionCollection> props;
	props = mClassDefPruned->GetProperties ();
	if (props->GetCount () == 0)
	{
		props = mFeatClass->GetProperties ();
		if (props->GetCount () ==0 )
		{
			props = FdoPropertyDefinitionCollection::Create (NULL);
			FdoPtr<FdoReadOnlyPropertyDefinitionCollection> baseProps = mFeatClass->GetBaseProperties ();
			for (FdoInt32 i=0; i<baseProps->GetCount(); i++)
				props->Add (baseProps->GetItem (i));
		}
	}

	if (props->IndexOf (identifier) == -1)
		throw FdoCommandException::Create (NlsMsgGet (FDOWMS_COMMAND_INVALID_PROPERTY_NAME, 
				"Invalid property name '%1$ls'.", 
				identifier));

	FdoPtr<FdoPropertyDefinition> prop = props->GetItem (identifier);
	if (prop == NULL || prop->GetPropertyType () != FdoPropertyType_RasterProperty)
	{
		throw FdoCommandException::Create (NlsMsgGet (FDOWMS_COMMAND_INCORRECT_PROPERTY_DATA_TYPE,
				"Property '%1$ls' is not the proper data type '%2$ls'.",
				identifier,
				L"FdoString*"));
	}

	// There is only one feature to be read
	if (mFeatureIdx != 0)
		return NULL;

	if (mStream == NULL)
		throw FdoCommandException::Create (NlsMsgGet(FDOWMS_NULL_RASTER_IO_STREAM, "NULL Raster I/O stream."));


	return new FdoWmsRasterGdal(mStream, &mBounds);
}

bool FdoWmsFeatureReader::ReadNext ()
{
	mFeatureIdx++;
	if (mFeatureIdx == 0)
		return true;
	else
		return false;
}
