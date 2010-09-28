//
// Copyright (C) 2008 Mateusz Loskot <mateusz@loskot.net>
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
#include "stdafx.h"
#include "PostGisProvider.h"
#include "InsertFeatureReader.h"

namespace fdo { namespace postgis {

InsertFeatureReader::InsertFeatureReader(FdoPropertyValueCollection* props,
                                         FdoClassDefinition* classDef)
                                         : mProps(props), mClassDef(classDef), mIsFirst(true)
{
    FDOLOG_MARKER("InsertFeatureReader::InsertFeatureReader");

    FDO_SAFE_ADDREF(mProps.p);
    FDO_SAFE_ADDREF(mClassDef.p);
}

InsertFeatureReader::~InsertFeatureReader()
{
}

///////////////////////////////////////////////////////////////////////////////
// FdoIDisposable interface
///////////////////////////////////////////////////////////////////////////////

void InsertFeatureReader::Dispose()
{
    delete this;
}

///////////////////////////////////////////////////////////////////////////////
// FdoIReader  interface
///////////////////////////////////////////////////////////////////////////////

bool InsertFeatureReader::GetBoolean(FdoString* propertyName)
{
    return (GetInt32(propertyName) != 0);
}

FdoByte InsertFeatureReader::GetByte(FdoString* propertyName)
{
    const FdoInt32 tmp = GetInt32(propertyName);
    return static_cast<FdoByte>(tmp);
}

FdoDateTime InsertFeatureReader::GetDateTime(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_DateTime == dataVal->GetDataType())
    {
        FdoDateTimeValue* val = static_cast<FdoDateTimeValue*>(dataVal.p);
        return val->GetDateTime();
    }

    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetDateTime - invalid type");
}

double InsertFeatureReader::GetDouble(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_Double == dataVal->GetDataType())
    {
        FdoDoubleValue* val = static_cast<FdoDoubleValue*>(dataVal.p);
        return val->GetDouble();
    }

    throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader::GetDouble - invalid type");
}

FdoInt16 InsertFeatureReader::GetInt16(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_Int16 == dataVal->GetDataType())
    {
        FdoInt16Value* val = static_cast<FdoInt16Value*>(dataVal.p);
        return val->GetInt16();
    }

    throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader::GetInt16 - invalid type");
}

FdoInt32 InsertFeatureReader::GetInt32(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_Int32 == dataVal->GetDataType())
    {
        FdoInt32Value* val = static_cast<FdoInt32Value*>(dataVal.p);
        return val->GetInt32();
    }

    throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader::GetInt32 - invalid type");
}

FdoInt64 InsertFeatureReader::GetInt64(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_Int64 == dataVal->GetDataType())
    {
        FdoInt64Value* val = static_cast<FdoInt64Value*>(dataVal.p);
        return val->GetInt64();
    }

    throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader::GetInt64 - invalid type");
}

float InsertFeatureReader::GetSingle(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_Single == dataVal->GetDataType())
    {
        FdoSingleValue* val = static_cast<FdoSingleValue*>(dataVal.p);
        return val->GetSingle();
    }

    throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader::GetSingle - invalid type");
}

FdoString* InsertFeatureReader::GetString(FdoString* propertyName)
{
    FdoPtr<FdoDataValue> dataVal(GetDataValue(propertyName));
    if (FdoDataType_String == dataVal->GetDataType())
    {
        FdoStringValue* val = static_cast<FdoStringValue*>(dataVal.p);
        return val->GetString();
    }

    throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader::GetSingle - invalid type");
}

FdoLOBValue* InsertFeatureReader::GetLOB(FdoString* propertyName)
{
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetLOB unsupported");
}

FdoIStreamReader* InsertFeatureReader::GetLOBStreamReader(FdoString* propertyName)
{
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetLOBStreamReader unsupported");
}

FdoIRaster* InsertFeatureReader::GetRaster(FdoString* propertyName)
{
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetRaster unsupported");
}

bool InsertFeatureReader::IsNull(FdoString* propertyName)
{
    if (mIsFirst || NULL == mProps)
    {
        throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader - end of recordset");
    }

    FdoPtr<FdoPropertyValue> propVal(mProps->GetItem(propertyName));
    if (NULL == propVal)
    {
        throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader - property not found");
    }

    FdoPtr<FdoValueExpression> valExp(propVal->GetValue());
    return (NULL == valExp.p);
}

bool InsertFeatureReader::ReadNext()
{
    bool ret = mIsFirst;
    mIsFirst = false;
    return ret;
}

void InsertFeatureReader::Close()
{
    // idle
}

///////////////////////////////////////////////////////////////////////////////
// FdoIFeatureReader interface
///////////////////////////////////////////////////////////////////////////////

FdoClassDefinition* InsertFeatureReader::GetClassDefinition()
{
    return FDO_SAFE_ADDREF(mClassDef.p);
}

FdoInt32 InsertFeatureReader::GetDepth()
{
    // XXX - mloskot: Return Zero or throw?
    //return 0;
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetDepth unsupported");
}

const FdoByte* InsertFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* count)
{
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetGeometry unsupported");
}

FdoByteArray* InsertFeatureReader::GetGeometry(FdoString* propertyName)
{
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetGeometry unsupported");
}

FdoIFeatureReader* InsertFeatureReader::GetFeatureObject(FdoString* propertyName)
{
    throw FdoCommandException::Create(
        L"[PostGIS] InsertFeatureReader::GetFeatureObject unsupported");
}

///////////////////////////////////////////////////////////////////////////////
// Private operations
///////////////////////////////////////////////////////////////////////////////

FdoDataValue* InsertFeatureReader::GetDataValue(FdoString* propertyName)
{
    if (mIsFirst || NULL == mProps)
    {
        throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader - end of recordset");
    }

    FdoPtr<FdoPropertyValue> propVal(mProps->GetItem(propertyName));
    if (NULL == propVal)
    {
        throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader - property not found");
    }

    FdoPtr<FdoValueExpression> valExp(propVal->GetValue());
    FdoDataValue* dataVal = dynamic_cast<FdoDataValue*>(valExp.p);
    if (NULL == dataVal)
    {
        throw FdoCommandException::Create(
            L"[PostGIS] InsertFeatureReader - invalid data value");
    }

    // TODO - mloskot: Check of ref-counter works here
    FDO_SAFE_ADDREF(valExp.p);
    return dataVal; 
}

}} // namespace fdo::postgis


