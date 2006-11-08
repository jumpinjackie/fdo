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
#include <ShpFilterExecutor.h>
#include <ShpSchemaUtilities.h>

// Maximum allowed size for the merged list of FeatIds 
// produced by the ShpFeatIdQueryEvaluator optimizer
#define SHP_MAX_FEATID_LIST_SIZE    10000

#define SHP_CACHED_GEOMETRY_INITIAL_SIZE    100 // bytes

class ShpFilterExecutor;
class ShpQueryOptimizer;
class ShpFeatIdQueryTester;
class ShpFeatIdQueryEvaluator;

template <class FDO_READER> class ShpReader :
    public FDO_READER
{
    friend class ShpFilterExecutor;
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
           
    // Query optimizer 
    bool            mFirstRead;         // 1st ReadNext()  
    bool            mUseFeatidMergedList; // Use an unique list of results (merging succeeded)
    bool            mIsFeatIdQuery;     // Filter is FeatId based query
    int             mFeatidQueryIndex;  // Current index in the stack of pre-computed results 

	bool			mFetchGeometry;		// ExtendedSelect doesn't need geometries for indexing data.

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
        mGeomByteArray (FdoByteArray::Create (SHP_CACHED_GEOMETRY_INITIAL_SIZE))
    {
        FDO_SAFE_ADDREF(connection);
        FDO_SAFE_ADDREF (filter);

        // Determine the physical FileSet to use:
        FdoPtr<ShpLpClassDefinition> shpLpClassDef = ShpSchemaUtilities::GetLpClassDefinition(connection, className);
        mFileSet = shpLpClassDef->GetPhysicalFileSet();

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

        columnName = ShpSchemaUtilities::GetPhysicalColumnName (mConnection, mClassName, propertyName);
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
					WCHAR* cpg = NULL;
					if (mFileSet->GetCpgFile())
						cpg = (WCHAR*)(FdoString *)mFileSet->GetCpgFile()->GetCodePage();

					mData->GetData (data, i, type, cpg);
				}
                else
                    throw FdoException::Create (NlsMsgGet(SHP_VALUE_TYPE_MISMATCH, "Value type (%1$ls) to insert, update or retrieve doesn't match the type (%2$ls) of property '%3$ls'.", type_name, ColumnTypeToString (type), propertyName));
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
            ProcessComputedIdentifier (computed);
            bool bIsNull;
            ret = mFilterExecutor->GetBooleanResult (bIsNull);
            mFilterExecutor->Reset ();
            if (bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
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
            ProcessComputedIdentifier (computed);
            bool bIsNull;
            ret = mFilterExecutor->GetDateTimeResult (bIsNull);
            mFilterExecutor->Reset ();
            if (bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
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
            ProcessComputedIdentifier (computed);
            bool bIsNull;
            if (mFilterExecutor->GetResultDataType() == FdoDataType_Double)
                ret = mFilterExecutor->GetDoubleResult (bIsNull);
            else if (mFilterExecutor->GetResultDataType() == FdoDataType_Decimal)
                ret = mFilterExecutor->GetDecimalResult (bIsNull);
            else
                throw FdoException::Create (NlsMsgGet(SHP_VALUE_TYPE_MISMATCH, "Value type (%1$ls) to insert, update or retrieve doesn't match the type (%2$ls) of property '%3$ls'.",
                    FdoCommonMiscUtil::FdoDataTypeToString(FdoDataType_Double), FdoCommonMiscUtil::FdoDataTypeToString(mFilterExecutor->GetResultDataType()), identifier));
            mFilterExecutor->Reset ();
            if (bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
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
            ProcessComputedIdentifier (computed);
            bool bIsNull;
            ret = mFilterExecutor->GetInt32Result (bIsNull);
            mFilterExecutor->Reset ();
            if (bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
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
        FdoInt64 ret;

        id = validate (identifier);
        computed = (id == NULL) ? NULL : dynamic_cast<FdoComputedIdentifier*>(id.p);
        if (NULL != computed)
        {
            ProcessComputedIdentifier (computed);
            bool bIsNull;
            ret = mFilterExecutor->GetInt64Result (bIsNull);
            mFilterExecutor->Reset ();
            if (bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
        }
        else
            throw FdoException::Create (NlsMsgGet(SHP_UNSUPPORTED_DATATYPE, "The '%1$ls' data type is not supported by Shp.", L"Int64"));

        return (ret);
    }

    /// <summary>Gets the Single floating point value of the specified property. No
    /// conversion is performed, thus the property must be FdoDataType_Single
    /// or an exception is thrown.</summary>
    /// <param name="identifier">Input the property name.</param> 
    /// <returns>Returns the single value</returns> 
    virtual float GetSingle (FdoString* identifier)
    {
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
            ProcessComputedIdentifier (computed);
            bool bIsNull;
            ret = mFilterExecutor->GetStringResult (bIsNull);
            mFilterExecutor->Reset ();
            if (bIsNull)
                throw FdoException::Create(NlsMsgGet(SHP_READER_PROPERTY_NULL, "The property '%1$ls' is NULL.", identifier));
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
            ProcessComputedIdentifier (computed);
            ret = mFilterExecutor->IsResultNull();
            mFilterExecutor->Reset ();
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
                ret = (eNullShape == mShape->GetShapeType ());
            // if it's anything else, need to explicitly check:
            else
            {
                FdoString* columnName = ShpSchemaUtilities::GetPhysicalColumnName(mConnection, mClassName, propertyName);
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
                mFeatIdFilterExecutor = ShpFeatIdQueryEvaluator::Create (this, mSelected );
                mFilter->Process (mFeatIdFilterExecutor);
            }
        }

        if ( mIsFeatIdQuery && mFilter )
            ret = ReadNextFeatidQuery();
        else
            ret = ReadNextNonFeatidQuery();

        mFirstRead = false;

        return (ret);
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

    void ProcessComputedIdentifier (FdoComputedIdentifier* computed)
    {
        try
        {
            mCheckSelected = false;
            mFilterExecutor->ProcessIdentifier (*computed);
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

            if (deleted)
                pass = false;
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

            //Results available in lists;
            retno_lists*        intervals = mFeatIdFilterExecutor->GetFeatidLists();
            logical_op_list*    logical_ops = mFeatIdFilterExecutor->GetFeatidFilterOpsList();

            retno_lists::iterator       iter_comp_op = intervals->end();
            logical_op_list::iterator   iter_logical_op = logical_ops->end();
            int                         curr_logical_op = ShpLogicalOperation_None;

            // Evaluate each filter
            for (; iter_comp_op != intervals->begin(); )
            {
                iter_comp_op--;

                interval_res*       curr_filter = *iter_comp_op; // rename
                int                 curr_comp_op = curr_filter->op;
                recno_list*         featid_list = &curr_filter->queryResults;


                // Handle NOT operator and advance 
                if ( curr_logical_op == ShpUnaryLogicalOperation_Not )
                {
                    ret_final = !ret_final;
                    if ( iter_logical_op != logical_ops->begin() )
                        curr_logical_op = (*--iter_logical_op);
                }

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

                // Use current binary logical operation to proceed or not
                switch ( curr_logical_op )
                {
                case FdoBinaryLogicalOperations_And:
                    ret_final = ret_final && ret;
                    break;
                case FdoBinaryLogicalOperations_Or:
                    ret_final = ret_final || ret; 
                    break;
                case ShpLogicalOperation_None:
                    ret_final = ret_final && ret;     
                    break;
                default:
                    throw FdoException::Create (L"Invalid logical operation type");
                }

                // Advance in the list of logical operations.
                if ( iter_logical_op != logical_ops->begin() )
                     curr_logical_op = (*--iter_logical_op);
            }

            if ( curr_logical_op == ShpUnaryLogicalOperation_Not )
                ret_final = !ret_final;

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

            if (deleted || !exists)
                pass = false;
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

            if (deleted || !exists)
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

