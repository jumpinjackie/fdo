// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
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
#include "SdfSimpleFeatureReader.h"
#include <FdoExpressionEngine.h>
#include <Fdo/Expression/LiteralValue.h>

#include "SdfConnection.h"
#include "PropertyIndex.h"
#include "BinaryReader.h"
#include "DataDb.h"
#include "FdoCommonSchemaUtil.h"
#include "KeyDb.h"
#include "SchemaDb.h"



//-------------------------------------------------------
// Constructor / destructor
//-------------------------------------------------------


// constructs an SdfSimpleFeatureReader using the specified
// connection, class definition, and Sdf reader
SdfSimpleFeatureReader::SdfSimpleFeatureReader(SdfConnection* connection, FdoClassDefinition* classDef, FdoFilter* filter, recno_list* features,
                                               FdoIdentifierCollection* selectIdents, FdoPropertyDefinitionCollection* computedProps)
{
    _ASSERT(classDef);
    m_class = classDef;
    m_class->AddRef();

    _ASSERT(connection);
    m_connection = connection;
    m_connection->AddRef();

    m_filter = filter;
    FDO_SAFE_ADDREF(m_filter);

    //if the user specified a list of properties they want,
    //generate a copy of the feature class containing only these properties
    //and also add the computed properties
    if (selectIdents && selectIdents->GetCount() > 0)
        m_classDefPruned = CloneAndPruneClass(m_class, selectIdents, computedProps);
    else 
        m_classDefPruned = NULL;


    //obtain cursor into correct feature database
    m_dbData = m_connection->GetDataDb(m_class);
	m_dbData->SetTag( (void*) this );
    m_currentKey = new SQLiteData();
    m_currentKey->set_size(sizeof(REC_NO));

    m_currentData = new SQLiteData();

    //this prop index might change if the feature at hand
    //has a feature class that is a child of the
    //given feature class -- needed so that we know
    //how the class properties are ordered in the data record
    m_propIndex = m_connection->GetPropertyIndex(classDef);
    
    //this prop index stays fixed
    m_basePropIndex = m_propIndex;

    m_dataReader = new BinaryReader(NULL, 0, m_propIndex->GetNumProps() );

    if (m_filter)
    {
        m_filterExec = FdoExpressionEngine::Create(this, m_class, selectIdents, NULL);
    }
    else
    {
        m_currentFeatureRecno = 1; //init to first record
        m_filterExec = NULL;
    }

    //we might still need an expression engine even if there
    //is no filter, in case there are computed properties
    if (!m_filterExec && computedProps && (computedProps->GetCount() > 0))
    {
		m_filterExec = FdoExpressionEngine::Create(this, m_class, selectIdents, NULL);
    }

    m_features = features;
    if (m_features)
        m_currentFeatureRecnoIter = m_features->begin();
    else
        m_currentFeatureRecno = 0; 

    m_currentFCID = 0;
}

SdfSimpleFeatureReader::SdfSimpleFeatureReader( SdfSimpleFeatureReader& reader )
{

	m_class = reader.m_class;
    m_class->AddRef();

    m_connection = reader.m_connection;
    m_connection->AddRef();

    m_filter = reader.m_filter;
    FDO_SAFE_ADDREF(m_filter);

	m_classDefPruned = NULL;

	m_dbData = m_connection->GetDataDb(m_class);
	m_dbData->SetTag( (void*) this );
    m_currentKey = new SQLiteData();
    m_currentKey->set_size(sizeof(REC_NO));

    m_currentData = new SQLiteData();

	m_propIndex = m_connection->GetPropertyIndex(m_class);
	 //this prop index stays fixed
    m_basePropIndex = m_propIndex;

    m_filterExec = FdoExpressionEngine::Create(this, m_class, NULL, NULL);


    m_dataReader = new BinaryReader(NULL, 0, m_propIndex->GetNumProps() );

	m_currentFeatureRecno = reader.m_currentFeatureRecno;

	if( reader.m_features )
	{
		m_features = new recno_list;
		recno_list::iterator iter = reader.m_features->begin();
		for (iter = reader.m_features->begin(); iter != reader.m_features->end(); iter++ )
			m_features->push_back(*iter);

		m_currentFeatureRecnoIter = m_features->begin();
	}
	else
	{
		m_features = NULL;
		m_currentFeatureRecno = 0;
	}

    m_currentFCID = 0;
}

// default destructor
SdfSimpleFeatureReader::~SdfSimpleFeatureReader()
{
    Close();

    FDO_SAFE_RELEASE(m_class);
    FDO_SAFE_RELEASE(m_connection);
    FDO_SAFE_RELEASE(m_filter);
    FDO_SAFE_RELEASE(m_classDefPruned);

    //need to do this because of multiple inheritance in SdfFilterExecutor
    FdoIFilterProcessor* asfilter = (FdoIFilterProcessor*)m_filterExec;
    FDO_SAFE_RELEASE(asfilter);
//    FdoIExpressionProcessor* asexpr = (FdoIExpressionProcessor*)m_filterExec;
//    FDO_SAFE_RELEASE(asexpr);

    //it is our responsibility to free the feature recno list here
    if (m_features)
        delete m_features;

    delete m_currentKey;
    delete m_currentData;
    delete m_dataReader;
}

//-------------------------------------------------------
// FdoIDisposable implementation
//-------------------------------------------------------

// dispose this object
void SdfSimpleFeatureReader::Dispose()
{
    delete this;
}

//-------------------------------------------------------
// FdoIFeatureReader implementation
//-------------------------------------------------------


// Gets the definition of the object currently being read.  
FdoClassDefinition* SdfSimpleFeatureReader::GetClassDefinition()
{
    //return a clone of either the pruned class (containing only
    //properties the user requested or a clone of the original
    //class if we need all properties or the user didn't care.
    if (m_classDefPruned)
    {
        if (m_lastClassDefinition == NULL)
            m_lastClassDefinition = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(m_classDefPruned);

        return FDO_SAFE_ADDREF(m_lastClassDefinition.p);
    }
    else
    {
        if (m_lastClassDefinition == NULL)
            m_lastClassDefinition = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(m_class);

        return FDO_SAFE_ADDREF(m_lastClassDefinition.p);
    }
}

// Gets a value indicating the depth of nesting for the current reader.
// The depth value increases each time GetFeatureObject is called and
// a new reader is returned.  The outermost reader has a depth of 0.
FdoInt32 SdfSimpleFeatureReader::GetDepth()
{
    //SDF+ does not support nested objects
    return 0;
}

FdoDataType SdfSimpleFeatureReader::GetDataType( FdoString* propertyName )
{
	PropertyStub* ps =  m_propIndex->GetPropInfo(propertyName);
	if( ps )
		return ps->m_dataType;

	return (FdoDataType)-1;
}

// Gets the Boolean value of the specified property.  No conversion
// is performed, therefore the property must be of type Boolean or
// an exception is thrown.
bool SdfSimpleFeatureReader::GetBoolean(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps =  m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_Boolean)
			{
				FdoBooleanValue *booleanValue = static_cast<FdoBooleanValue *>(dataValue);
				return booleanValue->GetBoolean();
			}
		}
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
    }

    if (ps->m_dataType != FdoDataType_Boolean)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));
    
    //read property value
    unsigned char ret = m_dataReader->ReadByte();
    return (ret != 0);
}
 

// Gets the Byte value of the specified property.  No conversion
// is performed, therefore the property must be of type Byte or
// an exception is thrown.
FdoByte SdfSimpleFeatureReader::GetByte(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (!ps)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    if (ps->m_dataType != FdoDataType_Byte)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadByte();
}

// Gets the DateTime value of the specified property.  No conversion
// is performed, therefore the property must be of type DateTime or
// an exception is thrown.
FdoDateTime SdfSimpleFeatureReader::GetDateTime(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

        FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_DateTime)
			{
				FdoDateTimeValue *dateTimeValue = static_cast<FdoDateTimeValue *>(dataValue);
				return dateTimeValue->GetDateTime();
			}
		}
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
    }

    if (ps->m_dataType != FdoDataType_DateTime)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadDateTime();
}

// Gets the Double floating point value of the specified property.
// No conversion is performed, therefore the property must be of
// type Double or an exception is thrown.
double SdfSimpleFeatureReader::GetDouble(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_Double)
			{
				FdoDoubleValue *doubleValue = static_cast<FdoDoubleValue *>(dataValue);
				return doubleValue->GetDouble();
			}
		}
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
    }

    if (ps->m_dataType != FdoDataType_Double && ps->m_dataType != FdoDataType_Decimal)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadDouble();
}

// Gets the Int16 value of the specified property.  No conversion
// is performed, therefore the property must be of type Int16 or
// an exception is thrown.
FdoInt16 SdfSimpleFeatureReader::GetInt16(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (!ps)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    if (ps->m_dataType != FdoDataType_Int16)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadInt16();
}

// Gets the Int32 value of the specified property.  No conversion
// is performed, therefore the property must be of type Int32 or
// an exception is thrown.
FdoInt32 SdfSimpleFeatureReader::GetInt32(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_Int32)
			{
				FdoInt32Value *int32Value = static_cast<FdoInt32Value *>(dataValue);
				return int32Value->GetInt32();
			}
		}
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
    }

    if (ps->m_dataType != FdoDataType_Int32)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    //read-only autogenerated property value
    if (ps->m_isAutoGen)
        return m_currentFeatureRecno;
   
    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadInt32();
}

// Gets the Int64 value of the specified property.  No conversion
// is performed, therefore the property must be of type Int64 or
// an exception is thrown.
FdoInt64 SdfSimpleFeatureReader::GetInt64(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_Int64)
			{
				FdoInt64Value *int64Value = static_cast<FdoInt64Value *>(dataValue);
				return int64Value->GetInt64();
			}
		}
        
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
    }

    if (ps->m_dataType != FdoDataType_Int64)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadInt64();
}

// Gets the Single floating point value of the specified property.
// No conversion is performed, therefore the property must be of
// type Single or an exception is thrown.
float SdfSimpleFeatureReader::GetSingle(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoSingleValue *dataValue = static_cast<FdoSingleValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_Single)
			{
				FdoSingleValue *singleValue = static_cast<FdoSingleValue *>(dataValue);
				return singleValue->GetSingle();
			}
		}
        
        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
    }

    if (ps->m_dataType != FdoDataType_Single)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    return m_dataReader->ReadSingle();
}

FdoValueExpression* SdfSimpleFeatureReader::GetValue( FdoDataType data_type )
{
	FdoValueExpression	*exp = NULL;

	switch( data_type )
	{
		case FdoDataType_Boolean:
			exp = FdoBooleanValue::Create( (m_dataReader->ReadByte() != 0 ));
			break;

		case FdoDataType_Byte:
			exp = FdoByteValue::Create( m_dataReader->ReadByte() );
			break;

		case FdoDataType_Single:
			exp = FdoSingleValue::Create(m_dataReader->ReadSingle());
			break;

		case FdoDataType_String:
			exp = FdoStringValue::Create( m_dataReader->ReadString() );
			break;

		case FdoDataType_DateTime:
			exp = FdoDateTimeValue::Create( m_dataReader->ReadDateTime() );
			break;

		case FdoDataType_Double:
		case FdoDataType_Decimal:
			exp = FdoDoubleValue::Create( m_dataReader->ReadDouble() );
			break;

		case FdoDataType_Int16:
			exp = FdoInt16Value::Create( m_dataReader->ReadInt16() );
			break;

		case FdoDataType_Int32:
			exp = FdoInt32Value::Create( m_dataReader->ReadInt32() );
			break;

		case FdoDataType_Int64:
			exp = FdoInt64Value::Create( m_dataReader->ReadInt64() );
			break;
	}
    return exp;
}

// Gets the String value of the specified property.  No conversion
// is performed, therefore the property must be of type String or
// an exception is thrown.
FdoString* SdfSimpleFeatureReader::GetString(FdoString* propertyName)
{
	RefreshData();

    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //check for computed property
    if (!ps)
    {
        CheckIfPropExists(propertyName);

        if (m_stringPropsCache[propertyName])
            return m_stringPropsCache[propertyName];

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);

		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoStringValue *dataValue = static_cast<FdoStringValue *> (results.p);
			if (dataValue->GetDataType() == FdoDataType_String)
			{
				FdoStringValue *stringValue = static_cast<FdoStringValue *>(dataValue);
				wchar_t* ret = new wchar_t[wcslen(stringValue->GetString())+1];
				wcscpy(ret, stringValue->GetString());
				m_stringPropsCache[propertyName] = (wchar_t *) ret;
				return ret;
			}
		}

        throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));
        
    }

    if (ps->m_dataType != FdoDataType_String)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    //check for null value
    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));

    //returns a pointer to the BinaryReader string cache memory
    //the pointer is valid until the next call to ReadNext(), when
    //the binary reader is reset
    FdoString* st = m_dataReader->ReadRawString(len, ps->m_recordIndex );

    return st;
}


FdoLOBValue* SdfSimpleFeatureReader::GetLOB(FdoString* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));
}

FdoIStreamReader* SdfSimpleFeatureReader::GetLOBStreamReader(FdoString* propertyName )
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_13_LOBS_NOT_SUPPORTED)));
}

// Returns true if the value of the specified property is null.
bool SdfSimpleFeatureReader::IsNull(FdoString* propertyName)
{
	RefreshData();

    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    //it may be a computed property...
    if (!ps) 
    {
        CheckIfPropExists(propertyName);

		FdoPtr<FdoLiteralValue> results = m_filterExec->Evaluate(propertyName);
		if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
		{
			FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
			return dataValue->IsNull();
			{
				FdoBooleanValue *booleanValue = static_cast<FdoBooleanValue *>(dataValue);
				return booleanValue->GetBoolean();
			}

		}
        else if (results->GetLiteralValueType() == FdoLiteralValueType_Geometry)
        {
            FdoGeometryValue *geometryValue = static_cast<FdoGeometryValue *> (results.p);
            return geometryValue->IsNull();
        }
        else
            throw FdoException::Create(FdoException::NLSGetMessage (FDO_NLSID (FDO_57_UNEXPECTEDERROR)));

		return 0;
        
    }

	if( ps->m_propertyType == FdoPropertyType_AssociationProperty )
		return IsNullObject( ps );

    //read-only autogenerated property value
    if (ps->m_isAutoGen)
        return false;
    
    int len = PositionReader(ps->m_recordIndex);

	

    //if there is no data record for this propery, we will
    //say that it is NULL
    //TODO: I am not clear as to whether having a default value for 
    //this property in the schema would mean that the property is not null
    return (len == 0) && !ps->m_isAutoGen;
}

bool SdfSimpleFeatureReader::IsNullObject( PropertyStub* ps )
{
	RefreshData();
	// In case the association property has identity proeperties and only the reverse identities are initialized.
	// for example if the current class has a "name" property and the asociated class has a "first name" property.
	// If the association property is created with the identity proeprty set to "first name" and reverse identity
	// property set to "name", then the association can be initialized by either setting the "name" property or by
	// setting the asso_property."first name". If the later is set, then len != 0. If "name" is used to set the
	// the association, then len = 0 but the IsNUll should return false as the association is set(not necesary associated
	// to anything as the name can be associated to non-existing objects).
	int len = PositionReader( ps->m_recordIndex );
	if ( len != 0  )
		return false;

	FdoPtr<FdoPropertyDefinitionCollection> properties = m_class->GetProperties();
	FdoPtr<FdoPropertyDefinition> prop = properties->FindItem( ps->m_name );
	if( prop == NULL )
	{
		// check base properties
		FdoPtr<FdoReadOnlyPropertyDefinitionCollection> base_properties = m_class->GetBaseProperties();
		try
		{
			prop = base_properties->GetItem( ps->m_name );
		}
		catch(FdoException *e)
		{
			e->Release();
			throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));
		}
	}
	if( prop->GetPropertyType() != FdoPropertyType_AssociationProperty )
		throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_67_READER_ASSOCIATION_ONLY)));

	FdoAssociationPropertyDefinition* apd = (FdoAssociationPropertyDefinition*)prop.p;
	FdoPtr<FdoDataPropertyDefinitionCollection> idents = apd->GetReverseIdentityProperties();
	if( idents->GetCount() != 0 )
	{
		for(int i=0; i<idents->GetCount(); i++ )
		{
			FdoPtr<FdoDataPropertyDefinition>prop = idents->GetItem( i );
			if( IsNull( prop->GetName() ) )
				return true;
		}
		return false;
	}

	// If we reach this point, then this is most likely the case where this method is called for a reverse association.
	// In this case we need to see if the filter <reverse association property name>.<my identifier> = <current id>
	// executed for the associated class returns a value. Basically it means that I have objects associated to me
	// This is exactly the processing required to return a reader which in this case is a waiste of time as the user
	// will likely call us again to get the reader which will double the processing time. In this case we'll return
	// false and let the user get a reader that may return false for the first ReadNext.
	if( apd->GetIsReadOnly() )
		return false;
	else
		return true;
}

// Gets a reference to an IFeatureReader to read the data contained
// in the object or object collection property. 
FdoIFeatureReader* SdfSimpleFeatureReader::GetFeatureObject(FdoString* propertyName)
{
	RefreshData();

	PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (!ps)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    if (ps->m_propertyType != FdoPropertyType_AssociationProperty )
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    if ( IsNullObject( ps ) )
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));
 
	FdoPtr<FdoPropertyDefinitionCollection> properties = m_class->GetProperties();
	FdoPtr<FdoPropertyDefinition> prop = properties->FindItem( propertyName );
	if( prop == NULL )
	{
		// check base properties
		FdoPtr<FdoReadOnlyPropertyDefinitionCollection> base_properties = m_class->GetBaseProperties();
		try
		{
			prop = base_properties->GetItem( propertyName );
		}
		catch(FdoException *e)
		{
			e->Release();
			throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));
		}
	}
	if( prop->GetPropertyType() != FdoPropertyType_AssociationProperty )
		throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_67_READER_ASSOCIATION_ONLY)));

	FdoAssociationPropertyDefinition* apd = (FdoAssociationPropertyDefinition*)prop.p;

	// Create a reader for the associated class and a filter based on the following cases:
	// 1- if the association has identity properties(property from the associated class) and are set, then 
	//    a filter that look like: ident1 = <val of assoc.ident1> and ident2 = <val of assoc.ident2> ...
	// 2- If the association has identity properties but are not set, then the reverse identities(property from 
	//    the current class) value is used to build a filter similar to case one:
	//		ident1 = <val of rever_ident1> and ident2 = <val of rever_ident1> ...
	// 3- In case the association has no identity property, the associated class identity property is used(default).
	//    The value assoc.<associated class id1> ... should be set if the association is NOT read only. In that case
	//    we get the values of assoc.<associated class id1>s and build a filter similar to
	//    <<associated class id1>=val and ...
	//    If assoc.<associated class id1> is not set and the association IS read only, then this is the case where
	//    the user is trying to traverse the association backward(from the associated object to the associating object)
	//    In this case we build a filter of the form <reverse name>.<myid1> = <val of myid1> and <reverse name>.<myid2> = <val of myid2> ...
	FdoPtr<FdoClassDefinition>cls = apd->GetAssociatedClass();
	FdoPtr<FdoDataPropertyDefinitionCollection> idents = apd->GetIdentityProperties();
	FdoPtr<FdoDataPropertyDefinitionCollection> rev_idents = apd->GetReverseIdentityProperties();
	if( idents->GetCount() == 0 )
	{
		if( ! apd->GetIsReadOnly() )
			idents = cls->GetIdentityProperties();
		else
			idents = m_class->GetIdentityProperties();
	}

	FdoPtr<FdoFilter>associated_filter;
	for(int i=0; i<idents->GetCount(); i++ )
	{
		std::wstring wstr;
		FdoPtr<FdoValueExpression> val;
		FdoPtr<FdoDataPropertyDefinition>prop = idents->GetItem( i );
		if( ! apd->GetIsReadOnly() )
		{
			wstr = prop->GetName();
			val = GetValue( prop->GetDataType() );
		}
        if( val == NULL )
		{
			if( rev_idents->GetCount() != 0 )
			{
				FdoPtr<FdoDataPropertyDefinition>idProp = rev_idents->GetItem( i );
				wstr = prop->GetName();
				PropertyStub* ps = m_propIndex->GetPropInfo( idProp->GetName() );
				if( ! ps )
					break;
				
				if (ps->m_isAutoGen)
					val = FdoInt32Value::Create(m_currentFeatureRecno);
				else
				{
					int len = PositionReader(ps->m_recordIndex);
					if( len == 0 )
						break; // Just bail out. The association is not set therefore no associted objects exist

					if( idProp->GetDataType() == FdoDataType_String )
						val = FdoStringValue::Create( m_dataReader->ReadRawString( len, ps->m_recordIndex ) ); // Need to use the raw string getter
					else
						val = GetValue( idProp->GetDataType() ); 
				}
				if( val == NULL )
					break;  
			}
			else
			{
				FdoPtr<FdoDataPropertyDefinition>idProp = idents->GetItem( i );
				wstr = apd->GetReverseName();
				wstr += L".";
				wstr += idProp->GetName();
				PropertyStub* ps = m_propIndex->GetPropInfo( idProp->GetName() );
				if( ! ps )
					break; // Should not happen as it's the primary key of the object; it must exist
				
				if (ps->m_isAutoGen)
					val = FdoInt32Value::Create(m_currentFeatureRecno);
				else
				{
					int len = PositionReader(ps->m_recordIndex);
					if( len == 0 )
						break; // Should not happen as it's the primary key of the object; it must exist

					if( idProp->GetDataType() == FdoDataType_String )
						val = FdoStringValue::Create( m_dataReader->ReadRawString( len, ps->m_recordIndex ) ); // Need to use the raw string getter
					else
						val = GetValue( idProp->GetDataType() ); 
				}
				if( val == NULL )
					break;  
			}
		}
		
		FdoComparisonCondition* associated_cond = FdoComparisonCondition::Create(
					FdoPtr<FdoIdentifier>(FdoIdentifier::Create( wstr.c_str() ) ), 
					FdoComparisonOperations_EqualTo, val );
		if( associated_filter.p == NULL )
			associated_filter = associated_cond;
		else
		{
			associated_filter = FdoFilter::Combine( associated_filter, FdoBinaryLogicalOperations_And, associated_cond );
			associated_cond->Release();
		}
	}

	if( associated_filter.p != NULL )
	{
		FdoPtr<FdoISelect> select = (FdoISelect*)m_connection->CreateCommand(FdoCommandType_Select); 

		select->SetFeatureClassName( cls->GetName() );

		select->SetFilter(associated_filter);
    
		return select->Execute();
	}

    return NULL;
}


// Gets the Geometry value of the specified property as a byte array
// in FGF format.  No conversion is performed, therefore the property
// must be of Geometric type or an exception is thrown.
FdoByteArray* SdfSimpleFeatureReader::GetGeometry(FdoString* propertyName)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (!ps)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    if (ps->m_dataType != -1)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));
        
    void* ptr = m_dataReader->GetDataAtCurrentPosition();

    return FdoByteArray::Create((unsigned char*)ptr, len);
}

const FdoByte* SdfSimpleFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* retlen)
{
	RefreshData();
    PropertyStub* ps = m_propIndex->GetPropInfo(propertyName);

    if (!ps)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    if (ps->m_dataType != -1)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_36_INCORRECT_PROPERTY_TYPE)));

    int len = PositionReader(ps->m_recordIndex);

    if (len == 0)
        throw FdoException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_51_NULL_VALUE), "Property value is null."));
        
    void* ptr = m_dataReader->GetDataAtCurrentPosition();

    //TODO: we are trying to change FDO to return the simple pointer to 
    //the data instead of creating a FdoByteArray.
    *retlen = len;
    return (const FdoByte*)ptr;
}


FdoIRaster* SdfSimpleFeatureReader::GetRaster(FdoString* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_43_NO_RASTERS)));
}

// Advances the reader to the next item and returns true if there is
// another object to read or false if reading is complete.  The default
// position of the reader is prior to the first item.  Therefore you
// must call ReadNext to begin accessing any data.
bool SdfSimpleFeatureReader::ReadNext()
{   
    if (m_filter || m_features)
    {
        //filter could have been optimized away by query optimizer
        //in which case we may have a list of specific features
        //the optimizer was able to reduce the search to

        bool passedFilter = true;
        do
        {
            int ret = 0;

            //if we were initially given a list of features... obtain feature
            //record for the next record number in the list, otherhwise
            //just iterate over records sequentially
            if (m_features)
            {
                //are we at the end of the list?
                if (m_currentFeatureRecnoIter == m_features->end())
                    return false;

                //navigate to next recno
                m_currentFeatureRecno = *m_currentFeatureRecnoIter++;
                m_currentKey->set_data(&m_currentFeatureRecno);
                //Note m_currentKey.size() is set already by the constructor
                                
                ret = m_dbData->GetFeature(m_currentKey, m_currentData);
            }
            else
            {
				if( m_currentFeatureRecno == 0 )
					ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
				else
					ret = m_dbData->GetNextFeature( m_currentKey, m_currentData );
				
				if( ret == SQLiteDB_OK )
				{
					if( m_currentKey->get_size() == 4 )
						m_currentFeatureRecno = *(REC_NO*)(m_currentKey->get_data());
					else
						m_currentFeatureRecno++;
                    m_currentKey->set_data(&m_currentFeatureRecno); // This is required since m_currentKey was pointing to a memory location that may get re-used/freed
				}
            }

            if (ret == SQLiteDB_NOTFOUND)
                return false;

            if (ret != SQLiteDB_OK)
                throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));

            m_dataReader->Reset((unsigned char*)m_currentData->get_data(), m_currentData->get_size());
            m_dbData->SetTag((void*)this);

            //we can skip the filter if the feature class
            //doesn't match the query base class
            //-- move on to the next feature
            if (!(passedFilter = TestFeatureClassHierarchy()))
                continue;

            
            if (m_filter)
            {
                passedFilter = m_filterExec->ProcessFilter(m_filter);
            }
            
        }
        while(!passedFilter);
    }
    else
    {

        bool passedClassCheck = true;
        do
        {
            int ret = 0;

			if( m_currentFeatureRecno == 0 )
				ret = m_dbData->GetFirstFeature( m_currentKey, m_currentData );
			else
				ret = m_dbData->GetNextFeature( m_currentKey, m_currentData );

            if (ret == SQLiteDB_NOTFOUND)
                return false;

			if( m_currentKey->get_size() == 4 )
				m_currentFeatureRecno = *(REC_NO*)(m_currentKey->get_data());
			else
				m_currentFeatureRecno++;
			m_currentKey->set_data(&m_currentFeatureRecno); // This is required since m_currentKey was pointing to a memory location that may get re-used/freed
            
            if (ret != 0)
                throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_10_ERROR_ACCESSING_SDFDB)));

            m_dataReader->Reset((unsigned char*)m_currentData->get_data(), m_currentData->get_size());
            m_dbData->SetTag((void*)this);
            
            //we can skip the filter if the feature class
            //doesn't match the query base class
            //-- move on to the next feature
            passedClassCheck = TestFeatureClassHierarchy();

        }
        while (!passedClassCheck);

    }

    //strings returned for string property values need to be 
    //available until the following call to ReadNext()
    //This means we store them in a map which we now have to clean up
    //if needed. Note that this is only used for computed properties,
    //attribute properties have their own handling in BinaryReader.
    if (!m_stringPropsCache.empty())
    {
        std::map<std::wstring, wchar_t*>::iterator iter = m_stringPropsCache.begin();

        for (; iter != m_stringPropsCache.end(); iter++)
            delete [] iter->second;

        m_stringPropsCache.clear();
    }

    return true;
}

// Closes the IFeatureReader object freeing any resources it may be
// holding.
void SdfSimpleFeatureReader::Close()
{
    DataDb *dataDb = this->m_connection->GetDataDb(m_class);
    if (dataDb)
	{
		m_dbData->SetTag( (void*) this );
        dataDb->CloseCursor();
	}
    KeyDb *keyDb = this->m_connection->GetKeyDb(m_class);
    if (keyDb)
        keyDb->CloseCursor();
    SdfRTree *rtree = this->m_connection->GetRTree(m_class);
    if (rtree)
        rtree->CloseCursor();
    SchemaDb *schemaDb = this->m_connection->GetSchemaDb();
    if (schemaDb)
        schemaDb->CloseCursor();
}


int SdfSimpleFeatureReader::PositionReader(int recordIndex)
{
    //position where offset to property value is stored
    //remember: we have a 2 byte FCID at the beginning
    //and then offsets for each property
    m_dataReader->SetPosition(sizeof(FCID_STORAGE) + recordIndex * sizeof(int));

    //get offset to property value and position there
    int offset = m_dataReader->ReadInt32();

    //compute how many bytes the
    //property value spans -- we do this by subtracting the two offsets
    int endoffset = 0;

    if (recordIndex < m_propIndex->GetNumProps() - 1)
        endoffset = m_dataReader->ReadInt32();
    else
        endoffset = m_currentData->get_size();

    //skip to the beginning of the data value
    m_dataReader->SetPosition(offset);

    //the length of the data
    //If it is 0, it means the property value is NULL or equal to
    //the default property value (if specified in the schema)
    int len = endoffset - offset;

    return len;
}


bool SdfSimpleFeatureReader::TestFeatureClassHierarchy()
{
    //reset property index to given base class
    m_propIndex = m_basePropIndex;
	unsigned int lastFCID = m_currentFCID;
    //get the FCID for the current feature
    //assumes reader is positioned at its beginning
    m_currentFCID = m_dataReader->ReadUInt16();

	if( lastFCID != m_currentFCID )
	{
		FdoFeatureSchema* schema = m_connection->GetSchema();
		FdoPtr<FdoClassCollection> classes = schema->GetClasses();

		//find property index for current class
		FdoPtr<FdoClassDefinition> clas = classes->GetItem(m_currentFCID);

		//update the class definition for the current feature : 
		//its class may be a child of the class that the
		//select command used to create the FeatureReader
		FDO_SAFE_RELEASE(m_class);
        m_lastClassDefinition = NULL;
		m_class = FDO_SAFE_ADDREF(clas.p);
	}
	
	//make sure the feature class matches the one 
    //which the feature reader was created with,
    //or an inheriting class
    if (m_currentFCID == m_propIndex->GetFCID())
        return true;

    //now make sure that the current fc is a child of the 
    //requested fc
    FdoPtr<FdoClassDefinition> base = FDO_SAFE_ADDREF(m_class);
    
    bool isChild = false;

    while ((base = base->GetBaseClass()) != NULL)
    {
        //navigate up class hierarchy until we reach
        //a parent class which matches the given base FCID
        PropertyIndex* pi = m_connection->GetPropertyIndex(base);
        if (pi->GetFCID() == m_basePropIndex->GetFCID())
        {
            isChild = true;
            //setting the prop index makes sure that the Get...
            //functions know the right feature class structure
            //to read the feature data record
            m_propIndex = m_connection->GetPropertyIndex(m_class);                
            break;
        }
    }

    //returns false if the currentFCID does not match a class
    //which is a child of the given base class, true
    //otherwise
    return isChild;
}


FdoClassDefinition* SdfSimpleFeatureReader::CloneAndPruneClass(FdoClassDefinition *fdoClassDef, FdoIdentifierCollection *idCollection, FdoPropertyDefinitionCollection* computedProps)
{
    FdoPtr<FdoCommonSchemaCopyContext> copyContext;
    if (idCollection != NULL && idCollection->GetCount() > 0)
        copyContext = FdoCommonSchemaCopyContext::Create(idCollection);
    
    // Clone the given class definition:
    FdoClassDefinition* classDefPruned = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(fdoClassDef, copyContext);

    FdoPtr<FdoPropertyDefinitionCollection> properties;
    properties = classDefPruned->GetProperties ();

    if (computedProps != NULL)
    {
        //now add the computed properties to the class
        for (int i=0; i<computedProps->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = computedProps->GetItem(i);
            properties->Add(pd);
        }
    }

    return classDefPruned;
}


void SdfSimpleFeatureReader::CheckIfPropExists(FdoString* name)
{
    //this function will only get called if the property name is
    //not in the actual feature class' properties, meaning that
    //we only need to check if it is a computed property. If we
    //have any computed properties, the pruned class def will
    //be non-null.
    if (!m_classDefPruned)
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));

    //make sure it exists, by looking for it in the
    //property definitions -- all computed idents
    //are added to the property definitions of the 
    //pruned class def, so if it is not there, it's not a 
    //valid property name
    FdoPtr<FdoPropertyDefinitionCollection> props = m_classDefPruned->GetProperties();
    //this will throw exception if the property does not exist
    //it's what we want
    
    FdoPtr<FdoPropertyDefinition> dummy = props->FindItem(name);
    if (dummy == NULL )
        throw FdoCommandException::Create(NlsMsgGetMain(FDO_NLSID(SDFPROVIDER_38_INVALID_PROPERTY_NAME)));
}

void SdfSimpleFeatureReader::RefreshData()
{
	if( !m_dbData || (m_dbData->CurrentDataIsValid() && m_dbData->GetTag() == (void*)this ) )
		return;

	if( m_dbData->GetFeature(m_currentKey, m_currentData) == 0 )
		m_dataReader->Reset((unsigned char*)m_currentData->get_data(), m_currentData->get_size());

	m_dbData->SetTag((void*)this);
}

// Internal getter for the binary data
SQLiteData* SdfSimpleFeatureReader::GetRawData()
{
	return m_currentData;
}

	// Internal getter for the binary key
SQLiteData* SdfSimpleFeatureReader::GetRawKey()
{
	return m_currentKey;
}

