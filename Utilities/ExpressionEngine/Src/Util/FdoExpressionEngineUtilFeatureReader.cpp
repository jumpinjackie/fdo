// 
//  
//  Copyright (C) 2004-2008  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include "stdafx.h"
#include <Util/FdoExpressionEngineUtilFeatureReader.h>
#include <FdoCommonMiscUtil.h>
#include "FdoCommonSchemaUtil.h"
#include <assert.h>

#define EXPRESSIONENGINE_GET_RESULT_DEFINE(RESULT_TYPE, OBJECT_TYPE) \
{ \
	RESULT_TYPE	ret; \
	if ( IsComputedProperty( propertyName ) ) \
	{ \
		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName); \
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data) \
		{ \
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p); \
			if (dataValue->GetDataType() == FdoDataType_##OBJECT_TYPE) \
			{ \
				Fdo##OBJECT_TYPE##Value *value = static_cast<Fdo##OBJECT_TYPE##Value *>(dataValue); \
				ret = value->Get##OBJECT_TYPE(); \
			} \
		} \
	} \
	else \
	{ \
		ret = m_reader->Get##OBJECT_TYPE(propertyName); \
	} \
	return ret; \
}

FdoExpressionEngineUtilFeatureReader::FdoExpressionEngineUtilFeatureReader (FdoClassDefinition* classDef, FdoIFeatureReader* reader, FdoFilter *filter, FdoIdentifierCollection* selectedIds, FdoExpressionEngineFunctionCollection *userDefinedFunctions) 
{
	m_reader = FDO_SAFE_ADDREF(reader);
	m_filter = FDO_SAFE_ADDREF(filter);
	
    FdoPtr<FdoCommonSchemaCopyContext> copyContext;
    if (selectedIds != NULL && selectedIds->GetCount() > 0)
        copyContext = FdoCommonSchemaCopyContext::Create(selectedIds);
    m_classDef = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(classDef, copyContext);//FDO_SAFE_ADDREF(classDef);
    
    m_selectedIds = FDO_SAFE_ADDREF(selectedIds);
	m_computedIds = NULL;

	// Create a list of computed ids only.
	if ( selectedIds )
	{
		m_computedIds = FdoIdentifierCollection::Create();
        FdoPtr<FdoPropertyDefinitionCollection> props = m_classDef->GetProperties();

		for (FdoInt32 i = 0; i < selectedIds->GetCount(); i++)
		{
			FdoPtr<FdoIdentifier> pPropertyId = selectedIds->GetItem(i);
			FdoComputedIdentifier* pComputedId = dynamic_cast<FdoComputedIdentifier*>(pPropertyId.p);

			if ( pComputedId )
            {
                m_computedIds->Add(	pPropertyId );

                // Add it into class definition
                FdoPtr<FdoExpression> expr = pComputedId->GetExpression();

                FdoPropertyType propType;
                FdoDataType dataType;

                FdoExpressionEngine::GetExpressionType(classDef, expr, propType, dataType);

                FdoPtr<FdoDataPropertyDefinition> pd = FdoDataPropertyDefinition::Create(pComputedId->GetName(), NULL);
                pd->SetDataType(dataType);
                props->Add(pd);

            }
		}
	}

	// In the presence of the filter, pass NULL for the list of properties (i.e. all ) 
	m_filterExec = FdoExpressionEngine::Create (reader, classDef, 
												m_filter ? m_computedIds : m_computedIds, 
												userDefinedFunctions);
}

FdoExpressionEngineUtilFeatureReader::~FdoExpressionEngineUtilFeatureReader()
{
}

bool FdoExpressionEngineUtilFeatureReader::IsComputedProperty(FdoString* name)
{
	FdoPtr<FdoIdentifier> dummy;

    if ( m_computedIds )
		dummy = m_computedIds->FindItem(name);

	return (dummy != NULL );
}

FdoString* FdoExpressionEngineUtilFeatureReader::GetPropertyName(FdoInt32 index)
{
    assert(m_selectedIds != NULL && index < m_selectedIds->GetCount());

    if (m_selectedIds != NULL && index < m_selectedIds->GetCount())
    {
        FdoPtr<FdoIdentifier> id = m_selectedIds->GetItem(index);
        if (id != NULL)
            return id->GetName();
    }

    throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_73_PROPERTY_INDEXOUTOFBOUNDS), index));
    return L""; //to suppress compiler warning.
}

FdoInt32 FdoExpressionEngineUtilFeatureReader::GetPropertyIndex(FdoString* propertyName)
{
    FdoInt32 index = -1;

    assert(m_selectedIds != NULL);
    if (m_selectedIds != NULL)
    {
        index = m_selectedIds->IndexOf(propertyName);
    }

    if (-1 != index)
    {
        return index;
    }
    else
    {
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_74_PROPERTY_NAME_NOT_FOUND), propertyName));
        return -1; // to suppress compiler warning.
    }
}

bool FdoExpressionEngineUtilFeatureReader::GetBoolean (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( bool, Boolean );
}

FdoByte FdoExpressionEngineUtilFeatureReader::GetByte (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( FdoByte, Byte );
}

FdoDateTime FdoExpressionEngineUtilFeatureReader::GetDateTime (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( FdoDateTime, DateTime );
}

double FdoExpressionEngineUtilFeatureReader::GetDouble (FdoString* propertyName)
{
	double	ret = 0;
	if ( IsComputedProperty( propertyName ) )
	{
		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			if ((dataValue->GetDataType() == FdoDataType_Double) ||
			    (dataValue->GetDataType() == FdoDataType_Decimal)   )
			{
				FdoDoubleValue *value = static_cast<FdoDoubleValue *>(dataValue);
				ret = value->GetDouble();
			}
		}
	}
	else
	{
		ret = m_reader->GetDouble(propertyName);
	}
	return ret;
}

FdoInt16 FdoExpressionEngineUtilFeatureReader::GetInt16 (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( FdoInt16, Int16 );
}

FdoInt32 FdoExpressionEngineUtilFeatureReader::GetInt32 (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( FdoInt32, Int32 );
}

FdoInt64 FdoExpressionEngineUtilFeatureReader::GetInt64 (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( FdoInt64, Int64 );
}

float FdoExpressionEngineUtilFeatureReader::GetSingle (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( float, Single );
}

FdoString* FdoExpressionEngineUtilFeatureReader::GetString (FdoString* propertyName)
{
	EXPRESSIONENGINE_GET_RESULT_DEFINE( FdoString*, String );
}

FdoLOBValue* FdoExpressionEngineUtilFeatureReader::GetLOB(FdoString* propertyName)
{
	if ( IsComputedProperty( propertyName ) )
		throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_72_LOB_DATA_TYPE_NOT_SUPPORTED)));
	else
		return m_reader->GetLOB( propertyName );
}

FdoIStreamReader* FdoExpressionEngineUtilFeatureReader::GetLOBStreamReader(const wchar_t* propertyName)
{
	if ( IsComputedProperty( propertyName ) )
		throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_72_LOB_DATA_TYPE_NOT_SUPPORTED)));
	else
		return m_reader->GetLOBStreamReader( propertyName );
}

bool FdoExpressionEngineUtilFeatureReader::IsNull(FdoString* propertyName)
{
	bool	 ret;

	if ( IsComputedProperty( propertyName ) )
	{
		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		
		if ( results->GetLiteralValueType() == FdoLiteralValueType_Data )
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			ret = dataValue->IsNull();
		}
		else if ( results->GetLiteralValueType() == FdoLiteralValueType_Geometry )
		{
			FdoGeometryValue *geomValue = static_cast<FdoGeometryValue *> (results.p);
			ret = geomValue->IsNull();
		}
		else
            throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
	}
	else
	{
		ret = m_reader->IsNull(propertyName);
	}
	return ret;
}

FdoByteArray* FdoExpressionEngineUtilFeatureReader::GetGeometry(FdoString* propertyName)
{
	FdoByteArray* ret = NULL;

	if ( IsComputedProperty( propertyName ) )
	{
		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Geometry)
		{
			FdoGeometryValue *geomValue = static_cast<FdoGeometryValue *> (results.p);
			bool bIsNull = geomValue->IsNull();
			ret = bIsNull ? NULL : geomValue->GetGeometry();
		}
	}
	else
	{
		ret = m_reader->GetGeometry(propertyName);
	}
	return ret;
}

FdoIRaster* FdoExpressionEngineUtilFeatureReader::GetRaster(FdoString* propertyName)
{
	if ( IsComputedProperty( propertyName ) )
		throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), FdoCommonMiscUtil::FdoPropertyTypeToString(FdoPropertyType_RasterProperty)));
	else
		return m_reader->GetRaster(propertyName);
}

bool FdoExpressionEngineUtilFeatureReader::GetBoolean (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetBoolean(propertyName);
}

FdoByte FdoExpressionEngineUtilFeatureReader::GetByte (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetByte(propertyName);
}

FdoDateTime FdoExpressionEngineUtilFeatureReader::GetDateTime (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDateTime(propertyName);
}

double FdoExpressionEngineUtilFeatureReader::GetDouble (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetDouble(propertyName);
}

FdoInt16 FdoExpressionEngineUtilFeatureReader::GetInt16 (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt16(propertyName);
}

FdoInt32 FdoExpressionEngineUtilFeatureReader::GetInt32 (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt32(propertyName);
}

FdoInt64 FdoExpressionEngineUtilFeatureReader::GetInt64 (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetInt64(propertyName);
}

float FdoExpressionEngineUtilFeatureReader::GetSingle (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetSingle(propertyName);
}

FdoString* FdoExpressionEngineUtilFeatureReader::GetString (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetString(propertyName);
}

FdoLOBValue* FdoExpressionEngineUtilFeatureReader::GetLOB(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOB(propertyName);
}

FdoIStreamReader* FdoExpressionEngineUtilFeatureReader::GetLOBStreamReader(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetLOBStreamReader(propertyName);
}

bool FdoExpressionEngineUtilFeatureReader::IsNull(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return IsNull(propertyName);
}

FdoByteArray* FdoExpressionEngineUtilFeatureReader::GetGeometry(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName);
}

FdoIRaster* FdoExpressionEngineUtilFeatureReader::GetRaster(FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetRaster(propertyName);
}

void FdoExpressionEngineUtilFeatureReader::Close()
{
	m_reader->Close();
}

void FdoExpressionEngineUtilFeatureReader::Dispose()
{
    delete this;
}

bool FdoExpressionEngineUtilFeatureReader::ReadNext()
{
	bool	ret = false;

	// Evaluate the filter if any
    if (m_filter)
    {
        bool passedFilter = true;
        do
        {
			if ( m_reader->ReadNext() )
			{
				passedFilter = m_filterExec->ProcessFilter(m_filter);
				ret = passedFilter;
			}
			else
				passedFilter = true; // exit
        }
        while(!passedFilter);
    }
	else
		ret = m_reader->ReadNext();
	
	return ret;
}

FdoClassDefinition* FdoExpressionEngineUtilFeatureReader::GetClassDefinition ()
{
	return FDO_SAFE_ADDREF(m_classDef.p);
}

FdoInt32 FdoExpressionEngineUtilFeatureReader::GetDepth ()
{
    return (0);
}

const FdoByte* FdoExpressionEngineUtilFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32 * count)
{
    FdoByteArray * geom = GetGeometry(propertyName);

    if ( count )
        *count = geom ? geom->GetCount() : 0;

    return (geom? geom->GetData() : NULL );
}

FdoIFeatureReader* FdoExpressionEngineUtilFeatureReader::GetFeatureObject (FdoString* propertyName)
{
    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_70_PROPERTY_TYPE_NOT_SUPPORTED), L"Object property"));
}

const FdoByte* FdoExpressionEngineUtilFeatureReader::GetGeometry(FdoInt32 index, FdoInt32 * count)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetGeometry(propertyName, count);
}

FdoIFeatureReader* FdoExpressionEngineUtilFeatureReader::GetFeatureObject (FdoInt32 index)
{
    FdoStringP propertyName = GetPropertyName(index);
    return GetFeatureObject(propertyName);
}
