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

ArcSDEDestroySchemaCommand::ArcSDEDestroySchemaCommand (FdoIConnection *connection) :
    ArcSDECommand<FdoIDestroySchema> (connection),
    mSchemaName (NULL)
{
}

/** Do not implement the copy constructor. **/
//ArcSDEDestroySchemaCommand::ArcSDEDestroySchemaCommand (const ArcSDEDestroySchemaCommand &right) { }

ArcSDEDestroySchemaCommand::~ArcSDEDestroySchemaCommand (void)
{
    FDO_SAFE_RELEASE(mSchemaName);
}

//
// FdoIDestroySchema interface
//

///<summary>Gets the name of the schema to destroy as a string.</summary>
/// <returns>Returns the name of the schema to destroy</returns> 
FdoString* ArcSDEDestroySchemaCommand::GetSchemaName ()
{
    return (mSchemaName->GetName ());
}


///<summary>Sets the name of the schema to destroy as a string.</summary>
/// <param name="value">Input the name of the schema to destroy</param> 
/// <returns>Returns nothing</returns> 
void ArcSDEDestroySchemaCommand::SetSchemaName (FdoString* value)
{
    FDO_SAFE_RELEASE(mSchemaName);
    mSchemaName = NULL;
    if (value != NULL)
    {
        FdoPtr<FdoIdentifier> name = FdoIdentifier::Create (value);
        mSchemaName = name;
        FDO_SAFE_ADDREF(mSchemaName);
    }
}

///<summary>Executes the destroy schema command, which removes the schema, class
///definitions, relation definitions, and all instance data from the DataStore.
///If elements in other schemas refer to the schema to be destroyed
///an exception is thrown.</summary>
/// <returns>Returns nothing</returns> 
void ArcSDEDestroySchemaCommand::Execute ()
{
    FdoPtr<ArcSDEConnection> connection;
    
    // verify the connection
    connection = static_cast<ArcSDEConnection*>(GetConnection ());
    if (connection == NULL)
        throw FdoException::Create (NlsMsgGet (ARCSDE_CONNECTION_NOT_ESTABLISHED, "Connection not established."));

    FdoPtr<FdoIDescribeSchema> describe = (FdoIDescribeSchema*)connection->CreateCommand (FdoCommandType_DescribeSchema);
    describe->SetSchemaName (mSchemaName->GetName ());
    FdoPtr<FdoFeatureSchemaCollection> schemas = describe->Execute ();
    FdoPtr<FdoFeatureSchema> schema = schemas->GetItem (0);
    FdoPtr<FdoClassCollection> classes = schema->GetClasses ();
    // for now, to delete a schema, just delete all the classes
    for (int i = 0; i < classes->GetCount (); i++)
    {
        FdoPtr<FdoClassDefinition> feature = classes->GetItem (i);
        feature->Delete ();
    }
    FdoPtr<FdoIApplySchema> apply = (FdoIApplySchema*)connection->CreateCommand (FdoCommandType_ApplySchema);
    apply->SetFeatureSchema (schema);
    apply->Execute ();
}

