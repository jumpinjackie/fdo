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
#include "ArcSDEFilterToSql.h"

ArcSDEFeatureReader::ArcSDEFeatureReader (ArcSDEConnection* connection, FdoClassDefinition* fdoClassDef, FdoFilter* filter, FdoIdentifierCollection* propertiesToSelect) :
    ArcSDEReader(connection, fdoClassDef, propertiesToSelect),
    mFilter (filter),
    mConflictReader (NULL),
    mLockStrategy (FdoLockStrategy_All),
    mLockType (FdoLockType_None),
    mInitialized (false),
    mLog (0)
{
    FDO_SAFE_ADDREF (mFilter.p);

    // Validate that mSelectIds contains valid entries. Computed identifiers are supported.
    for (FdoInt32 i=0; i<mSelectIds->GetCount(); i++)
    {
        FdoPtr<FdoIdentifier> pPropertyId = mSelectIds->GetItem(i);
		FdoComputedIdentifier* pComputedId = dynamic_cast<FdoComputedIdentifier*>(pPropertyId.p);

        if ((pComputedId == NULL ) && !ClassContainsProperty(mClassDef, pPropertyId))
            throw FdoCommandException::Create(NlsMsgGet(ARCSDE_INVALID_PROPERTY_NAME, "The given property name was invalid."));
	}

    // Clone the given class definition & prune off the properties the user isn't querying:
    mClassDefPruned = ArcSDESchemaManager::CloneAndPruneClass(fdoClassDef, mSelectIds);
}

ArcSDEFeatureReader::~ArcSDEFeatureReader (void)
{
    Close ();
}

/** Do not implement the copy constructor. **/
//ArcSDEFeatureReader::ArcSDEFeatureReader (const ArcSDEFeatureReader &right) { }

void ArcSDEFeatureReader::Dispose ()
{
    delete this;
}

/// <summary>Gets the definition of the object currently being read. If the user
/// has requested only a subset of the class properties, the class 
/// definition reflects what the user has asked, rather than the full class 
/// definition.</summary>
/// <returns>Returns the class definition object.</returns> 
FdoClassDefinition* ArcSDEFeatureReader::GetClassDefinition ()
{
#ifdef MIMIC_OTHER_PROVIDER
    // SPR 613159 Can not access class definition without calling ReadNext()
    // We allow accessing the class definition prior at any time.
#else
    if (mRowNotValidated)
        validate ();
#endif // MIMIC_OTHER_PROVIDER
    return (FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(mClassDefPruned));
}

/// <summary>Gets a value indicating the depth of nesting for the current reader.
/// The depth value increases each time GetFeatureObject is called and a new 
/// reader is returned. The outermost reader has a depth of 0.</summary>
/// <returns>Returns the depth</returns> 
FdoInt32 ArcSDEFeatureReader::GetDepth ()
{
    return (0);
}

/// <summary>Gets a reference to an FdoIFeatureReader to read the data contained in
/// the object or object collection property. If the property is not an
/// object property, an exception is thrown.</summary>
/// <param name="propertyName">Input the property name.</param> 
/// <returns>Returns the nested feature reader</returns> 
FdoIFeatureReader* ArcSDEFeatureReader::GetFeatureObject (FdoString* propertyName)
{
    throw FdoCommandException::Create(NlsMsgGet(ARCSDE_OBJECT_PROPERTIES_NOT_SUPPORTED, "Object properties are not supported."));
}

/// <summary>Advances the reader to the next item and returns true if there is
/// another object to read or false if reading is complete. The default
/// position of the reader is prior to the first item. Thus you must
/// call ReadNext to begin accessing any data.</summary>
/// <returns>Returns true if there is a next item.</returns> 
bool ArcSDEFeatureReader::ReadNext ()
{
    PrepareStream ();

    // Call superclass:
    return (superclass::ReadNext());
}

/// <summary>Validate the computed identifiers wrt the natively supported functions</summary>
bool ArcSDEFeatureReader::ContainsSDEValidExpressionsOnly (bool& filterValid, bool& selectListValid)
{
	// Validate the filter
	FdoPtr<ArcSDEFilterToSql> f2s = new ArcSDEFilterToSql (mConnection, mClassDef);

	return f2s->ContainsSDEValidExpressionsOnly( mFilter, mSelectIds, filterValid, selectListValid );
}


/// <summary>Sets up the stream ready for a ReadNext</summary>
void ArcSDEFeatureReader::PrepareStream ()
{
    CHAR *whereClause = NULL;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    FdoPtr<FdoPropertyDefinitionCollection> properties;
    CHAR **columnNames = NULL;
    FdoPtr<FdoPropertyDefinition> fdoPropertyDef;
    LONG result = 0;
    SHORT numSpatialFilters = 0;
    SE_FILTER *pSpatialFilters = NULL;
    FdoPtr<FdoException> exception;
    const CHAR* id_column[1];
    CHAR column[SE_MAX_COLUMN_LEN];
    bool versioned;
    SE_SQL_CONSTRUCT  sql_construct;
    CHAR* tables[1];
    FdoPtr<FdoISQLDataReader> reader;
    CHAR logfile[SE_MAX_PATH_LEN];

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
            properties = mClassDef->GetProperties ();
            FdoInt32 numProperties = mSelectIds->GetCount();
            columnNames = (CHAR **)alloca (numProperties * sizeof (CHAR *));

            for (int i=0; i<numProperties; i++)
            {
                // Get the property definition of interest:
                propertyId = mSelectIds->GetItem(i);

				FdoComputedIdentifier* pComputedId = dynamic_cast<FdoComputedIdentifier*>(propertyId.p);
            
				// Special handling for computed identifiers in the select list. 
				// The entire function(expresion) stands for the column name.
				if (pComputedId)
				{
					FdoPtr<FdoExpression> computedExpr = pComputedId->GetExpression();
					FdoFunction* fdoFunction = dynamic_cast<FdoFunction*>(computedExpr.p);

					// Get the SQL
					FdoPtr<ArcSDEFilterToSql> f2s = new ArcSDEFilterToSql (mConnection, mClassDef);
					f2s->ProcessFunction (*fdoFunction);

					FdoStringP	func = f2s->GetSql (); // volatile, since memory is on stack
					FdoStringP	func2 = func.Right(L"WHERE "); // trim

					CHAR *mbName = NULL;
					sde_wide_to_multibyte (mbName, (FdoString *) func2);  
					
					columnNames[i] = mbName;				

				}
				else
				{
					fdoPropertyDef = properties->GetItem (propertyId->GetName());

					// Store the column name that corresponds to the property name:
					FdoPtr<ArcSDEPropertyMapping> propertyMapping = mConnection->GetPropertyMapping(mClassDef, fdoPropertyDef->GetName());
					CHAR *columnName = NULL;
					if (wcslen(propertyMapping->GetColumnName()) > 0)
					{
						sde_wide_to_multibyte(columnName, propertyMapping->GetColumnName());
						columnNames[i] = columnName;
					}
					else
					{
						sde_wide_to_multibyte(columnName, fdoPropertyDef->GetName());
						columnNames[i] = columnName;
					}
				}
            }

            // Initialize the stream query:
            result = SE_stream_create (mConnection->GetConnection (), &mStream);
            handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_ALLOC, "Cannot initialize SE_STREAM structure.");

            // if necessary, version enable the stream
            versioned = ArcSDELongTransactionUtility::VersionStream (mConnection, mStream, table, false);

            // if necessary, apply locking to stream (or turn it off if the table isn't lockable)
            if (FdoLockType_None != GetLockType ())
            {
                if (ArcSDELockUtility::IsLockable (mConnection->GetConnection (), table, column))
                {
                    // set up stuff we need repeatedly
                    id_column[0] = column;
                    tables[0] = table;
                    sql_construct.tables = tables;
                    sql_construct.num_tables = ELEMENTS (tables);
                    sql_construct.where = NULL;

                    result = SE_stream_set_rowlocking (mStream, SE_ROWLOCKING_LOCK_ON_QUERY);
                    handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

                    // apply attribute and spatial query to stream
                    ApplyFilterInfoToStream (mConnection, mStream, table, whereClause, 1, id_column, numSpatialFilters, pSpatialFilters);

                    // set up a temporary log file
                    ArcSDELockUtility::MakeLog (&mLog, mConnection->GetConnection (), table);

                    // accumulate the query in the log file
                    result = SE_stream_set_logfile (mStream, mLog, FALSE);
                    handle_sde_err<FdoCommandException> (mStream, result, __FILE__, __LINE__, ARCSDE_LOG_SET_LOGFILE, "Could not set log file.");

                    // lock the table's lock table to prevent alteration
                    reader = ArcSDELockUtility::LockLockTable (mConnection, table);
                }
                else
                {
                    // table not lockable, switch off locking
                    // TODO: should we throw an exception?
                    SetLockType (FdoLockType_None);

                    // apply attribute and spatial query to stream
                    ApplyFilterInfoToStream(mConnection, mStream, table, whereClause, numProperties, (const CHAR**)columnNames, numSpatialFilters, pSpatialFilters);
                }
            }
            else
            {
                // Apply attribute and spatial query to stream:
                ApplyFilterInfoToStream(mConnection, mStream, table, whereClause, numProperties, (const CHAR**)columnNames, numSpatialFilters, pSpatialFilters);
            }

            // Actually execute the query:
            result = SE_stream_execute (mStream);
            handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

            if (FdoLockType_None != GetLockType ())
            {
                result = SE_stream_fetch (mStream);
                // two possibilities: everything was locked (SE_FINISHED) or conflict (SE_LOCK_CONFLICT)
                switch (result)
                {
                    case SE_FINISHED:
                        break;
                    case SE_LOCK_CONFLICT:
                        {

                            // if there were conflicts, query against the log file for rows
                            // not locked by the current user and add them to the conflict reader
                            // ToDo: examine the possibility of having the lock conflict reader take a stream instead

                            // reuse the same stream
                            result = SE_stream_close (mStream, TRUE);

                            // if necessary, version enable the stream
                            if (versioned)
                                ArcSDELongTransactionUtility::VersionStream (mConnection, mStream, table, false);

                            // query the rows locked by somebody else
                            result = SE_stream_set_rowlocking (mStream, SE_ROWLOCKING_FILTER_OTHER_LOCKS);
                            handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

                            // get the list of row ids from the log file
                            ArcSDELockUtility::GetLogFile (logfile, mConnection->GetConnection (), mLog);
                            result = SE_stream_query_logfile (mStream, logfile, 1, id_column, &sql_construct);
                            handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_LOG_FILE_QUERY, "Unable to query log file.");

                            // execute the query that returns other-user-locked rows
                            result = SE_stream_execute (mStream);
                            handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

                            // put the row ids into the conflict list
                            ArcSDELockUtility::GatherConflicts (mStream, column, 1, mConflictReader);

                            // if there were conflicts and the lock strategy was 'partial'
                            // execute a query against the log file to lock the ones we can
                            if (FdoLockStrategy_Partial == GetLockStrategy ())
                            {
                                // reuse the same stream
                                result = SE_stream_close (mStream, TRUE);

                                // if necessary, version enable the stream
                                if (versioned)
                                    ArcSDELongTransactionUtility::VersionStream (mConnection, mStream, table, false);

                                // lock the unlocked rows
                                result = SE_stream_set_rowlocking (mStream, SE_ROWLOCKING_LOCK_ON_QUERY | SE_ROWLOCKING_FILTER_UNLOCKED);
                                handle_sde_err<FdoCommandException> (mConnection->GetConnection (), result, __FILE__, __LINE__, ARCSDE_STREAM_LOCK, "Cannot set row locking on the stream.");

                                // get the list of row ids from the log file
                                result = SE_stream_query_logfile (mStream, logfile, 1, id_column, &sql_construct);
                                handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_LOG_FILE_QUERY, "Unable to query log file.");

                                // execute the query that locks rows
                                result = SE_stream_execute (mStream);
                                handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

                                // ToDo: do we need this with a transaction lock?:
                                result = SE_stream_fetch (mStream);
                                handle_sde_err<FdoCommandException> (mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
                            }
                        }
                        break;
                    default:
                        handle_sde_err<FdoCommandException> (mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_FETCH, "Stream fetch failed.");
                }

                // finally, set up the real stream for the superclass
                // this is every feature in the log file, but with the requested columns (properties)

                // reuse the same stream
                result = SE_stream_close (mStream, TRUE);

                // if necessary, version enable the stream
                if (versioned)
                    ArcSDELongTransactionUtility::VersionStream (mConnection, mStream, table, false);

                // get the list of row ids from the log file, but include all requested columns in the query
                ArcSDELockUtility::GetLogFile (logfile, mConnection->GetConnection (), mLog);
                result = SE_stream_query_logfile (mStream, logfile, numProperties, (const CHAR**)columnNames, &sql_construct);
                handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_LOG_FILE_QUERY, "Unable to query log file.");

                // execute the query that returns full rows
                result = SE_stream_execute (mStream);
                handle_sde_err<FdoCommandException>(mStream, result, __FILE__, __LINE__, ARCSDE_STREAM_EXECUTE, "Stream execute failed.");

                // release the transaction lock
                if (reader != NULL)
                    reader->Close ();
            }
        }
        catch (FdoException *e)
        {
            exception = e;
        }

        // Clean up:
        if (NULL != whereClause)
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

void ArcSDEFeatureReader::Close ()
{
    LONG result;

    if (mConflictReader != NULL)
    {
        mConflictReader->SetFeatureReader (NULL);
        mConflictReader->Release ();
        mConflictReader = NULL;
    }
    if (0 != mLog)
    {
        result = SE_log_close (mConnection->GetConnection (), mLog);
        mLog = 0;
        handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_LOG_FILE_CLOSE, "Unable to close log file.");
    }
    superclass::Close ();
}

ArcSDEFeatureReader::ColumnDefinition* ArcSDEFeatureReader::createColumnDef (int columnIndex, SE_COLUMN_DEF* columnDef, wchar_t* propertyName, wchar_t* functionName)
{
    ColumnDefinition *retColumnDef = new ColumnDefinition();

    // Set column name/number:
    retColumnDef->mColumnNumber = columnIndex;
    wchar_t* wColumnName = NULL;
    sde_multibyte_to_wide(wColumnName, columnDef->column_name);
    wcscpy(retColumnDef->mColumnName, wColumnName);
    retColumnDef->mColumnType = columnDef->sde_type;

    // Set FDO property name, type & length:
    wcscpy (retColumnDef->mPropertyName, propertyName);

	if ( functionName )
	{
		FdoPtr<FdoIExpressionCapabilities> expressCaps = mConnection->GetExpressionCapabilities();
		FdoPtr<FdoFunctionDefinitionCollection> funcDefs = expressCaps->GetFunctions();

		FdoPtr<FdoFunctionDefinition>	funcDef = funcDefs->FindItem(functionName);
		FdoFunctionCategoryType category = funcDef->GetFunctionCategoryType();

		retColumnDef->mDataLength = 0;
		if ( category == FdoFunctionCategoryType_Geometry )
			retColumnDef->mPropertyType = (FdoDataType)-1;
		else if ( category == FdoFunctionCategoryType_String )
			retColumnDef->mPropertyType = FdoDataType_String;
		else if ( category == FdoFunctionCategoryType_Date )
			retColumnDef->mPropertyType = FdoDataType_DateTime;
		else
		{
			retColumnDef->mPropertyType = FdoDataType_Double;
			retColumnDef->mDataLength = sizeof(double);
		}
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

    return retColumnDef;
}

// Here, identifier is interpreted as an *FDO property* name
ArcSDEFeatureReader::ColumnDefinition* ArcSDEFeatureReader::getColumnDef (const wchar_t* identifier)
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

void ArcSDEFeatureReader::getColumnDefs ()
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
        numProperties = mSelectIds->GetCount();

        // Determine number of columns we actually received from query result:
        result = SE_stream_num_result_columns (mStream, &numColumns);
        handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_STREAM_GENERAL_ERROR, "Error encountered while manipulating stream.");


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
			
			FdoComputedIdentifier* pComputedId = dynamic_cast<FdoComputedIdentifier*>(propertyId.p);
            
			// Get the name of the selected id. Special handling for computed identifiers.
			FdoString*	propName = NULL;
			FdoString*	funcName = NULL;
			if (pComputedId)
			{
				propName = propertyId->GetName();
				FdoPtr<FdoExpression> computedExpr = pComputedId->GetExpression();
				FdoFunction* fdoFunction = dynamic_cast<FdoFunction*>(computedExpr.p);
				funcName = fdoFunction->GetName();
			}
			else
			{
				fdoPropertyDef = properties->GetItem (propertyId->GetName());
				propName = fdoPropertyDef->GetName();
			}

            // Get the corresponding column definition of interest:
            result = SE_stream_describe_column (mStream, i + 1, &column);
            handle_sde_err<FdoCommandException>(result, __FILE__, __LINE__, ARCSDE_STREAM_GET_COLUMN_FAILED, "Failed to retrieve column information from the stream.");

            // Add a new ColumnDefinition to the array:
            mColumnDefs[i] = createColumnDef (i + 1, &column , (wchar_t *)propName, (wchar_t *)funcName );
        }
        mColumnCount = numColumns;
    }
}

void ArcSDEFeatureReader::SetLockConflictReader (ArcSDELockConflictReader* conflicts)
{
    mConflictReader = conflicts;
    if (NULL != mConflictReader)
    {
        // the feature reader holds a soft reference to the lock conflict reader
        // the lock conflict reader holds a hard reference to the feature reader
        mConflictReader->AddRef ();
        mConflictReader->SetFeatureReader (this);
    }
}

ArcSDELockConflictReader* ArcSDEFeatureReader::GetLockConflictReader ()
{
    return (FDO_SAFE_ADDREF (mConflictReader));
}

void ArcSDEFeatureReader::SetLockStrategy (FdoLockStrategy strategy)
{
    mLockStrategy = strategy;
}

FdoLockStrategy ArcSDEFeatureReader::GetLockStrategy ()
{
    return (mLockStrategy);
}

void ArcSDEFeatureReader::SetLockType (FdoLockType type)
{
    mLockType = type;
    if ((FdoLockType_None != type) && (mConflictReader == NULL))
        throw FdoException::Create (NlsMsgGet1(ARCSDE_UNEXPECTED_ERROR_ARG, "Unexpected error encountered in ArcSDE Provider (%1$ls).", L"need a conflict reader"));
}

FdoLockType ArcSDEFeatureReader::GetLockType ()
{
    return (mLockType);
}



