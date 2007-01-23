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
#include "ArcSDEUtils.h"


ArcSDEDataReader::ArcSDEDataReader (ArcSDEConnection* connection, FdoClassDefinition* fdoClassDef, FdoFilter* filter, FdoIdentifierCollection* propertiesToSelect,
    bool bDistinct, FdoOrderingOption eOrderingOption, FdoIdentifierCollection* pOrderingIds, FdoFilter* pGroupingFilter, FdoIdentifierCollection* pGroupingIds) :
    ArcSDEReader(connection, fdoClassDef, propertiesToSelect),
    mFilter (filter),
    mGroupingFilter (pGroupingFilter),
    mGroupingIds (pGroupingIds),
    mInitialized (false)
{
    mDistinct = bDistinct;
    mOrderingOption = eOrderingOption;
    mOrderingIds = FDO_SAFE_ADDREF(pOrderingIds);

    FDO_SAFE_ADDREF (mFilter.p);
    FDO_SAFE_ADDREF (mGroupingFilter.p);
    FDO_SAFE_ADDREF (mGroupingIds.p);

    // Validate that propertiesToSelect contains valid entries (or is empty):
    if (propertiesToSelect != NULL)
    {
        bool bContainsAggregateFunctions = false;
        bool bContainsOtherExpressionTypes = false;
        for (FdoInt32 i=0; i<propertiesToSelect->GetCount(); i++)
        {
            FdoPtr<FdoIdentifier> pPropertyId = propertiesToSelect->GetItem(i);

            // Validate this property:
            bool bIsAggregateFunction = false;
            FdoPtr<FdoComputedIdentifier> pComputedId = FDO_SAFE_ADDREF(dynamic_cast<FdoComputedIdentifier*>(pPropertyId.p));
            if (pComputedId == NULL)
            {
                if (!ClassContainsProperty(mClassDef, pPropertyId))
                    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_INVALID_PROPERTY_NAME, "The given property name was invalid."));
            }
            else
            {
                FdoPtr<FdoExpression> computedExpr = pComputedId->GetExpression();
                FdoFunction* computedExprFunc = dynamic_cast<FdoFunction*>(computedExpr.p);
                if (NULL == computedExprFunc)
                    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SELECTAGGREGATES_NON_AGGREGATE_EXPR_NOT_SUPPORTED, "The ArcSDE Provider does not support non-aggregate functions or expresssions in the select aggregates command."));
                if (IsAggregateFunction(computedExprFunc->GetName()))
                    bIsAggregateFunction = true;
                else
                    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SELECTAGGREGATES_NON_AGGREGATE_EXPR_NOT_SUPPORTED, "The ArcSDE Provider does not support non-aggregate functions or expresssions in the select aggregates command."));
            }

            if (bIsAggregateFunction)
                bContainsAggregateFunctions = true;
            else
                bContainsOtherExpressionTypes = true;
        }

        if (bContainsAggregateFunctions)
        {
            if (bContainsOtherExpressionTypes)
                throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SELECTAGGREGATES_MIX_EXPR_NOT_SUPPORTED, "The ArcSDE Provider does not support combining aggregate functions with other identifiers or expresssions in the select aggregates command."));

            mSelectingAggregates = true;
        }
    }

    // Clone the given class definition & prune off the properties the user isn't querying:
    mClassDefPruned = ArcSDESchemaManager::CloneAndPruneClass(fdoClassDef, propertiesToSelect);
}

ArcSDEDataReader::~ArcSDEDataReader (void)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEDataReader::ArcSDEDataReader (const ArcSDEDataReader &right) { }

void ArcSDEDataReader::Dispose ()
{
    delete this;
}


/// <summary>Gets the number of propertys in the result set.</summary>
/// <returns>Returns the number of propertys.</returns> 
FdoInt32 ArcSDEDataReader::GetPropertyCount()
{
    return mColumnCount;
}

/// <summary>Gets the name of the property at the given ordinal position.</summary>
/// <param name="index">Input the position of the property.</param> 
/// <returns>Returns the property name</returns> 
FdoString* ArcSDEDataReader::GetPropertyName(FdoInt32 index)
{
    return ArcSDEReader::getColumnDef((int)index)->mPropertyName;
}

/// <summary>Gets the data type of the property with the specified name.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the type of the property.</returns> 
FdoDataType ArcSDEDataReader::GetDataType(FdoString* propertyName)
{
    // NOTE: this value will be 'undefined' if the FDO property is not a Data property
    return getColumnDef(propertyName)->mPropertyType;
}

/// <summary>Gets the property type of the property with the specified name.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the type of the property.</returns> 
FdoPropertyType ArcSDEDataReader::GetPropertyType(FdoString* propertyName)
{
    if (getColumnDef(propertyName)->mPropertyType == -1)
        return FdoPropertyType_GeometricProperty;
    else
        return FdoPropertyType_DataProperty;
}

/// <summary>Advances the reader to the next item and returns true if there is
/// another object to read or false if reading is complete. The default
/// position of the reader is prior to the first item. Thus you must
/// call ReadNext to begin accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool ArcSDEDataReader::ReadNext ()
{
    PrepareStream ();

    // Call superclass to do the work:
    return (superclass::ReadNext());
}

/// <summary>Sets up the stream ready for a ReadNext</summary>
void ArcSDEDataReader::PrepareStream ()
{
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    FdoPtr<FdoPropertyDefinitionCollection> properties;
    CHAR **columnNames = NULL;
    FdoPtr<FdoPropertyDefinition> fdoPropertyDef;
    LONG result = 0;
    SHORT numSpatialFilters = 0;
    SE_FILTER *pSpatialFilters = NULL;
    FdoPtr<FdoException> exception;
    bool versioned = false;
    CHAR *whereClause = NULL;

    if (!mInitialized)
    {
        try
        {
            // Set SQL query's "from" table list:
            mConnection->ClassToTable(table, mClassDef);

            // Set SQL query's "where" clause * spatial filters:
            GetFilterInfo(mConnection, mFilter, mClassDef, whereClause, numSpatialFilters, pSpatialFilters);

            // Set SQL query's columns to select:
            FdoPtr<FdoIdentifier> propertyId;
            FdoInt32 numProperties = 0;
            properties = mClassDef->GetProperties();
            numProperties = mSelectIds->GetCount();
            columnNames = (CHAR **)alloca (numProperties * sizeof (CHAR *));
            for (int i=0; i<numProperties; i++)
            {
                // Get the property definition of interest:
                propertyId = mSelectIds->GetItem(i);
                if (!mSelectingAggregates) // Aggregate functions are computed ids, not ids of property names
                    fdoPropertyDef = properties->GetItem (propertyId->GetName());

                // Store the column name that corresponds to the property name:
                columnNames[i] = (CHAR*)alloca (SE_MAX_TABLE_LEN * sizeof(CHAR));
                if (mSelectingAggregates)
                {
                FdoComputedIdentifier* computedId = dynamic_cast<FdoComputedIdentifier*>(propertyId.p);
                FdoPtr<FdoExpression> fdoExpr = computedId->GetExpression();
                FdoFunction *fdoFunction = dynamic_cast<FdoFunction*>(fdoExpr.p);
                CHAR *mbName = NULL;
                wide_to_multibyte(mbName, GetAggregateFunctionPropertyName(fdoFunction));
                strcpy(columnNames[i], mbName);
                }
                else
                    mConnection->PropertyToColumn(columnNames[i], mClassDef, FdoPtr<FdoIdentifier>(FdoIdentifier::Create(fdoPropertyDef->GetName())));
            }

            // Initialize the stream query:
            result = SE_stream_create (mConnection->GetConnection (), &mStream);
            handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

            // if necessary, version enable the stream
            versioned = ArcSDELongTransactionUtility::VersionStream (mConnection, mStream, table, false);

            // Execute the query:
            if (mDistinct || mSelectingAggregates)
            {
                // Perform validation:
                if (mDistinct)
                {
                    // Validate number of columns (ArcSDE can only perform distinct on a single column):
                    if (numProperties != 1)
                        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_SELECT_DISTINCT_TOO_MANY_PROPERTIES, "Select with 'distinct' option on %1$d properties failed; ArcSDE supports 'distinct' on only one property at a time.", numProperties));

                    // Validate type of column (ArcSDE can only perform distinct on integer/float/string/date type):
                    propertyId = mSelectIds->GetItem(0);
                    fdoPropertyDef = properties->GetItem (propertyId->GetName());
                    FdoPropertyType propType = fdoPropertyDef->GetPropertyType();
                    if (propType != FdoPropertyType_DataProperty)
                        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SELECT_DISTINCT_INVALID_TYPE, "Select with 'distinct' option failed; ArcSDE doesn't support 'distinct' option with column types other than Int16, Int32, Single, Double, DateTime, and String."));
                    FdoDataPropertyDefinition* dataProp = dynamic_cast<FdoDataPropertyDefinition*>(fdoPropertyDef.p);
                    FdoDataType dataPropType = dataProp->GetDataType();
                    if (   (dataPropType != FdoDataType_Single)  && (dataPropType != FdoDataType_Double)
                        && (dataPropType != FdoDataType_Int16)    && (dataPropType != FdoDataType_Int32)
                        && (dataPropType != FdoDataType_DateTime) && (dataPropType != FdoDataType_String) )
                        throw FdoCommandException::Create(NlsMsgGet(ARCSDE_SELECT_DISTINCT_INVALID_TYPE, "Select with 'distinct' option failed; ArcSDE doesn't support 'distinct' option with column types other than Int16, Int32, Single, Double, DateTime, and String."));
                }

                SE_QUERYINFO query_info;
                result = SE_queryinfo_create(&query_info);
                handle_sde_err<FdoCommandException>(mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_QUERYINFO_MANIPULATION_FAILED, "Unexpected error while manipulating an ArcSDE SE_QUERYINFO object.");

                // Apply the attribute filter to SE_QUERYINFO object:
                ApplyFilterInfoToQueryInfo(mConnection, query_info, table, whereClause, numProperties, (const char**)columnNames, mOrderingOption, mOrderingIds);

                // Apply spatial filters to the stream:
                result = SE_stream_set_spatial_constraints (mStream, SE_OPTIMIZE, FALSE, numSpatialFilters, pSpatialFilters);
                handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

                // Get all requested statistics; will loop more than once only when selecting multiple aggregates at once:
                for (FdoInt32 i=0; i<numProperties; i++)
                {
                    FdoPtr<FdoIdentifier> id = mSelectIds->GetItem(i);
                    ColumnDefinition *col = getColumnDef(id->GetName());

                    LONG lStatsMask = 0L;
                    if (mSelectingAggregates)
                        lStatsMask = GetAggregateSdeStatsMask(GetAggregateFunctionName(id));
                    else // (mDistinct)
                        lStatsMask = SE_DISTINCT_STATS;

                    result = SE_stream_calculate_table_statistics(mStream, (const char*)columnNames[i], lStatsMask, query_info, 0 /*max_distinct*/, &(col->mStreamStats));
                    if (SE_TOO_MANY_DISTINCTS == result)
                        handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_SELECT_DISTINCT_TOO_MANY_RESULTS, "The number of distinct results exceeds the maximum imposed by ArcSDE Server; please update the value of MAXDISTINCT in the ArcSDE Server's GioMgr.defs file accordingly.");
                    else
                        handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_STATISTICS_FAILED, "Calculating stream statistics failed.");
                    // mark the column as not-null
                    col->mBindIsNull = 0;
                }

                // clean up:
                SE_queryinfo_free(query_info);
            }
            else
            {
                // Apply attribute and spatial query to stream:
                ApplyFilterInfoToStream(mConnection, mStream, table, whereClause, numProperties, (const char**)columnNames, numSpatialFilters, pSpatialFilters, mOrderingOption, mOrderingIds);

                // Actually execute the query:
                result = SE_stream_execute (mStream);
                handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");
            }
        }
        catch (FdoException *e)
        {
            exception = e;
        }

        // Clean up:
        delete[] whereClause;
        if (NULL != pSpatialFilters)
        {
            for (int i = 0; i < numSpatialFilters; i++)
                if (NULL != pSpatialFilters[i].filter.shape)
                    SE_shape_free (pSpatialFilters[i].filter.shape);
            delete[] pSpatialFilters;
        }

        // Re-throw exception, if one occurred:
        if (exception != NULL)
        {
            Close();
            throw FDO_SAFE_ADDREF(exception.p);
        }
        mInitialized = true;
    }
}


ArcSDEDataReader::ColumnDefinition* ArcSDEDataReader::createColumnDef (int columnIndex, SE_COLUMN_DEF* columnDef, wchar_t* propertyName, FdoIdentifier *identifier)
{
    return createColumnDef(columnIndex, columnDef->column_name, columnDef->sde_type, propertyName, identifier);
}


ArcSDEDataReader::ColumnDefinition* ArcSDEDataReader::createColumnDef(int columnIndex, CHAR* columnName, LONG columnType, wchar_t* propertyName, FdoIdentifier *identifier)
{
    ColumnDefinition *retColumnDef = new ColumnDefinition();

    // Set column name/number:
    retColumnDef->mColumnNumber = columnIndex;
    wchar_t* wColumnName = NULL;
    multibyte_to_wide(wColumnName, columnName);
    wcscpy(retColumnDef->mColumnName, wColumnName);
    retColumnDef->mColumnType = columnType;

    // Set FDO property name, type & length:
    wcscpy (retColumnDef->mPropertyName, propertyName);
    if (mSelectingAggregates)
    {
        retColumnDef->mPropertyType = SDEType2FdoType(columnType);
        retColumnDef->mIdentifier = FDO_SAFE_ADDREF(identifier);

        // NOTE: we don't set retColumnDef->mDataLength because it's only used for bind variables,
        //       and ArcSDE aggregate functions don't use bind variables.
    }
    else
    {
        FdoPtr<FdoPropertyDefinition> propDef = mConnection->GetProperty(mClassDef, propertyName);
        if (propDef->GetPropertyType() == FdoPropertyType_GeometricProperty)
        {
            retColumnDef->mPropertyType = (FdoDataType)-1;
            retColumnDef->mDataLength = 0;
        }
        else if (propDef->GetPropertyType() == FdoPropertyType_DataProperty)
        {
            FdoDataPropertyDefinition* dataPropDef = dynamic_cast<FdoDataPropertyDefinition*>(propDef.p);  // No AddRef on purpose
            retColumnDef->mPropertyType = dataPropDef->GetDataType();
            retColumnDef->mDataLength = dataPropDef->GetLength();
        }
        else
            throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_UNHANDLED_PROPERTY_TYPE, "The property type '%1$d' is not supported.", (int)propDef->GetPropertyType()));
    }

    // Set remaining items:
    retColumnDef->mBindIsNull = SE_IS_NULL_VALUE;
    retColumnDef->mValuePointer = NULL;
    retColumnDef->mValuePointerSize = 0;
    retColumnDef->mStreamStats = NULL;

    return retColumnDef;
}

// Here, identifier is interpreted as an *FDO property* name **OR** an aggregate function / computed expression
ArcSDEDataReader::ColumnDefinition* ArcSDEDataReader::getColumnDef (const wchar_t* identifier)
{
    ColumnDefinition* columnDef = NULL;

    if (0 == mColumnCount)
        getColumnDefs ();

    for (int i = 0; ((i < mColumnCount) && (NULL == columnDef)); i++)
        if (0 == wcscmp (identifier, mColumnDefs[i]->mPropertyName))
            columnDef = mColumnDefs[i];

    if (NULL==columnDef)
        throw FdoCommandException::Create(NlsMsgGet1(ARCSDE_PROPERTY_NOT_IN_RESULT, "The property '%1$ls' is not in the query result.", identifier));

    return (columnDef);
}

void ArcSDEDataReader::getColumnDefs ()
{
    LONG result = 0L;
    FdoInt32 numProperties = 0;
    SHORT numColumns = 0;
    SE_COLUMN_DEF column;
    FdoPtr<FdoPropertyDefinitionCollection> properties = mClassDef->GetProperties ();

    if (NULL == mColumnDefs)
    {
        // Validate stream:
        if (NULL == mStream)
            throw FdoException::Create (NlsMsgGet(ARCSDE_STREAM_NOT_INITIALIZED, "Stream not initialized."));

        // Determine number of properties to select:
        numProperties = mSelectIds->GetCount ();

        // Determine number of columns we actually received from query result:
        if (mSelectingAggregates)
            numColumns = numProperties;
        else if (mDistinct)
            numColumns = 1; // ArcSDE only supports select distinct on one column at a time
        else
        {
            result = SE_stream_num_result_columns (mStream, &numColumns);
            handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_STREAM_GENERAL_ERROR, "Error encountered while manipulating stream.");
        }

        // Validate count of query's result columns match properties we requested:
        if (numProperties != numColumns)
            throw FdoCommandException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));

        // Populate ColumnDefinition array:
        mColumnDefs = new ColumnDefinition*[numColumns];
        FdoPtr<FdoIdentifier> propertyId;
        FdoPtr<FdoPropertyDefinition> fdoPropertyDef;
        for (int i = 0; i < numColumns; i++)
        {
            // Get the property definition of interest:
            propertyId = mSelectIds->GetItem(i);
            if (!mSelectingAggregates) // Aggregate functions are computed ids, not ids of property names
                fdoPropertyDef = properties->GetItem (propertyId->GetName());

            if (mSelectingAggregates)
            {
                // Get info about the function and its arguments:
                FdoPtr<FdoComputedIdentifier> computedId = FDO_SAFE_ADDREF(dynamic_cast<FdoComputedIdentifier*>(propertyId.p));
                FdoPtr<FdoExpression> fdoExpr = computedId->GetExpression();
                FdoFunction* fdoFunction = dynamic_cast<FdoFunction*>( fdoExpr.p);
                if (fdoFunction == NULL)
                    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));
                FdoString* propName = GetAggregateFunctionPropertyName(fdoFunction);
                FdoPtr<FdoPropertyDefinition> propDef = properties->GetItem(propName);
                FdoDataPropertyDefinition* prop = dynamic_cast<FdoDataPropertyDefinition*>(propDef.p);
                if (prop == NULL) // arcsde aggregates operator only on data properties
                    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));

                // Get the single "column" 's name (as the computed identifier alias):
                CHAR *colName = NULL;
                wide_to_multibyte(colName, computedId->GetName());

                // Get the single "column" 's type (as the resulting type of the function):
                LONG colType = GetFunctionSdeType(mConnection, fdoFunction->GetName());

                // Add a new ColumnDefinition to the array:
                mColumnDefs[i] = createColumnDef(i + 1, colName, colType, (wchar_t*)computedId->GetName(), propertyId);
            }
            else if (mDistinct)
            {
                // Get the single column's name and type:
                CHAR colName[SE_MAX_TABLE_LEN];
                mConnection->PropertyToColumn(colName, mClassDef, FdoPtr<FdoIdentifier>(FdoIdentifier::Create(fdoPropertyDef->GetName())));
                FdoDataPropertyDefinition* dataProp = dynamic_cast<FdoDataPropertyDefinition*>(fdoPropertyDef.p);
                if (dataProp == NULL)
                    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_UNEXPECTED_ERROR, "Unexpected error encountered in ArcSDE Provider."));
                LONG colType = FdoType2SDEType(dataProp->GetDataType());

                // Add a new ColumnDefinition to the array:
                mColumnDefs[i] = createColumnDef(i + 1, colName, colType, (wchar_t*)fdoPropertyDef->GetName ());
            }
            else
            {
                // Get the corresponding column definition of interest:
                result = SE_stream_describe_column (mStream, i + 1, &column);
                handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_STREAM_GET_COLUMN_FAILED, "Failed to retrieve column information from the stream.");

                // Add a new ColumnDefinition to the array:
                mColumnDefs[i] = createColumnDef (i + 1, &column , (wchar_t*)fdoPropertyDef->GetName () );
            }
        }
        mColumnCount = numColumns;
    }
}

