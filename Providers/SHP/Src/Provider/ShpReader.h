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
 */

#ifndef SHPREADER_H
#define SHPREADER_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include <algorithm>

#include <RowData.h>
#include <ShpSchemaUtilities.h>

// Maximum allowed size for the merged list of FeatIds 
// produced by the ShpFeatIdQueryEvaluator optimizer
#define SHP_MAX_FEATID_LIST_SIZE    100000

#define SHP_CACHED_GEOMETRY_INITIAL_SIZE    100 // bytes

#define SHP_DEBUG_PARSE_TREE2   false

class ShpQueryOptimizer;
class ShpFeatIdQueryTester;
class ShpFeatIdQueryEvaluator;

template <class FDO_READER> class ShpReader :
    public FDO_READER
{
    friend class ShpQueryOptimizer;
    friend class ShpFeatIdQueryTester;
    friend class ShpFeatIdQueryEvaluator;
	friend class ShpImpExtendedSelect;
	friend class ShpScrollableFeatureReader;

protected:
    FdoPtr <ShpConnection>              mConnection;
    FdoPtr <FdoFilter>                  mFilter;        // filter to apply to the results
    FdoPtr <FdoIdentifierCollection>    mSelected;      // selected columns
    FdoPtr <ShpQueryOptimizer>          mFilterExecutor; // filter executor (optimized)
    FdoPtr <ShpFeatIdQueryEvaluator>    mFeatIdFilterExecutor; // filter executor (optimized)
    FdoPtr <FdoByteArray>               mGeomByteArray; // cached geometry

    ShpFileSet*     mFileSet;           // access to the files
    FdoInt32        mFeatureNumber;     // current feature number; 0-based internally, 1-based to FDO users
    Shape*          mShape;             // the current shape
    eShapeTypes     mType;              // the type of the current shape
    RowData*        mData;              // dbf record for current shape
    bool            mCheckSelected;     // if true, check access is only to selected properties, i.e. client call
    FdoStringP      mClassName;
    FdoStringP      mLogicalIdentityPropertyName;
    FdoStringP      mLogicalGeometryPropertyName;
	int				mMaxNumObjects;		

#ifdef _WIN32
    ULONG           mCodePage;
#else
    const char*     mCodePage;
#endif

    FdoPtr<ShpLpClassDefinition> mLpClass; //cached LP class
           
    // Query optimizer 
    bool            mFirstRead;         // 1st ReadNext()  
    bool            mUseFeatidMergedList; // Use an unique list of results (merging succeeded)
    bool            mIsFeatIdQuery;     // Filter is FeatId based query
    int             mFeatidQueryIndex;  // Current index in the stack of pre-computed results 

	bool			mFetchGeometry;		// ExtendedSelect doesn't need geometries for indexing data.
	bool			mFetchDeletes;		// ExtendedSelect needs fetching the deleted rows to use the featid as index.
    std::map<std::wstring, wchar_t*> m_stringPropsCache; 

protected:
    ShpReader () {}; // to satisfy _NoAddRefReleaseOnFdoPtr 

public:
    ShpReader (ShpConnection *connection, FdoString *className, FdoFilter* filter, FdoIdentifierCollection* selected) :
        mClassName(className),
        mFilter (filter),
        mFeatureNumber (-1),
        mShape (NULL),
        mType (eNullShape),
        mData (NULL),
        mConnection(connection),
        mCheckSelected (true),
        mFirstRead(true),
        mIsFeatIdQuery(true),
        mUseFeatidMergedList(true),
		mFetchGeometry (true),
		mFetchDeletes (false),
		mMaxNumObjects (0),
        mGeomByteArray (FdoByteArray::Create (SHP_CACHED_GEOMETRY_INITIAL_SIZE))
    {
        FDO_SAFE_ADDREF(connection);
        FDO_SAFE_ADDREF (filter);

        // Determine the physical FileSet to use:
        mLpClass = ShpSchemaUtilities::GetLpClassDefinition(connection, className);
        mFileSet = mLpClass->GetPhysicalFileSet();

        // Determine the name of the singleton identity property and singleton geometry property:
        FdoPtr<FdoClassDefinition> classDef = ShpSchemaUtilities::GetLogicalClassDefinition(mConnection, mClassName, NULL);
        FdoPtr<FdoDataPropertyDefinitionCollection> idProps = classDef->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> idProp = idProps->GetItem(0);
        mLogicalIdentityPropertyName = idProp->GetName();
        
        // Validate the class type:
        FdoClassType classType = classDef->GetClassType();
        if ( (classType != FdoClassType_FeatureClass) && (classType != FdoClassType_Class) ) {
            throw FdoException::Create (
                NlsMsgGet(SHP_UNSUPPORTED_CLASSTYPE, "The '%1$ls' class type is not supported by Shp.", 
                    FdoCommonMiscUtil::FdoClassTypeToString(classType)));
        }

        // Grab the single geometry property, if any:
        if (classType == FdoClassType_FeatureClass)
        {
            FdoFeatureClass* featureClassDef = static_cast<FdoFeatureClass*>(classDef.p);
            FdoPtr<FdoGeometricPropertyDefinition> geomProp = featureClassDef->GetGeometryProperty();
            if (geomProp != NULL)
                mLogicalGeometryPropertyName = geomProp->GetName();
        }

        // Cache the identity property list, if any is given; otherwise ignore it:
        if ((NULL != selected) && (0 != selected->GetCount ()))
            mSelected = FDO_SAFE_ADDREF (selected);

        mFilterExecutor = ShpQueryOptimizer::Create (this, selected );


		// Get the code page from LDID. If not valid try the .CPG file.
		FdoStringP  codePage = L"";
		if(mFileSet->GetCpgFile())
			codePage = mFileSet->GetCpgFile()->GetCodePage();

		if (codePage == L"")
			codePage = mFileSet->GetDbfFile()->GetCodePage();

        //Cache the OS-specific code page ID
        ShapeCPG cpg;
#ifdef _WIN32
        mCodePage = cpg.ConvertCodePageWin((WCHAR*)(FdoString*)codePage);
#else
        mCodePage = cpg.ConvertCodePageLinux((WCHAR*)(FdoString*)codePage);
#endif
    }

    virtual ~ShpReader (void)
    {
        if ( mData )
            delete mData;
        Close ();
    }

    virtual void Dispose ()
    {
        delete this;
    }

    // Prevent the use of the Assignment Operation by defining it and not implementing it.
    // DO NOT IMPLEMENT
    ShpReader & operator= (const ShpReader &right);

    /// <summary>Gets data from the row buffer.</summary>
    /// <param name="data">Input the location to store the data.</param> 
    /// <param name="propertyName">Input the property name.</param> 
    /// <param name="column_type">Input the type to expect.</param> 
    /// <param name="type_name">Input the descriptive type name.</param> 
    /// <returns>Returns nothing.</returns> 
    void GetData (ColumnData* data, FdoString* propertyName, eDBFColumnType column_type, FdoString* type_name)
    {
        FdoString* columnName;
        ColumnInfo* info;
        int count;
        eDBFColumnType type;

        FdoPtr<ShpLpPropertyDefinitionCollection> lpProperties = mLpClass->GetLpProperties();
        FdoPtr<ShpLpPropertyDefinition> lpProperty = lpProperties->GetItem(propertyName);
        columnName = lpProperty->GetPhysicalColumnName();

        info = mData->GetColumnInfo ();
        count = info->GetNumColumns ();
        type = kColumnUnsupportedType;
        for (int i = 0; i < count; i++)
        {
            if (0 == wcscmp (columnName, info->GetColumnNameAt (i)))
            {
                type = info->GetColumnTypeAt (i);
                if (type == column_type)
				{
					mData->GetData (data, i, type, mCodePage);
				}
                else
                    throw FdoException::Create (NlsMsgGet(SHP_VALUE_TYPE_MISMATCH, "Value type to insert, update or retrieve doesn't match the type (%1$ls) of property '%2$ls'.", ColumnTypeToString (type), propertyName));
                break;
            }
        }
        if (kColumnUnsupportedType == type)
            throw FdoException::Create (NlsMsgGet(SHP_COMMAND_PROPERTY_NOT_FOUND, "Property '%1$ls' is not part of class '%2$ls'.", propertyName, (FdoString*)mClassName));
    }

    //
    // FdoIFeatureReader interface
    //

    /// <summary>Gets the Boolean value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Boolean or an 
    /// exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the Boolean value.</returns> 
    virtual bool GetBoolean (const FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        bool ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Boolean)
				{
					FdoBooleanValue *booleanValue = static_cast<FdoBooleanValue *>(dataValue);
					if (booleanValue->IsNull())
		                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
					return booleanValue->GetBoolean();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
        {
            ColumnData coldata;
            GetData (&coldata, identifier, kColumnLogicalType, L"FdoBoolean");
            if (coldata.bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
            else
                ret = coldata.value.bData;
        }

        return (ret);
    }

    /// <summary>Gets the byte value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_Byte or an 
    /// exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the byte value.</returns> 
    virtual FdoByte GetByte (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Byte)
				{
					FdoByteValue *byteValue = static_cast<FdoByteValue *>(dataValue);
					if (byteValue->IsNull())
		                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
					return byteValue->GetByte();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"Byte"));
    }

    /// <summary> Gets the date and time value of the specified property. No conversion is 
    /// performed, thus the property must be FdoDataType_DateTime or an 
    /// exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the date and time value.</returns> 
    virtual FdoDateTime GetDateTime (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        FdoDateTime ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_DateTime)
				{
					FdoDateTimeValue *dateTimeValue = static_cast<FdoDateTimeValue *>(dataValue);
					if (dateTimeValue->IsNull())
		                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
					return dateTimeValue->GetDateTime();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
		}
        else
        {
            ColumnData coldata;
            GetData (&coldata, identifier, kColumnDateType, L"FdoDateTime");
            if (coldata.bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
            else
            {
                FdoDateTime t ((FdoInt16)coldata.value.tData.nYear, (FdoInt8)coldata.value.tData.nMonth, (FdoInt8)coldata.value.tData.nDay);
                ret = t;
            }
        }

        return (ret);
    }

    /// <summary>Gets the double-precision floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Double
    /// or an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the double floating point value</returns> 
    virtual double GetDouble (FdoString* identifier)
    {
        // NOTE: we currently only support Decimal (not Double) property types,
        //       and Double or Decimal computed expressions.

        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        double ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
			FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Double)
				{
					FdoDoubleValue *doubleValue = static_cast<FdoDoubleValue *>(dataValue);
					return doubleValue->GetDouble();
				}
				else
					if (dataValue->GetDataType() == FdoDataType_Decimal)
					{
						FdoDecimalValue *decimalValue = static_cast<FdoDecimalValue *>(dataValue);
						if (decimalValue->IsNull())
							throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
						return decimalValue->GetDecimal();
					}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
        {
            ColumnData coldata;
            GetData (&coldata, identifier, kColumnDecimalType, L"FdoDecimal");
            if (coldata.bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
            else
                ret = coldata.value.dData;
        }

        return (ret);
    }

    /// <summary>Gets the 16-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int16 or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the FdoInt16 value.</returns> 
    virtual FdoInt16 GetInt16 (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Int16)
				{
					FdoInt16Value *int16Value = static_cast<FdoInt16Value *>(dataValue);
					if (int16Value->IsNull())
		                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
					return int16Value->GetInt16();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"Int16"));
    }

    /// <summary>Gets the 32-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int32 or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the FdoInt32 value</returns> 
    virtual FdoInt32 GetInt32 (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        FdoInt32 ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Int32)
				{
					FdoInt32Value *int32Value = static_cast<FdoInt32Value *>(dataValue);
					if (int32Value->IsNull())
		                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));

					return int32Value->GetInt32();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else 
        {
            // The identity property is a special property (it doesn't have a 
            // corresponding physical column), so handle it specially:
            if (0 == wcscmp (identifier, mLogicalIdentityPropertyName))
            {
                // Return the row number as the singleton identity property's value:
                ret = mFeatureNumber + 1;  // featis are 0-based internally, but 1-based externally (to FDO users)
            }
            else
            {
                ColumnData coldata;
                GetData (&coldata, identifier, kColumnDecimalType, L"FdoInt32");
                if (coldata.bIsNull)
                    throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
                else
                    ret = (FdoInt32)coldata.value.dData;
            }
        }

        return (ret);
    }

    /// <summary>Gets the 64-bit integer value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_Int64 or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the FdoInt64 value.</returns> 
    virtual FdoInt64 GetInt64 (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Int64)
				{
					FdoInt64Value *int64Value = static_cast<FdoInt64Value *>(dataValue);
					if (int64Value->IsNull())
		                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
					return int64Value->GetInt64();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"Int64"));

    }

    /// <summary>Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the single value</returns> 
    virtual float GetSingle (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
			FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_Single)
				{
					FdoSingleValue *singleValue = static_cast<FdoSingleValue *>(dataValue);
					return singleValue->GetSingle();
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"Single"));
    }

    /// <summary>Gets the string value of the specified property. No conversion is
    /// performed, thus the property must be FdoDataType_String or an exception
    /// is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the string value</returns> 
    virtual FdoString* GetString (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        FdoString* ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            if (m_stringPropsCache[identifier])
                return m_stringPropsCache[identifier];

            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoStringValue *dataValue = static_cast<FdoStringValue *> (results.p);
				if (dataValue->GetDataType() == FdoDataType_String)
				{
					FdoStringValue *stringValue = static_cast<FdoStringValue *>(dataValue);
					if (stringValue->IsNull())
						throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
				    wchar_t* ret = new wchar_t[wcslen(stringValue->GetString())+1];
				    wcscpy(ret, stringValue->GetString());
                    m_stringPropsCache[identifier] = ret;
					return ret;
				}
			}
            throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
        }
        else
        {
            ColumnData coldata;
            GetData (&coldata, identifier, kColumnCharType, L"FdoString");
            if (coldata.bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
            else
                ret = coldata.value.wszData;
        }

        return (ret);
    }


    /// <summary>Gets a LOBValue reference. The LOB is fully read in and data available.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB or
    /// FdoDataType_CLOB etc. (a LOB type)
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the reference to LOBValue</returns> 
    virtual FdoLOBValue* GetLOB(FdoString* identifier)
    {
        throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"LOB"));
    }

    /// <summary>Gets a reference of the specified LOB property as a FdoBLOBStreamReader or
    /// FdoCLOBStreamReader etc. to allow reading in blocks of data.
    /// Because no conversion is performed, the property must be FdoDataType_BLOB 
    /// or FdoDataType_CLOB etc. (a LOB type)
    /// Cast the FdoIStreamReader to the appropiate LOB Stream Reader.
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns a reference to a LOB stream reader</returns> 
    virtual FdoIStreamReader* GetLOBStreamReader(FdoString* identifier)
    {
        throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"LOB"));
    }

    /// <summary>Returns true if the value of the specified property is null.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns true if the value is null.</returns> 
    virtual bool IsNull (FdoString* propertyName)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        ColumnInfo* info;
        int count;
        bool ret;

        id = validate (propertyName);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            FdoPtr<FdoLiteralValue> results = ProcessComputedIdentifier (computed);
			if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
			{
				FdoDataValue *dataValue = static_cast<FdoDataValue *> (results.p);
				return dataValue->IsNull();

			}
			else
			{
				if (results->GetLiteralValueType() == FdoLiteralValueType_Data)
				{
					FdoGeometryValue *geometryValue = static_cast<FdoGeometryValue *> (results.p);
					return geometryValue->IsNull();
					
				}
                throw FdoException::Create (NlsMsgGet(SHP_INVALID_LITERAL_TYPE, "Invalid literal type '%1$d'.", results->GetLiteralValueType()));
			}
        }
        else
        {
            info = mData->GetColumnInfo ();
            count = info->GetNumColumns ();
            // if it's the singleton identity property, it's always non-null:
            if (0 == wcscmp (propertyName, mLogicalIdentityPropertyName))
                ret = false;
            // if it's the singleton geometry property, it's null if the shape is NullShape:
            else if (0 == wcscmp (propertyName, mLogicalGeometryPropertyName))
            {
                if(!mShape)                
                    ret = true;
                else
                    ret = (eNullShape == mShape->GetShapeType ());
            }
            // if it's anything else, need to explicitly check:
            else
            {
                FdoPtr<ShpLpPropertyDefinitionCollection> lpProperties = mLpClass->GetLpProperties();
                FdoPtr<ShpLpPropertyDefinition> lpProperty = lpProperties->GetItem(propertyName);
                FdoString* columnName = lpProperty->GetPhysicalColumnName();

                ret = true;
                for (int i = 0; ret && (i < count); i++)
                {
                    if (0 == wcscmp (columnName, info->GetColumnNameAt (i)))
                    {
                        ColumnData coldata;
                        GetData (&coldata, propertyName, info->GetColumnTypeAt(i), ColumnTypeToString(info->GetColumnTypeAt(i)));
                        ret = coldata.bIsNull;
                        break;
                    }

                    // If property not found, throw exception:
                    if (i == count-1)
                        throw FdoException::Create(NlsMsgGet(SHP_COMMAND_PROPERTY_NOT_FOUND, "Property '%1$ls' is not part of class '%2$ls'.", propertyName, (FdoString*)mClassName));
                }
            }
        }

        return (ret);
    }

    /// <summary>Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the byte array in FGF format.</returns> 
    virtual FdoByteArray* GetGeometry (FdoString* identifier)
    {
        FdoPtr<FdoIdentifier> id;
        FdoComputedIdentifier* computed;
        FdoByteArray* ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {   //hmmm, the result stack is FdoDataValue, and geometry is a second class citizen
            //ProcessComputedIdentifier (computed);
            //ret = mFilterExecutor->GetGeometryResult ();
            //mFilterExecutor->Reset ();

            FdoPtr<FdoExpression> expression = computed->GetExpression ();
            // it's got to be an identifier called "Geometry"
            id = dynamic_cast<FdoIdentifier*>(expression.p);
            if (id == NULL)
                throw FdoException::Create (NlsMsgGet(SHP_COMMAND_PROPERTY_NOT_FOUND, "Property '%1$ls' is not part of class '%2$ls'.", identifier, (FdoString*)mClassName));
            else
                identifier = id->GetName ();
        }

        // Since geometry property is a 'special case' (there is no geometry column in the DBF file),
        // we need to validate the geometry property name:
        if (0 != wcscmp (identifier, mLogicalGeometryPropertyName))
            throw FdoException::Create (NlsMsgGet(SHP_COMMAND_PROPERTY_NOT_FOUND, "Property '%1$ls' is not part of class '%2$ls'.", identifier, (FdoString*)mClassName));

        ret = GetGeometryLocal();

        return (ret);
    }

    /// <summary>Gets the geometry value of the specified property as a byte array in 
    /// FGF format. Because no conversion is performed, the property must be
    /// of Geometric type; otherwise, an exception is thrown. 
    /// This method is a language-specific performance optimization that returns a
    /// pointer to the array data, rather than to an object that encapsulates
    /// the array.  The array's memory area is only guaranteed to be valid
    /// until a call to ReadNext() or Close(), or the disposal of this reader
    /// object.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <param name="count">Output the number of bytes in the array.</param> 
    /// <returns>Returns a pointer to the byte array in FGF format.</returns> 
    virtual const FdoByte* GetGeometry (FdoString* identifier, FdoInt32 * count)
    {
        FdoPtr<FdoByteArray> geometry;

        geometry = GetGeometry (identifier);
        *count = geometry->GetCount ();

        return (geometry->GetData ());
    }

    /// <summary>Gets the raster object of the specified property.
    /// Because no conversion is performed, the property must be
    /// of Raster type; otherwise, an exception is thrown.</summary>
    /// <param name="propertyName">Input the property name.</param> 
    /// <returns>Returns the raster object.</returns> 
    virtual FdoIRaster* GetRaster(FdoString* identifier)
    {
        throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"Raster"));
    }

    virtual void SetFetchGeometry (bool fetchGeometry)
    {
		mFetchGeometry = fetchGeometry;
	}

    virtual void SetFetchDeletes (bool fetchDeletes)
    {
		mFetchDeletes = fetchDeletes;
	}

    /// <summary>Advances the reader to the next item and returns true if there is
    /// another object to read or false if reading is complete. The default
    /// position of the reader is prior to the first item. Thus you must
    /// call ReadNext to begin accessing any data.</summary>
    /// <returns>Returns true if there is a next item.</returns> 
    virtual bool ReadNext ()
    {
        bool    ret;

        if ( mFirstRead && mFilter )
        {
            FdoPtr<ShpFeatIdQueryTester>  featidTester = ShpFeatIdQueryTester::Create( this );

            mFilter->Process ( featidTester );
            mIsFeatIdQuery = featidTester->IsFeatidQuery();

            if ( mIsFeatIdQuery )
            {
                mFeatIdFilterExecutor = ShpFeatIdQueryEvaluator::Create (this, mSelected);
				mFilter->Process (mFeatIdFilterExecutor);
            }
        }

        if ( mIsFeatIdQuery && mFilter )
            ret = ReadNextFeatidQuery();
        else
            ret = ReadNextNonFeatidQuery();

        mFirstRead = false;


        ClearCidStringCache();

        return (ret);
    }

	void ClearCidStringCache()
	{
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
	}

    /// <summary>Closes the Reader object, freeing any resources it may be holding.</summary>
    /// <returns>Returns nothing</returns> 
    virtual void Close ()
    {
        if (NULL != mShape)
        {
            delete mShape;
            mShape = NULL;
        }
    }

protected:
    FdoIdentifier* validate (FdoString* identifier = NULL)
    {
        FdoIdentifier* ret;

        ret = NULL;

        if (0 > mFeatureNumber)
            throw FdoException::Create(NlsMsgGet(SHP_READER_NOT_READY, "'%1$ls' reader not ready.", L"FDO_READER"));
        if (mFeatureNumber >= mFileSet->GetShapeIndexFile ()->GetNumObjects ())
            throw FdoException::Create(NlsMsgGet(SHP_READER_EXHAUSTED, "'%1$ls' reader exhausted.", L"FDO_READER"));
        if (NULL != identifier)
            if ((mSelected != NULL) && (0 != mSelected->GetCount ()))
            {
                ret = mSelected->FindItem (identifier);
                if ( mCheckSelected && (ret == NULL))
                    throw FdoException::Create (FdoException::NLSGetMessage(FDO_NLSID(FDO_104_READER_PROPERTY_NOT_SELECTED), identifier));
            }

        return (ret);
    }

    FdoString* ColumnTypeToString (eDBFColumnType type)
    {
        FdoString* ret;

        switch (type)
        {
            case kColumnCharType:
                ret = L"CHAR";
                break;
            case kColumnDecimalType:
                ret = L"DECIMAL";
                break;
            case kColumnDateType:
                ret = L"DATE";
                break;
            case kColumnLogicalType:
                ret = L"LOGICAL";
                break;
            case kColumnUnsupportedType:
            default:
                ret = L"Unsupported";
                break;
        }

        return (ret);
    }

    ShpConnection* GetConnection ()
    {
        return (FDO_SAFE_ADDREF (mConnection.p));
    }

    FdoString* GetClassName() const
    {
        return mClassName;
    }

    FdoLiteralValue *ProcessComputedIdentifier (FdoComputedIdentifier* computed)
    {
        try
        {
            mCheckSelected = false;
            FdoLiteralValue *results = mFilterExecutor->Evaluate(*computed);
            return (FdoLiteralValue *) results;
        }
        catch (...)
        {
            mCheckSelected = true;
            throw;
        }
    }

private:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool ReadNextNonFeatidQuery()
    {
        bool pass;
        bool deleted;
        bool ret;

        do
        {
            if (NULL != mShape)
            {
                delete mShape;
                mShape = NULL;
            }
            if (NULL != mData)
            {
                delete mData;
                mData = NULL;
            }
            mFeatureNumber++;
            deleted = false;
            ret = false;
            if (mFeatureNumber < mFileSet->GetShapeIndexFile ()->GetNumObjects ())
            {
				mFileSet->GetObjectAt (&mData, mType, mFetchGeometry? &mShape : NULL, mFeatureNumber);
                deleted = mData->IsDeleted ();
                ret = true;
            }

            if (deleted && !mFetchDeletes)
			{
                pass = false;
			}
            else
                if (ret && mFilter != NULL)
                {
                    try
                    {
                        mCheckSelected = false;
                        mFilter->Process (mFilterExecutor);
                    }
                    catch (FdoException* e)
                    {
                        mCheckSelected = true;
                        throw e;
                    }
                    mCheckSelected = true;
                    bool bIsNull;
                    pass = mFilterExecutor->GetBooleanResult (bIsNull);
                    if (bIsNull)
                        pass = false;
                    mFilterExecutor->Reset ();
                }
                else
                    pass = true;
        }
        while (!pass);

        return (ret);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool ReadNextFeatidQueryNotMerged()
    {
        bool            pass = false;
        bool            deleted;
		bool			exists;
        bool            ret_final;

        FdoInt32        maxRecords = mFileSet->GetShapeIndexFile ()->GetNumObjects ();

        do
        {
            bool    ret = false;
            ret_final = true;

            if (NULL != mShape)
            {
                delete mShape;
                mShape = NULL;
            }
            if (NULL != mData)
            {
                delete mData;
                mData = NULL;
            }
            mFeatureNumber++;

            if ( mFeatureNumber == maxRecords )
            {
                ret_final = false;
                break;
            }

            // Evaluate the expresion for the current feature
            retno_lists*        featidLists = mFeatIdFilterExecutor->GetFeatidLists();
            logical_op_list*    logicalOpsList = mFeatIdFilterExecutor->GetFeatidFilterOpsList();
            left_right_op_list* leftRightOpsList = mFeatIdFilterExecutor->GetLeftRightOpsList();

	        long	last_leaf_index = (long)featidLists->size() - 1;
	        bool	isSimpleExpr = (leftRightOpsList->size() == 0);
	        bool	first_leaf = true;

            std::vector<bool> tmp_stack;

	        // Find the beginnig of the leaf. Start from the deepest one.
	        for ( ; last_leaf_index >= 0; last_leaf_index-- ) 
	        {
		        interval_res*		curr_filter = featidLists->at( last_leaf_index );
		        int                 curr_comp_op = curr_filter->op;
		        recno_list*         featid_list = &curr_filter->queryResults;
		        int					depth = curr_filter->depth;
        		
		        if ( isSimpleExpr || leftRightOpsList->at(depth) == 0 ) // Left
		        {	
			        if (SHP_DEBUG_PARSE_TREE2)
				        printf("--Process expression depth %d\n", depth );

			        // Process the current leaf expression
			        for ( size_t i = last_leaf_index; i < featidLists->size(); i++ )
			        {
				        interval_res*		curr_filter2 = featidLists->at(i);
				        int					depth2 = curr_filter2->depth;

				        // Done when hitting a Left leaf already processed
				        if ( i != last_leaf_index && ( !isSimpleExpr && leftRightOpsList->at(depth2) == 0 ) )
					        break;

				        if (SHP_DEBUG_PARSE_TREE2) {
					        if ( !isSimpleExpr )
						        printf("\tProcess node depth %d: logicalOp=%d  (%c)\n", depth2, logicalOpsList->at(depth2),
														        leftRightOpsList->at(depth2) == 0 ? 'L' : 'R');
				        }

				        int	 curr_logical_op = ( (i == last_leaf_index) || isSimpleExpr ) ?
											        ShpLogicalOperation_None : logicalOpsList->at(depth2);

				        ProcessLeafExpession( curr_filter2, curr_logical_op, &tmp_stack );
			        }

			        // Use current binary logical operation to merge the current leaf result to the global list
			        if ( first_leaf )
			        {
                        // Do nothing, the value is on the stack
			        }
			        else
			        {
				        // Extract the Logical operation between 2 leafs
				        int		depth3 = ( last_leaf_index == 0 )?  0 : depth-1;

				        switch ( logicalOpsList->at(depth3))
				        {
				        case FdoBinaryLogicalOperations_And:
					        {
                                bool    ret1 = tmp_stack.back();
                                tmp_stack.pop_back();
                                bool    ret2 = tmp_stack.back();                       
                                tmp_stack.pop_back();
                                tmp_stack.push_back( ret1 && ret2 );
						        break;
					        }
				        case FdoBinaryLogicalOperations_Or:
					        {
                                bool    ret1 = tmp_stack.back();
                                tmp_stack.pop_back();
                                bool    ret2 = tmp_stack.back();
                                tmp_stack.pop_back();
                                tmp_stack.push_back( ret1 || ret2 );
						        break;
					        }
                        case ShpLogicalOperation_None:
                            {
                                break;
                            }          
				        default:
					        throw FdoException::Create (L"Invalid logical operation type");
				        }
			        }
        		
			        first_leaf = false;
		        }
	        }

            // Get the boolean result
            ret_final = tmp_stack.back();
            tmp_stack.pop_back();

            // Go and fetch the row
            deleted = false;
			exists = true;
            if ( ret_final )
            {
                mFileSet->GetObjectAt (&mData, mType, mFetchGeometry? &mShape : NULL, mFeatureNumber);
				
				exists = ( mData != NULL );

				if ( exists )
					deleted = mData->IsDeleted ();

                ret = true;
            }

            if ((deleted && !mFetchDeletes) || !exists)
			{
                pass = false;
			}
            else
                if ( ret_final )
                {
                    mCheckSelected = true; // CHECK ??
                    pass = true;
                }
        }
        while (!pass);

        return (ret_final);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    void ProcessLeafExpession( interval_res* curr_filter, int curr_logical_op, std::vector<bool> *bvals_stack )
    {
        int                 curr_comp_op = curr_filter->op;
        recno_list*         featid_list = &curr_filter->queryResults;
        bool                ret = false;

        // The result set may be empty (say a spatial query returning nothing)
        if ( featid_list->size() == 0 )
        {
            ret = false;
        }
        else
        {
            FdoInt32    featId = (*featid_list->begin());

            switch ( curr_comp_op )
            {
            case FdoComparisonOperations_EqualTo : 
                ret = ( mFeatureNumber == featId );
                break;
            case FdoComparisonOperations_NotEqualTo : 
                ret = ( mFeatureNumber != featId );
                break;
            case FdoComparisonOperations_GreaterThan : 
                ret = ( mFeatureNumber > featId );
                break;
            case FdoComparisonOperations_GreaterThanOrEqualTo : 
                ret = ( mFeatureNumber >= featId );
                break;
            case FdoComparisonOperations_LessThan : 
                ret = ( mFeatureNumber < featId );
                break;
            case FdoComparisonOperations_LessThanOrEqualTo : 
                ret = ( mFeatureNumber <= featId );
                break;
            case ShpComparisonOperation_In : // In or Spatial query candidates
                ret = std::binary_search( featid_list->begin( ), featid_list->end( ), mFeatureNumber, std::less<FdoInt32>() );
                break;
            case FdoComparisonOperations_Like :
            default:
                throw FdoException::Create (L"Invalid comparison operation type");
            }
        }

       // Use current binary logical operation to merge the current list

        switch ( curr_logical_op )
	    {
	    case FdoBinaryLogicalOperations_And:
		    {
                bool    ret1 = bvals_stack->back();
                bvals_stack->pop_back();
                bvals_stack->push_back( ret1 && ret );
			    break;
		    }
	    case FdoBinaryLogicalOperations_Or:
		    {
                bool    ret1 = bvals_stack->back();
                bvals_stack->pop_back();
                bvals_stack->push_back( ret1 || ret );
			    break;
		    }
        case ShpLogicalOperation_None:
            {
                bvals_stack->push_back( ret );     
                break;
            }          
	    default:
		    throw FdoException::Create (L"Invalid logical operation type");
	    }

        logical_op_list*    logicalOpsList = mFeatIdFilterExecutor->GetFeatidFilterOpsList();
	    if ( (logicalOpsList->size() != 0 ) && logicalOpsList->at(curr_filter->depth) == ShpUnaryLogicalOperation_Not )
        {
            bool    ret1 = bvals_stack->back();
            bvals_stack->pop_back();
            bvals_stack->push_back( !ret1 );
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool ReadNextFeatidQueryMerged()
    {
        bool            pass = false;
        bool            deleted;
		bool			exists;
        bool            ret_final;

        do
        {
            ret_final = true;

            if (NULL != mShape)
            {
                delete mShape;
                mShape = NULL;
            }
            if (NULL != mData)
            {
                delete mData;
                mData = NULL;
            }

            recno_list*    featid_list = mFeatIdFilterExecutor->GetMergedFeatidList();

			if ( featid_list )
				mMaxNumObjects = (int) featid_list->size();

            // Quit if already at the end of the list. This covers an empty list.
            if ( featid_list == NULL || mFeatidQueryIndex == featid_list->size() )
            {
                ret_final = false;
                break;
            }
            
            // Get current record #
            mFeatureNumber = featid_list->at(mFeatidQueryIndex);

            // Go and fetch the row
            deleted = false;
			exists = true;
            if ( ret_final )
            {
                mFileSet->GetObjectAt (&mData, mType, mFetchGeometry? &mShape : NULL, mFeatureNumber);

				exists = ( mData != NULL );

				if ( exists )
					deleted = mData->IsDeleted ();
            }

            if ( (deleted && !mFetchDeletes) || !exists)
                pass = false;
            else
                if ( ret_final )
                {
                    mCheckSelected = true; 
                    pass = true;
                }

            // Advance in the list of results for the next read
            mFeatidQueryIndex++;
        }
        while (!pass);

        return (ret_final);
    }


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool ReadNextFeatidQuery()
    {
        bool            pass = false;
        bool            ret;

        FdoInt32        numRecords = mFileSet->GetShapeIndexFile()->GetNumObjects();

		mMaxNumObjects = numRecords;

        // Try to merge the result lists into one unique list.
        if ( mFirstRead )
        {
            mUseFeatidMergedList = mFeatIdFilterExecutor->MergeFeatidLists( SHP_MAX_FEATID_LIST_SIZE, numRecords );
            mFeatidQueryIndex = 0;
        }

        if ( mUseFeatidMergedList )
            ret = ReadNextFeatidQueryMerged();
        else
            ret = ReadNextFeatidQueryNotMerged();

        return (ret);
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    FdoByteArray * GetGeometryLocal()
    {
        FdoInt32        dimensionality = GetFdoGeomDimensionality();
        eShapeTypes     shpGeomType = mShape->GetShapeType();
        bool            relateRings = false;

        // Needs FDO utilities when relating rings
        if ( shpGeomType == ePolygonShape )
        {
            PolyShape *pPoly = (PolyShape *)mShape;
            if ( pPoly->GetNumParts() > 1 && RELATE_RINGS )
                relateRings = true;
        }

        // Create geometry through FDO for certain cases not covered.
        if ( (dimensionality & FdoDimensionality_Z) != 0 ||
             (dimensionality & FdoDimensionality_M) != 0 ||
             shpGeomType == eNullShape ||
             shpGeomType == eMultiPatchShape ||
             relateRings )
        {
            mGeomByteArray = mShape->GetGeometry();
        }
        else
        {
            // check if we own the cached byte array
			FdoInt32	refCount = mGeomByteArray ? mGeomByteArray->GetRefCount() : 0;

            // create a new cache
            if (1 != refCount)
                mGeomByteArray = FdoByteArray::Create (SHP_CACHED_GEOMETRY_INITIAL_SIZE);

            mGeomByteArray.p = CreateCachedGeometry (mGeomByteArray.p);
        }

        return (FDO_SAFE_ADDREF (mGeomByteArray.p));
    }

    //////////////////////////////////////////////////////////////////////////////////////////
    FdoByteArray * CreateCachedGeometry(FdoByteArray* fgfByteArray_I)
    {
        FdoByteArray*   fgfByteArray = fgfByteArray_I; // it may be reallocated
        int             count;
        FdoInt32        numBytesForOrdinates = 0;
        FdoInt32        numParts = 0;
        FdoGeometryType fdoGeomType = GetFdoGeometryType();
        FdoGeometryType fdoGeomType2 = fdoGeomType;
        FdoInt32        dimensionality = GetFdoGeomDimensionality();
        FdoInt32        numPointOrds = GetFdoGeometryNumOrds();
        FdoInt32        numPositions = mShape->GetNumPoints();

        fgfByteArray->Clear();

        fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(fdoGeomType), (FdoByte *) &fdoGeomType);
        
        switch (fdoGeomType)
        {
        case FdoGeometryType_None:
            break;

	    case FdoGeometryType_Point:
            {
                PointShape *pPoint = (PointShape *)mShape;
                numBytesForOrdinates = numPointOrds * sizeof(double) * 1; 
                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
                fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, (FdoByte *)pPoint->GetPoints());
            }
		    break;
        case FdoGeometryType_LineString:
            {
                PolylineShape *pLine = (PolylineShape *)mShape;
                count = pLine->GetNumPoints();
                numBytesForOrdinates = numPointOrds * sizeof(double) * count; 
                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(count), (FdoByte *)&count);
                fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, (FdoByte *)pLine->GetPoints());
             }
            break;

	    case FdoGeometryType_Polygon:
            {
                PolyShape *pPoly = (PolyShape *)mShape;
                numParts = pPoly->GetNumParts();
                count = pPoly->GetNumPoints();
                numBytesForOrdinates = numPointOrds * sizeof(double) * count;

                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);              
                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numParts), (FdoByte *) &numParts);
 
                int *pParts = pPoly->GetParts ();

                for (FdoInt32 i = 0; i < numParts; i++)
                {
                    count = (i + 1 < numParts) ? pParts[i + 1] : pPoly->GetNumPoints ();
                    count -= pPoly->GetParts ()[i];
                    numBytesForOrdinates = numPointOrds * sizeof(double) * count;
                    DoublePoint* points = &pPoly->GetPoints ()[pParts[i]];

                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(count), (FdoByte *)&count);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, (FdoByte *)points );
                }
            }
		    break;

	    case FdoGeometryType_MultiPoint:
            {
                fdoGeomType2 = FdoGeometryType_Point;
                MultiPointShape *pPoints = (MultiPointShape *)mShape;
                numBytesForOrdinates = numPointOrds * sizeof(double) * 1; 
                numParts = pPoints->GetNumPoints();
       
                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numParts), (FdoByte *) &numParts);

                for (FdoInt32 i = 0; i < numParts; i++)
                {
                    DoublePoint* point = &pPoints->GetPoints ()[i];
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(fdoGeomType2), (FdoByte *) &fdoGeomType2);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, (FdoByte *)point );
                }
            }
            break;

            /* // This code is wrong (doesnt output the number of rings (1) per polygon), and
               // never gets called anyway, so we comment it out for now.
	        case FdoGeometryType_MultiPolygon:
            {
                PolyShape   *pPoly = (PolyShape *)mShape;
                numParts = pPoly->GetNumParts();
                fdoGeomType2 = FdoGeometryType_Polygon;

                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numParts), (FdoByte *) &numParts);
                int *pParts = pPoly->GetParts ();

                for (FdoInt32 i = 0; i < numParts; i++)
                {
                    count = (i + 1 < numParts) ? pParts[i + 1] : pPoly->GetNumPoints ();
                    count -= pPoly->GetParts ()[i];
                    numBytesForOrdinates = numPointOrds * sizeof(double) * count;
                    DoublePoint* points = &pPoly->GetPoints ()[pParts[i]];

                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(fdoGeomType2), (FdoByte *) &fdoGeomType2);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(count), (FdoByte *)&count);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, (FdoByte *)points );
                }
            }
            break;
            */

	    case FdoGeometryType_MultiLineString:
            {
                PolylineShape *pLine = (PolylineShape *)mShape;
                numParts = pLine->GetNumParts();
                fdoGeomType2 = FdoGeometryType_LineString;
                
                fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(numParts), (FdoByte *) &numParts);
                int *pParts = pLine->GetParts ();

                for ( int i = 0; i < numParts; i++ )
                {
                    count = (i + 1 < numParts) ? pParts[i + 1] : pLine->GetNumPoints ();
                    count -= pLine->GetParts ()[i];
                    numBytesForOrdinates = numPointOrds * sizeof(double) * count;
                    DoublePoint* points = &pLine->GetPoints ()[pParts[i]];
                    
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(fdoGeomType2), (FdoByte *) &fdoGeomType2);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(dimensionality), (FdoByte *) &dimensionality);  
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, sizeof(count), (FdoByte *)&count);
                    fgfByteArray = FdoByteArray::Append(fgfByteArray, numBytesForOrdinates, (FdoByte *)points);
                }
            }
            break;

	    case FdoGeometryType_MultiGeometry:
	    default:
            throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_10_UNSUPPORTEDGEOMETRYTYPE)));
        }

	    return (fgfByteArray);
    }

    /////////////////////////////////////////////////////////////////////////////////
    FdoGeometryType GetFdoGeometryType()
    {
        FdoGeometryType fdoType = FdoGeometryType_None;

        switch ( mShape->GetShapeType())
        {
        case eNullShape:
            fdoType = FdoGeometryType_None;
            break;

        case ePointShape:
        case ePointZShape:
        case ePointMShape:
            fdoType = FdoGeometryType_Point;
            break;

        case ePolylineShape:
        case ePolylineZShape:
        case ePolylineMShape:
            {
                fdoType = FdoGeometryType_LineString;
                PolylineShape *pLine = (PolylineShape *)mShape;
                if (pLine->GetNumParts() > 1 )
                    fdoType = FdoGeometryType_MultiLineString;
            }
            break;

        case ePolygonShape:
        case ePolygonZShape:
        case ePolygonMShape:
        case eMultiPatchShape:
            fdoType = FdoGeometryType_Polygon;
            break;

        case eMultiPointShape:
        case eMultiPointZShape:
        case eMultiPointMShape:
            fdoType = FdoGeometryType_MultiPoint;
            break;
        default:
            ;
        }

        return fdoType;
    }

    /////////////////////////////////////////////////////////////////////////////////
    FdoInt32 GetFdoGeomDimensionality()
    {
        FdoInt32 fdoDim = FdoDimensionality_XY;

        switch ( mShape->GetShapeType())
        {
        case eNullShape:
        case ePointShape:
        case ePolylineShape:
        case ePolygonShape:
        case eMultiPointShape:
        case eMultiPatchShape:
            break;

        case ePointZShape:
        case ePolylineZShape:
        case ePolygonZShape:
        case eMultiPointZShape:
            fdoDim |= FdoDimensionality_Z;
            break;

        case ePointMShape:
        case ePolylineMShape:
        case ePolygonMShape:
        case eMultiPointMShape:
            fdoDim |= FdoDimensionality_M;
            break;

        default:
            ;
        }

        // Check for M 
        if ( mFileSet->GetShapeFile ()->HasMData () )
            fdoDim |= FdoDimensionality_M;
           
        return fdoDim;
    }

    /////////////////////////////////////////////////////////////////////////////////
    int GetFdoGeometryNumOrds()
    {
        int numOrd = 2;

        FdoInt32 fdoDim = GetFdoGeomDimensionality();

        if ( (fdoDim & FdoDimensionality_Z) != 0 )
            numOrd++;

        if ( (fdoDim & FdoDimensionality_M) != 0 )
            numOrd++;

        return numOrd;
    }

};

#endif // SHPREADER_H

