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

#include <malloc.h>
#include <string.h>
#include "FdoCommonSchemaUtil.h"

FdoWfsFeatureReader::FdoWfsFeatureReader ()
{
}

FdoWfsFeatureReader::~FdoWfsFeatureReader (void)
{
}

/** Do not implement the copy constructor. **/
//FdoWfsFeatureReader::FdoWfsFeatureReader (const FdoWfsFeatureReader &right) { }

void FdoWfsFeatureReader::Dispose ()
{
    delete this;
}

/// <summary>Gets the definition of the object currently being read. If the user
/// has requested only a subset of the class properties, the class 
/// definition reflects what the user has asked, rather than the full class 
/// definition.</summary>
/// <returns>Returns the class definition object.</returns> 
FdoClassDefinition* FdoWfsFeatureReader::GetClassDefinition ()
{	
	return FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(m_classDefPruned);	
}

/// <summary>Gets a value indicating the depth of nesting for the current reader.
/// The depth value increases each time GetFeatureObject is called and a new 
/// reader is returned. The outermost reader has a depth of 0.</summary>
/// <returns>Returns the depth</returns> 
FdoInt32 FdoWfsFeatureReader::GetDepth ()
{
    return this->m_featureReader->GetDepth();
}

FdoStringP FdoWfsFeatureReader::_decodeName(FdoString* name)
{
	FdoStringP propName = name;
	if (propName.Contains(FdoWfsGlobals::Dot))
		return propName.Replace(FdoWfsGlobals::Dot, L".");
	return propName;

}


/// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
/// the object or object collection property. If the property is not an
/// object property, an exception is thrown.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the nested feature reader</returns> 
FdoIFeatureReader* FdoWfsFeatureReader::GetFeatureObject (FdoString* propertyName)
{

	return this->m_featureReader->GetFeatureObject(_decodeName(propertyName));
}

void FdoWfsFeatureReader::SetClassDefinition (FdoClassDefinition* originClassDef, FdoIdentifierCollection* propsToSelect)
{
    FdoPtr<FdoCommonSchemaCopyContext> copyContext;

    //if identifier collection is empty or NULL, all properties are returned
    if (propsToSelect != NULL && propsToSelect->GetCount() > 0)
        copyContext = FdoCommonSchemaCopyContext::Create(propsToSelect);
    
    // Clone the given class definition:
    m_classDefPruned = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(originClassDef, copyContext);
}

const FdoByte * FdoWfsFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32 * count)
{
	return this->m_featureReader->GetGeometry(_decodeName(propertyName), count);
}

FdoByteArray* FdoWfsFeatureReader::GetGeometry(FdoString* propertyName)
{
	return this->m_featureReader->GetGeometry(_decodeName(propertyName));
}

bool FdoWfsFeatureReader::GetBoolean(FdoString* propertyName)
{
	return this->m_featureReader->GetBoolean(_decodeName(propertyName));
}

FdoByte FdoWfsFeatureReader::GetByte(FdoString* propertyName)
{
	return this->m_featureReader->GetByte(_decodeName(propertyName));
}

FdoDateTime FdoWfsFeatureReader::GetDateTime(FdoString* propertyName)
{
	return this->m_featureReader->GetDateTime(_decodeName(propertyName));
}

double FdoWfsFeatureReader::GetDouble(FdoString* propertyName)
{
	return this->m_featureReader->GetDouble(_decodeName(propertyName));
}

FdoInt16 FdoWfsFeatureReader::GetInt16(FdoString* propertyName)
{
	return this->m_featureReader->GetInt16(_decodeName(propertyName));
}

FdoInt32 FdoWfsFeatureReader::GetInt32(FdoString* propertyName)
{
	return this->m_featureReader->GetInt32(_decodeName(propertyName));
}

FdoInt64 FdoWfsFeatureReader::GetInt64(FdoString* propertyName)
{
	return this->m_featureReader->GetInt64(_decodeName(propertyName));
}

float FdoWfsFeatureReader::GetSingle(FdoString* propertyName)
{
	return this->m_featureReader->GetSingle(_decodeName(propertyName));
}

FdoString* FdoWfsFeatureReader::GetString(FdoString* propertyName)
{
	return this->m_featureReader->GetString(_decodeName(propertyName));
}

FdoLOBValue* FdoWfsFeatureReader::GetLOB(FdoString* propertyName)
{
	return this->m_featureReader->GetLOB(_decodeName(propertyName));
}

FdoIStreamReader* FdoWfsFeatureReader::GetLOBStreamReader(const wchar_t* propertyName )
{
	return this->m_featureReader->GetLOBStreamReader(_decodeName(propertyName));
}

bool FdoWfsFeatureReader::IsNull(FdoString* propertyName)
{
	return this->m_featureReader->IsNull(_decodeName(propertyName));
}

FdoIRaster* FdoWfsFeatureReader::GetRaster(FdoString* propertyName)
{
	return this->m_featureReader->GetRaster(_decodeName(propertyName));
}

bool FdoWfsFeatureReader::ReadNext()
{
    return m_featureReader->ReadNext();
}

void FdoWfsFeatureReader::Close()
{
	return this->m_featureReader->Close();
}
