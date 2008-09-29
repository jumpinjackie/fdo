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

#include "FdoRdbmsFeatureInfoReader.h"
#include <Inc/Nls/fdordbms_msg.h>
#include "GdbiException.h"
#include <Sm/Lp/ObjectPropertyClass.h>

static  char  *strNotSupportedExp = "Function not supported by this reader";
static  char  *strEndOfRecordExp = "End of feature data or NextFeature not called";

// Class FdoRdbmsFeatureInfoReader
FdoRdbmsFeatureInfoReader::FdoRdbmsFeatureInfoReader( FdoPropertyValueCollection    *mFeatInfo, const FdoSmLpClassDefinition *classDefinition  ):
  mFeatInfoCollection( NULL ),
  mCalled( false ),
  mClassDefinition( classDefinition )
{
    if( mFeatInfo )
        mFeatInfo->AddRef();

    mFeatInfoCollection = mFeatInfo;
}

FdoRdbmsFeatureInfoReader::~FdoRdbmsFeatureInfoReader()
{
  if(  mFeatInfoCollection )
    mFeatInfoCollection->Release();

}

FdoClassDefinition *FdoRdbmsFeatureInfoReader::GetClassDefinition()
{
    if( ! mCalled || mFeatInfoCollection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    FdoPtr<FdoDataPropertyDefinition> property;
    FdoPtr<FdoPropertyDefinitionCollection> properties = FdoPropertyDefinitionCollection::Create(NULL);
    FdoClassDefinition *classDefinition = NULL;
    const FdoSmLpObjectPropertyClass *objectPropertyClass = dynamic_cast<const FdoSmLpObjectPropertyClass *> (mClassDefinition);
    if (objectPropertyClass != NULL)
    {
        const FdoSmLpObjectPropertyDefinition* def = objectPropertyClass->RefObjectProperty();
        const FdoSmLpClassDefinition *classDef = def->RefClass();
        classDefinition = FdoClass::Create(classDef->GetName(), classDef->GetDescription());
    }
    else
    {
        classDefinition = FdoFeatureClass::Create(mClassDefinition->GetName(), mClassDefinition->GetDescription());
    }

    classDefinition->SetIsAbstract(false);
    properties = classDefinition->GetProperties();

    for (int i=0; i<mFeatInfoCollection->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue> propertyValue = mFeatInfoCollection->GetItem(i);
        FdoValueExpression *value = propertyValue->GetValue();
        FdoDataValue *dataValue = (static_cast<FdoDataValue*>(value));
        FdoPtr<FdoIdentifier> id = propertyValue->GetName();
        property = FdoDataPropertyDefinition::Create();
        property->SetName(id->GetName());
        property->SetDataType(dataValue->GetDataType());
        dataValue->Release();
        properties->Add(property);
    }

    return classDefinition;
}

int FdoRdbmsFeatureInfoReader::GetDepth()
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

FdoLOBValue* FdoRdbmsFeatureInfoReader::GetLOB(const wchar_t* propertyName )
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

FdoIStreamReader* FdoRdbmsFeatureInfoReader::GetLOBStreamReader(const wchar_t* propertyName )
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

FdoIFeatureReader* FdoRdbmsFeatureInfoReader::GetFeatureObject( const wchar_t* propertyName )
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

bool FdoRdbmsFeatureInfoReader::GetBoolean( const wchar_t *propertyName )
{
    return (GetInt32( propertyName ) != 0 );
}

FdoByte FdoRdbmsFeatureInfoReader::GetByte( const wchar_t *propertyName )
{
    return (FdoByte)GetInt32( propertyName );
}


FdoDateTime FdoRdbmsFeatureInfoReader::GetDateTime( const wchar_t *propertyName )
{
	FdoPropertyValue *featInfoProp = NULL;

    if( ! mCalled || mFeatInfoCollection == NULL )
		throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    featInfoProp =  mFeatInfoCollection->GetItem( propertyName );
        
	FdoValueExpression *val = featInfoProp->GetValue();
	featInfoProp->Release();
	FdoDataValue *dataValue = (static_cast<FdoDataValue*>(val));
	if ( dataValue->GetDataType() == FdoDataType_DateTime )
	{
		FdoDateTimeValue *dateTimeValue = (static_cast<FdoDateTimeValue*>(dataValue));
		dataValue->Release();
		return dateTimeValue->GetDateTime();
	}
	else
	   throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp)); // This should not happen; we just created this in the insert command. But just in case
}

double FdoRdbmsFeatureInfoReader::GetDouble( const wchar_t *propertyName )
{
	FdoPropertyValue *featInfoProp = NULL;

    if( ! mCalled || mFeatInfoCollection == NULL )
		throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

	featInfoProp =  mFeatInfoCollection->GetItem( propertyName );
        
	FdoValueExpression *val = featInfoProp->GetValue();
	featInfoProp->Release();
	FdoDataValue *dataValue = (static_cast<FdoDataValue*>(val));
	if( dataValue->GetDataType() == FdoDataType_Double || 
	   dataValue->GetDataType() == FdoDataType_Decimal ||
	   dataValue->GetDataType() == FdoDataType_Single )
	{
		FdoDoubleValue  *doubleValue = (static_cast<FdoDoubleValue*>(dataValue));
		dataValue->Release();
		return doubleValue->GetDouble();
	}
	else
	   throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp)); // This should not happen; we just created this in the insert command. But just in case
}

const wchar_t* FdoRdbmsFeatureInfoReader::GetString( const wchar_t *propertyName )
{
	FdoPropertyValue *featInfoProp = NULL;

    if( ! mCalled || mFeatInfoCollection == NULL )
		throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    featInfoProp =  mFeatInfoCollection->GetItem( propertyName );
        
	FdoValueExpression *val = featInfoProp->GetValue();
	featInfoProp->Release();
	FdoDataValue *dataValue = (static_cast<FdoDataValue*>(val));
	if( dataValue->GetDataType() == FdoDataType_String )
	{
		FdoStringValue *stringValue = (static_cast<FdoStringValue*>(dataValue));
		dataValue->Release();
		return stringValue->GetString();
	}
	else
	   throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp)); // This should not happen; we just created this in the insert command. But just in case
}
short FdoRdbmsFeatureInfoReader::GetInt16( const wchar_t *propertyName )
{
    return (short)GetInt64( propertyName );
}

int FdoRdbmsFeatureInfoReader::GetInt32( const wchar_t *propertyName )
{
    return (int)GetInt64( propertyName );
}

FdoInt64 FdoRdbmsFeatureInfoReader::GetInt64( const wchar_t *propertyName )
{
    FdoPropertyValue *featInfoProp = NULL;

    if( ! mCalled || mFeatInfoCollection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    featInfoProp =  mFeatInfoCollection->GetItem( propertyName );
        
	FdoValueExpression *val = featInfoProp->GetValue();
	featInfoProp->Release();
	FdoDataValue *dataValue = (static_cast<FdoDataValue*>(val));
	if( dataValue->GetDataType() == FdoDataType_Int64 ||
	   dataValue->GetDataType() == FdoDataType_Int16 ||
	   dataValue->GetDataType() == FdoDataType_Int32 ||
	   dataValue->GetDataType() == FdoDataType_Byte ||
	   dataValue->GetDataType() == FdoDataType_Boolean )
	{
		FdoInt64Value  *int64Value = (static_cast<FdoInt64Value*>(dataValue));
		dataValue->Release();
		return int64Value->GetInt64();
	}
	else
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp)); // This should not happen; we just created this in the insert command. But just in case

}

float FdoRdbmsFeatureInfoReader::GetSingle( const wchar_t *propertyName )
{
     return (float)GetDouble( propertyName );
}

bool FdoRdbmsFeatureInfoReader::IsNull( const wchar_t *propertyName )
{
    FdoPropertyValue *featInfoProp = NULL;

    if( ! mCalled || mFeatInfoCollection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_57, strEndOfRecordExp));

    featInfoProp =  mFeatInfoCollection->GetItem( propertyName );

    FdoValueExpression *val = featInfoProp->GetValue();
    featInfoProp->Release();
    if( val == NULL )
        return true;

    val->Release();
    return false;
}

void FdoRdbmsFeatureInfoReader::ThrowPropertyNotFountExp( const wchar_t* propertyName )
{
   throw FdoCommandException::Create(NlsMsgGet1(FDORDBMS_56, "Property '%1$ls' not found", propertyName));
}

FdoByteArray* FdoRdbmsFeatureInfoReader::GetGeometry(const wchar_t* propertyName )
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

const FdoByte * FdoRdbmsFeatureInfoReader::GetGeometry(const wchar_t* propertyName, FdoInt32 * count)
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

FdoIRaster* FdoRdbmsFeatureInfoReader::GetRaster(const wchar_t* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_240, strNotSupportedExp));
}

bool FdoRdbmsFeatureInfoReader::ReadNext( )
{
    bool ret = ! mCalled;

    if( mFeatInfoCollection == NULL )
        throw FdoCommandException::Create(NlsMsgGet(FDORDBMS_19, "Query ended"));

    mCalled = true;
    if( ! ret )
    {
        if(  mFeatInfoCollection )
            mFeatInfoCollection->Release();

        mFeatInfoCollection = NULL;
    }
    return ret;
}

void FdoRdbmsFeatureInfoReader::Close( )
{

}

void FdoRdbmsFeatureInfoReader::Dispose()
{
    delete this;
}


