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
 *
 * Revision Control Modification History
 *
 *  $HeadURL: http://svn.hobu.net/frank/GdalFile/src/Provider/FdoRfpSelectCommand.cpp $
 *   $Author: fwarmerdam $
 *     $Date: 2006-01-25 14:22:51 -0500 (Wed, 25 Jan 2006) $
 * $Revision: 23 $
 *
 */

#include "FDORFP.h"
#include "FdoRfpConnection.h"
#include "FdoRfpSelectCommand.h"
#include "FdoRfpFeatureReader.h"
#include "FdoRfpQueryExecutor.h"
#include "FdoRfpClassData.h"


FdoRfpSelectCommand::FdoRfpSelectCommand (FdoIConnection *connection) :
    FdoRfpFeatureCommand<FdoISelect> (connection)
{
}

FdoRfpSelectCommand::~FdoRfpSelectCommand()
{
}


/// <summary>Gets the FdoIdentifierCollection that holds the list of property names to 
/// return with the result. If empty all properties of the specified class
/// are returned.</summary>
/// <returns>Returns the list of property names.</returns> 
FdoIdentifierCollection* FdoRfpSelectCommand::GetPropertyNames ()
{
    if (m_propertiesToSelect == NULL)
        m_propertiesToSelect = FdoIdentifierCollection::Create();

    return (FDO_SAFE_ADDREF(m_propertiesToSelect.p));
}

/// <summary>Gets the FdoLockType value (see "Locking Commands").</summary>
/// <returns>Returns the lock type.</returns> 
FdoLockType FdoRfpSelectCommand::GetLockType ()
{
	throw FdoCommandException::Create(NlsMsgGet(GRFP_72_LOCK_NOT_SUPPORTED, "Locking not supported.")); 
	return FdoLockType_None;
}

/// <summary>Sets the FdoLockType value (see "Locking Commands").</summary>
/// <param name="value">Input the lock type.</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpSelectCommand::SetLockType (FdoLockType value)
{
	throw FdoCommandException::Create(NlsMsgGet(GRFP_72_LOCK_NOT_SUPPORTED, "Locking not supported.")); 
}

/// <summary>Gets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <returns>Returns the lock strategy.</returns> 
FdoLockStrategy FdoRfpSelectCommand::GetLockStrategy ()
{
	throw FdoCommandException::Create(NlsMsgGet(GRFP_72_LOCK_NOT_SUPPORTED, "Locking not supported.")); 
	return FdoLockStrategy_All;
}

/// <summary>Sets the FdoLockStrategy value (see "Locking Commands").</summary>
/// <param name="value">Input the lock strategy.</param> 
/// <returns>Returns nothing</returns> 
void FdoRfpSelectCommand::SetLockStrategy (FdoLockStrategy value)
{
	throw FdoCommandException::Create(NlsMsgGet(GRFP_72_LOCK_NOT_SUPPORTED, "Locking not supported.")); 
}

/// <summary>Executes the select command and returns a reference to an FdoIFeatureReader.</summary>
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* FdoRfpSelectCommand::Execute ()
{
    FdoRfpConnectionP connection = static_cast<FdoRfpConnection*>(GetConnection());

    //Verify the feature class name is specified
    if (mClassName == NULL)
        throw FdoCommandException::Create(NlsMsgGet(GRFP_49_CLASS_NAME_MUST_BE_SPECIFIED, "Feature class name must be specified."));		

    FdoPtr<FdoClassDefinition> classDef;
    connection->GetClassDef(mClassName, classDef);
    if (classDef == NULL)
        throw FdoCommandException::Create(NlsMsgGet1(GRFP_11_FEATURE_CLASS_NOT_FOUND, "Feature class '%1$ls' not found in schema.", mClassName->GetName()));

    //Get the class data
    FdoPtr<FdoRfpClassData> classData = connection->GetClassData(classDef);
    
    // if the user specify the select list, identity property should be included 
    if (m_propertiesToSelect != NULL && m_propertiesToSelect->GetCount() > 0)
    {
        // first find the identity property
        FdoPtr<FdoDataPropertyDefinitionCollection> props = classDef->GetIdentityProperties();
        FdoPtr<FdoDataPropertyDefinition> prop = props->GetItem(0); //only contain 1 identify property
        FdoString* identityPropName = prop->GetName();
        // find in properties to select
        FdoPtr<FdoIdentifier> identityId = m_propertiesToSelect->FindItem(identityPropName);
        if (identityId == NULL) // not found, add it
        {
            identityId = FdoIdentifier::Create(identityPropName);
            m_propertiesToSelect->Add(identityId);
        }
    }
    
    // delegate the task to the executor
    FdoRfpQueryExecutor executor(classDef, mFilter, m_propertiesToSelect, classData);

    FdoPtr<FdoRfpQueryResult> queryResult = executor.Execute();

    // can not select aggregated result by select command, use SelectAggregate instead
    if (queryResult->aggregated)
        throw FdoCommandException::Create(NlsMsgGet(GRFP_88_CAN_NOT_SELECT_AGGREGATE, "Can not select aggregated result by Select command."));

    return FdoRfpFeatureReader::Create(classDef, queryResult);
}


/// <summary>Executes the select command and returns a reference to an
/// IFeatureReader.</summary> 
/// <returns>Returns the feature reader.</returns> 
FdoIFeatureReader* FdoRfpSelectCommand::ExecuteWithLock ()
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"FdoISelectCommand::ExecuteWithLock"));
}

/// <summary> When executing the operation ExecuteWithLock lock 
/// conflicts may occur. Those lock conflicts are reported. The 
/// function GetLockConflicts returns a lock conflict reader 
/// providing access to the list of lock conflicts that occurred 
/// during the execution of the operation.</summary>
/// <returns>Returns a lock conflict reader.</returns> 
FdoILockConflictReader* FdoRfpSelectCommand::GetLockConflicts ()
{
	throw FdoCommandException::Create(NlsMsgGet1(GRFP_59_OPERATION_NOT_SUPPORTED, "'%1$ls' not supported.", L"FdoISelectCommand::GetLockConflicts"));
}


