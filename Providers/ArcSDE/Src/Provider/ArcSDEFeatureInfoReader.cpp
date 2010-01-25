/*
 * 
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

ArcSDEFeatureInfoReader::ArcSDEFeatureInfoReader (FdoPropertyValueCollection *info, FdoClassDefinition *definition) :
    mFeatInfoCollection (info),
    m_iRowIndex(-1),
    mClosed(false)
{
    FDO_SAFE_ADDREF (info);

    // Clone & prune the given class definition:
    FdoPtr<FdoIdentifierCollection> idCollection = FdoIdentifierCollection::Create();
    for (int i=0; i<mFeatInfoCollection->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue> propVal = mFeatInfoCollection->GetItem(i);
        FdoPtr<FdoIdentifier> id = propVal->GetName();
        if (!idCollection->Contains(id))
            idCollection->Add(id);
    }
    mClassDefinition = ArcSDESchemaManager::CloneAndPruneClass(definition, idCollection);
}

ArcSDEFeatureInfoReader::~ArcSDEFeatureInfoReader ()
{
}

FdoClassDefinition *ArcSDEFeatureInfoReader::GetClassDefinition ()
{
    // Return a copy of the pruned class (so callers don't mess up our internal copy):
    return (FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(mClassDefinition));
}

int ArcSDEFeatureInfoReader::GetDepth()
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}

FdoLOBValue* ArcSDEFeatureInfoReader::GetLOB(FdoString* propertyName)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}

FdoIStreamReader* ArcSDEFeatureInfoReader::GetLOBStreamReader(const wchar_t* propertyName )
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}

FdoIFeatureReader* ArcSDEFeatureInfoReader::GetFeatureObject (const wchar_t* name)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OBJECT_PROPERTIES_NOT_SUPPORTED, "Object properties are not supported."));
}

bool ArcSDEFeatureInfoReader::GetBoolean (const wchar_t* name)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_BOOLEAN, "The Boolean data type is not supported by ArcSDE."));
}

FdoByte ArcSDEFeatureInfoReader::GetByte (const wchar_t* name)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_BYTE, "The Byte data type is not supported by ArcSDE."));
}


FdoDateTime ArcSDEFeatureInfoReader::GetDateTime (const wchar_t* name)
{
    FdoPtr<FdoDateTimeValue> value = GetValue<FdoDateTimeValue>(name, L"FdoDateTime");
    return (value->GetDateTime());
}


double ArcSDEFeatureInfoReader::GetDouble (const wchar_t* name)
{
    FdoPtr<FdoDoubleValue> value = GetValue<FdoDoubleValue>(name, L"FdoDouble");
    return (value->GetDouble());
}

const wchar_t* ArcSDEFeatureInfoReader::GetString (const wchar_t* name)
{
    FdoPtr<FdoStringValue> value = GetValue<FdoStringValue>(name, L"FdoString");
    return (value->GetString());
}

FdoInt16 ArcSDEFeatureInfoReader::GetInt16 (const wchar_t* name)
{
    FdoPtr<FdoInt16Value> value = GetValue<FdoInt16Value>(name, L"FdoInt16");
    return (value->GetInt16());
}

FdoInt32 ArcSDEFeatureInfoReader::GetInt32 (const wchar_t* name)
{
    FdoPtr<FdoInt32Value> value = GetValue<FdoInt32Value>(name, L"FdoInt32");
    return (value->GetInt32 ());
}

FdoInt64 ArcSDEFeatureInfoReader::GetInt64 (const wchar_t* name)
{
    throw FdoException::Create(NlsMsgGet(ARCSDE_UNSUPPORTED_DATATYPE_INT64, "The Int64 data type is not supported by ArcSDE."));

}

float ArcSDEFeatureInfoReader::GetSingle (const wchar_t* name)
{
    FdoPtr<FdoSingleValue> value = GetValue<FdoSingleValue>(name, L"FdoSingle");
    return (value->GetSingle());
}

bool ArcSDEFeatureInfoReader::IsNull (const wchar_t* name)
{
    Validate();

    // make sure the given name is a valid property name:
    FdoPtr<FdoPropertyValue> propVal = mFeatInfoCollection->FindItem (name);
    if (propVal == NULL)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_PROPERTY_NOT_FOUND, "The property '%1$ls' was not found.", name));

    // Determine whether or not this value is null:
    FdoPtr<FdoValueExpression> valueExpr = propVal->GetValue();
    FdoDataValue *dataValue = static_cast<FdoDataValue*>(valueExpr.p);
    if (NULL != dataValue)
        return dataValue->IsNull();
    else
        return false;
}

FdoByteArray* ArcSDEFeatureInfoReader::GetGeometry (const wchar_t* name)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}

const FdoByte * ArcSDEFeatureInfoReader::GetGeometry(FdoString* propertyName, FdoInt32 * count)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}

FdoIRaster* ArcSDEFeatureInfoReader::GetRaster (const wchar_t* name)
{
    throw FdoException::Create (NlsMsgGet(ARCSDE_OPERATION_UNSUPPORTED, "This operation is not supported."));
}

bool ArcSDEFeatureInfoReader::ReadNext ()
{
    if (mClosed)
        throw FdoException::Create(NlsMsgGet(ARCSDE_READER_CLOSED, "Reader is closed."));

    m_iRowIndex++;

    // only one row can be returned
    return ((mFeatInfoCollection != NULL) && m_iRowIndex == 0);
}

void ArcSDEFeatureInfoReader::Close ()
{
    mClosed = true;
}

void ArcSDEFeatureInfoReader::Dispose ()
{
    delete this;
}


void ArcSDEFeatureInfoReader::Validate()
{
    if (m_iRowIndex == -1)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_READER_NOT_READY, "Must %1$ls prior to accessing reader.", L"ReadNext"));

    if ((mFeatInfoCollection==NULL) || (m_iRowIndex != 0))
        throw FdoException::Create(NlsMsgGet(ARCSDE_READER_EXHAUSTED, "Reader is exhausted."));

    if (mClosed)
        throw FdoException::Create(NlsMsgGet(ARCSDE_READER_CLOSED, "Reader is closed."));
}


template <typename T>
T* ArcSDEFeatureInfoReader::GetValue(FdoString* name, FdoString* typeName)
{
    Validate();

    FdoPtr<FdoPropertyValue> propVal = mFeatInfoCollection->FindItem (name);
    if (propVal == NULL)
        throw FdoException::Create (NlsMsgGet1(ARCSDE_PROPERTY_NOT_FOUND, "The property '%1$ls' was not found.", name));

    FdoPtr<FdoValueExpression> valueExpr = propVal->GetValue ();
    T* value = static_cast<T*>(valueExpr.p);
    if (value == NULL)
        throw FdoException::Create (NlsMsgGet2(ARCSDE_PROPERTY_UNEXPECTED_TYPE, "The property '%1$ls' was not of the expect type '%2$ls'.", name, typeName));

    return FDO_SAFE_ADDREF(value);
}

