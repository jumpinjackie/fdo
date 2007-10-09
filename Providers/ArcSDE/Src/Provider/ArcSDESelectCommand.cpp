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
#include "ArcSDESelectCommand.h"
#include "ArcSDEFeatureReader.h"
#include "Util/FdoExpressionEngineUtilFeatureReader.h"
#include "Util/FdoExpressionEngineUtilDataReader.h"
#include <FdoExpressionEngine.h>
#include "ArcSDEFunctionArea2D.h"
#include "ArcSDEFunctionLength2D.h"

ArcSDESelectCommand::ArcSDESelectCommand (FdoIConnection *connection) :
    ArcSDEFeatureCommand<FdoISelect> (connection),
    mType (FdoLockType_None),
    mStrategy (FdoLockStrategy_All),
    mConflictReader (NULL)
{
}

/** Do not implement the copy constructor. **/
//ArcSDESelectCommand::ArcSDESelectCommand (const ArcSDESelectCommand &right) { }

ArcSDESelectCommand::~ArcSDESelectCommand (void)
{
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* ArcSDESelectCommand::GetPropertyNames ()
{
    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    if (mPropertiesToSelect == NULL)
        mPropertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType ArcSDESelectCommand::GetLockType ()
{
    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    return (mType);
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDESelectCommand::SetLockType (FdoLockType value)
{
    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    switch (value)
    {
        case FdoLockType_None:
            // Indicates no lock should be applied to objects. 
            break;
        case FdoLockType_Exclusive:
            // Indicates an exclusive lock type.
            break;
        case FdoLockType_LongTransactionExclusive:
            // Indicates a long transacation exclusive lock type.
            throw FdoCommandException::Create (NlsMsgGet (ARCSDE_LONG_TRANSACTION_EXCLUSIVE_LOCKS_NOT_SUPPORTED, "Lock type FdoLockType_LongTransactionExclusive is not supported."));
        default:
            throw FdoCommandException::Create (NlsMsgGet1 (ARCSDE_INVALID_LOCK_TYPE, "Invalid lock type ('%1$d').", (int)value));
    }
    mType = value;
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy ArcSDESelectCommand::GetLockStrategy ()
{
    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    return (mStrategy);
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void ArcSDESelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    switch (value)
    {
        case FdoLockStrategy_All:
            // Declares that either all of the locks must be acquired or none should be acquired.
            break;
        case FdoLockStrategy_Partial:
            // Declares that all of the locks that can be acquired should be acquired.
            break;
        default:
            throw FdoCommandException::Create (NlsMsgGet1 (ARCSDE_INVALID_LOCK_STRATEGY, "Invalid lock strategy ('%1$d').", (int)value));
    }
    mStrategy = value;
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* ArcSDESelectCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    FdoPtr<FdoClassDefinition> definition;
    FdoPtr<FdoFilter> filter;
    FdoPtr<ArcSDEFeatureReader> ret;

    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // Get the class definition which reflects the requested feature class name
    definition = connection->GetRequestedClassDefinition (mClassName);

    // get any filtering
    filter = GetFilter ();

    // Validate that there are no aggregate functions:
    FdoCommonExpressionType exprType;

	FdoPtr<FdoIExpressionCapabilities> expressCaps = connection->GetExpressionCapabilities();
	FdoPtr<FdoFunctionDefinitionCollection> funcDefs = expressCaps->GetFunctions();
    FdoPtr< FdoArray<FdoFunction*> > functions = FdoExpressionEngineUtilDataReader::GetAggregateFunctions(funcDefs, mPropertiesToSelect, exprType);
    if (exprType == FdoCommonExpressionType_Aggregate)
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_182_AGGREGATE_IN_SELECT, "Aggregate functions are not supported by the Select command; use the SelectAggregates command instead."));


	// Create the feature reader
	ret = new ArcSDEFeatureReader (connection, definition, filter, mPropertiesToSelect); 

	bool filterValid, selectListValid;
	if ( ret->ContainsSDEValidExpressionsOnly (filterValid, selectListValid) )
	{
#ifdef MIMIC_OTHER_PROVIDER
		// SPR 612054 SelectCommand error reporting inconsistency with the Oracle Provider when using invalid property name in filter.
		// we call prepare stream to discover errors now, rather than at ReadNext() time
		ret->PrepareStream ();
		return (FDO_SAFE_ADDREF (ret.p));

#endif // MIMIC_OTHER_PROVIDER
	}
	else
	{
		// Create the collection of custom functions.
		FdoPtr<FdoExpressionEngineFunctionCollection> userDefinedFunctions = GetUserDefinedFunctions( connection, definition );

		FdoPtr<ArcSDEFeatureReader> sub_reader;
		FdoPtr<FdoIFeatureReader>	reader;

		// Delegate processing to the Expression engine.
		// TODO: add userDefinedFunctions for AREA2D() and LENGTH2D()
		if ( filterValid )
		{
			// CASE 1: the filter is supported. Create a new sub-reader over all properties with the filter.
			sub_reader = new ArcSDEFeatureReader (connection, definition, filter, NULL); 
			reader = new FdoExpressionEngineUtilFeatureReader (definition, sub_reader, NULL, mPropertiesToSelect, userDefinedFunctions);
		}
		else
		{
			// CASE 2: the filter is not supported. Create a new sub-reader with no filtering over all properties.
			sub_reader = new ArcSDEFeatureReader (connection, definition, NULL, NULL); 
			reader = new FdoExpressionEngineUtilFeatureReader (definition, sub_reader, filter, mPropertiesToSelect, userDefinedFunctions);
		}

		return FDO_SAFE_ADDREF(reader.p);	
	}
}

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* ArcSDESelectCommand::ExecuteWithLock ()
{
    FdoPtr<ArcSDEConnection> connection;
    FdoPtr<FdoClassDefinition> definition;
    FdoPtr<FdoFilter> filter;
    CHAR table[SE_QUALIFIED_TABLE_NAME];
    CHAR column[SE_MAX_COLUMN_LEN];
    wchar_t* wcolumn;
    FdoString* property;
    FdoPtr<ArcSDELockConflictReader> conflicts;
    FdoPtr<ArcSDEFeatureReader> ret;

    // Need to do a release here, in case someone is re-using this select command:
    FDO_SAFE_RELEASE(mConflictReader); // for execute with locking, this is the lock conflict reader returned by GetLockConflicts()

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    // verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_FEATURE_CLASS_UNSPECIFIED, "Feature class name not specified."));

    // Get the class definition which reflects the requested feature class name
    definition = connection->GetRequestedClassDefinition (mClassName);

    // get any filtering
    filter = GetFilter ();

    // get SQL query's "from" table list
    mConnection->ClassToTable (table, definition);

    // ensure lockable table
    ArcSDELockUtility::LockEnable (connection->GetConnection (), table, column);

    // get the property name that is the row_id
    multibyte_to_wide (wcolumn, column);
    property = connection->ColumnToProperty (definition, wcolumn);

    // establish an empty conflict reader
    conflicts = new ArcSDELockConflictReader (connection, definition->GetQualifiedName (), table, property);
    conflicts->SetSelectCommand (this);

    // create the feature reader
    ret = new ArcSDEFeatureReader (connection, definition, filter, mPropertiesToSelect);
    ret->SetLockConflictReader (GetLockConflictReader ());
    ret->SetLockStrategy (mStrategy);
    ret->SetLockType (mType);
#ifdef MIMIC_OTHER_PROVIDER
    // SPR 612054 SelectCommand error reporting inconsistency with the Oracle Provider when using invalid property name in filter.
    // we call prepare stream to discover errors now, rather than at ReadNext() time
    ret->PrepareStream ();
#endif // MIMIC_RDBMS

    return (FDO_SAFE_ADDREF (ret.p));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* ArcSDESelectCommand::GetLockConflicts ()
{
    FdoPtr<ArcSDEConnection> connection;

    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    return (FDO_SAFE_ADDREF (GetLockConflictReader ()));
}

void ArcSDESelectCommand::SetLockConflictReader (ArcSDELockConflictReader* conflicts)
{
    // this code and the over-ridden Dispose() maintain the refcount on this object
    // artificially low by one, which is the refcount that would have been placed
    // on this object by the lock conflict reader
    mConflictReader = conflicts;
    if (NULL == GetLockConflictReader ())
        // our lock conflict reader wants to die, he'll soon Release() us
        AddRef ();
    else
    {
        Release (); // our lock conflict reader is letting us know he's interested, he's already done AddRef()
        GetLockConflictReader ()->FdoILockConflictReader::AddRef (); // we're interested in him
    }
}


ArcSDELockConflictReader* ArcSDESelectCommand::GetLockConflictReader ()
{
    return (mConflictReader);
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void ArcSDESelectCommand::Dispose ()
{
    if (NULL == GetLockConflictReader ())
        delete this; // we're stand-alone, OK to go bye-bye
    else
        // our conflict reader still needs us, but we don't need it so Release()
        // this may lead to a recursive call through SetLockConflictReader()
        // to this->Dispose() if his refcount goes to zero
        // because we're the only one referencing him,
        // but at that time the conflict reader will be NULL
        GetLockConflictReader ()->Release ();
}

FdoExpressionEngineFunctionCollection* ArcSDESelectCommand::GetUserDefinedFunctions( FdoIConnection *fdoConnection, FdoClassDefinition *classDef )
{
     if (classDef->GetClassType() != FdoClassType_FeatureClass)
        return NULL;

	// Get the coordinated reference of for this class
    FdoPtr<FdoGeometricPropertyDefinition> geomProp = ((FdoFeatureClass*)classDef)->GetGeometryProperty();
    FdoPtr<FdoPropertyDefinitionCollection> fdoProperties = classDef->GetProperties();
    FdoPtr<FdoPropertyDefinition> fdoProperty = fdoProperties->GetItem(geomProp->GetName());

  	ArcSDEConnection* connection = static_cast<ArcSDEConnection*>(GetConnection ());

	// Get sde column name:
    CHAR column[SE_QUALIFIED_COLUMN_LEN];
    connection->PropertyToColumn (column, classDef, FdoPtr<FdoIdentifier>(FdoIdentifier::Create(fdoProperty->GetName())));

    // Get sde table name:
	CHAR table[SE_QUALIFIED_TABLE_NAME];
    connection->ClassToTable(table, classDef);

    // Get the sde coordref for the column the spatial filter applies to:
    SE_COORDREF coordRef;
    LONG lResult = GetCoordRefFromColumn(connection, table, column, coordRef);
    handle_sde_err<FdoCommandException>(connection->GetConnection(), lResult, __FILE__, __LINE__, ARCSDE_FAILED_PROCESSING_SPATIAL_CONDITION, "Failed to process the given spatial condition.");

	// Create the custom functions
	FdoPtr<FdoExpressionEngineFunctionCollection> userDefinedFunctions;

	userDefinedFunctions = FdoExpressionEngineFunctionCollection::Create();
	userDefinedFunctions->Add( ArcSDEFunctionLength2D::Create(connection, coordRef));
	userDefinedFunctions->Add( ArcSDEFunctionArea2D::Create (connection, coordRef));

	return FDO_SAFE_ADDREF(userDefinedFunctions.p);
}

