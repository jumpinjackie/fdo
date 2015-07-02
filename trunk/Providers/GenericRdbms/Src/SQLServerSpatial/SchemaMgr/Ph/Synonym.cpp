/*
 * (C) Copyright 2011 by Autodesk, Inc. All Rights Reserved.
 *
 * By using this code, you are agreeing to the terms and conditions of
 * the License Agreement included in the documentation for this code.
 *
 * AUTODESK MAKES NO WARRANTIES, EXPRESS OR IMPLIED, AS TO THE
 * CORRECTNESS OF THIS CODE OR ANY DERIVATIVE WORKS WHICH INCORPORATE
 * IT. AUTODESK PROVIDES THE CODE ON AN "AS-IS" BASIS AND EXPLICITLY
 * DISCLAIMS ANY LIABILITY, INCLUDING CONSEQUENTIAL AND INCIDENTAL
 * DAMAGES FOR ERRORS, OMISSIONS, AND OTHER PROBLEMS IN THE CODE.
 *
 * Use, duplication, or disclosure by the U.S. Government is subject
 * to restrictions set forth in FAR 52.227-19 (Commercial Computer
 * Software Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
 * (Rights in Technical Data and Computer Software), as applicable.
 * 
 */

#include "stdafx.h"
#include "Synonym.h"
#include "Mgr.h"
#include "Owner.h"

FdoSmPhSqsSynonym::FdoSmPhSqsSynonym(
    FdoStringP name,
    FdoSmPhDbObjectP rootObject,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhSqsDbObject( name, pOwner, reader ),
    FdoSmPhGrdSynonym( name, rootObject, pOwner, elementState, reader ),
    FdoSmPhDbObject(name, pOwner, elementState)
{
}

FdoSmPhSqsSynonym::~FdoSmPhSqsSynonym()
{
}

bool FdoSmPhSqsSynonym::ClassifyObjectType(FdoBoolean classifyDefaultTypes )
{
    return true;
}

bool FdoSmPhSqsSynonym::Add()
{
    // SQL Server does not allow the synonym name to include database name.
    // This is ok since ActivateOwnerAndExecute ensures that the database for 
    // the synonym is the current one.
    FdoStringP sqlStmt = FdoStringP::Format(
        L"create synonym %ls for %ls",
        (FdoString*) GetDbName(),
        (FdoString*) GetRootObject()->GetDbQName()
    );

    ActivateOwnerAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhSqsSynonym::Delete()
{
    // SQL Server does not allow the synonym name to include database name.
    // This is ok since ActivateOwnerAndExecute ensures that the database for 
    // the synonym is the current one.

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop synonym %ls",
        (FdoString*) GetDbName()
    );

    ActivateOwnerAndExecute( sqlStmt );

    return true;
}



