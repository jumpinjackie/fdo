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
#include "ShpSelectCommand.h"
#include "ShpFeatureReader.h"
#include <algorithm>
#include "Util/FdoExpressionEngineUtilDataReader.h"
#include "FdoExpressionEngineImp.h"


ShpSelectCommand::ShpSelectCommand (ShpConnection* connection) :
    FdoCommonFeatureCommand<FdoISelect, ShpConnection> (connection)
{
}

/** Do not implement the copy constructor. **/
//ShpSelectCommand::ShpSelectCommand (const ShpSelectCommand &right) { }

ShpSelectCommand::~ShpSelectCommand (void)
{
}

// <summary>Dispose this object.</summary>
// <returns>Returns nothing.</returns> 
void ShpSelectCommand::Dispose ()
{
    delete this;
}

/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* ShpSelectCommand::GetPropertyNames ()
{
    if (mPropertiesToSelect == NULL)
        mPropertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(mPropertiesToSelect.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType ShpSelectCommand::GetLockType ()
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockType"));
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void ShpSelectCommand::SetLockType (FdoLockType value)
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"SetLockType"));
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy ShpSelectCommand::GetLockStrategy ()
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockStrategy"));
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void ShpSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"SetLockStrategy"));
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* ShpSelectCommand::Execute ()
{
    FdoPtr<FdoIdentifier> id;
    FdoString* class_name;
    FdoPtr<ShpFeatureReader> ret;

    FdoPtr<ShpConnection> shpConn = (ShpConnection*)GetConnection ();

    // Validate that there are no aggregate functions:
    FdoCommonExpressionType exprType;

	FdoPtr<FdoIExpressionCapabilities> expressCaps = shpConn->GetExpressionCapabilities();
	FdoPtr<FdoFunctionDefinitionCollection> funcDefs = expressCaps->GetFunctions();
    FdoPtr< FdoArray<FdoFunction*> > functions = FdoExpressionEngineUtilDataReader::GetAggregateFunctions(funcDefs, mPropertiesToSelect, exprType);
    if (exprType == FdoCommonExpressionType_Aggregate)
        throw FdoCommandException::Create(FdoException::NLSGetMessage(FDO_182_AGGREGATE_IN_SELECT, "Aggregate functions are not supported by the Select command; use the SelectAggregates command instead."));

    id = GetFeatureClassName ();
    class_name = id->GetText ();

    // Validate that the expressions are correct:
    FdoPtr<FdoClassDefinition> fdoClass = FdoPtr<ShpLpClassDefinition>(ShpSchemaUtilities::GetLpClassDefinition (mConnection, class_name))->GetLogicalClass();
    FdoPropertyType propType;
    FdoDataType dataType;
    for (int i=0; mPropertiesToSelect && i<mPropertiesToSelect->GetCount(); i++)
    {
        FdoPtr<FdoIdentifier> id = mPropertiesToSelect->GetItem(i);
        FdoPtr<FdoIExpressionCapabilities> expressionCaps = shpConn->GetExpressionCapabilities();
        FdoPtr<FdoFunctionDefinitionCollection> functions = expressionCaps->GetFunctions();
        FdoCommonMiscUtil::GetExpressionType(functions, fdoClass, id, propType, dataType);
    }

    // Create the reader:

	// Validate the filter. The filter may contain computed expressions involving not selected properties.
	// Also check for unsupported spatial operations and try to optimize the filter
	if( mFilter != NULL )
	{
		FdoPtr<FdoClassDefinition> classDef = ShpSchemaUtilities::GetLogicalClassDefinition (shpConn, class_name, NULL);
		FdoPtr<FdoIFilterCapabilities> filterCaps = shpConn->GetFilterCapabilities();
        FdoExpressionEngineImp::ValidateFilter( classDef, mFilter, mPropertiesToSelect, filterCaps );

		mFilter = FdoExpressionEngineImp::OptimizeFilter( mFilter );
	}

    ret = new ShpFeatureReader (shpConn, class_name, mFilter, mPropertiesToSelect);

    return (FDO_SAFE_ADDREF (ret.p));
}

/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* ShpSelectCommand::ExecuteWithLock ()
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"ExecuteWithLock"));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* ShpSelectCommand::GetLockConflicts ()
{
    throw FdoException::Create (NlsMsgGet (SHP_LOCKING_NOT_SUPPORTED, "Locking not supported (%1$ls).", L"GetLockConflicts"));
}

