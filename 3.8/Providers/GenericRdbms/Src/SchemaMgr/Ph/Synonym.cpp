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
#include "Owner.h"

FdoSmPhGrdSynonym::FdoSmPhGrdSynonym(
    FdoStringP name,
    FdoSmPhDbObjectP rootObject,
    const FdoSmPhOwner* pOwner,
    FdoSchemaElementState elementState,
    FdoSmPhRdDbObjectReader* reader
) :
    FdoSmPhSynonym(name, rootObject, pOwner, elementState, reader )
{
}

FdoSmPhGrdSynonym::~FdoSmPhGrdSynonym()
{
}

bool FdoSmPhGrdSynonym::Add()
{
    FdoSmPhGrdOwner* objOwner = static_cast<FdoSmPhGrdOwner*>((FdoSmPhSchemaElement*) GetParent());

    FdoStringP sqlStmt = FdoStringP::Format(
        L"create synonym %ls for %ls",
        (FdoString*) GetDbQName(),
        (FdoString*) GetRootObject()->GetDbQName()
    );

    objOwner->ActivateAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhGrdSynonym::Delete()
{
    FdoSmPhGrdOwner* objOwner = static_cast<FdoSmPhGrdOwner*>((FdoSmPhSchemaElement*) GetParent());

    FdoStringP sqlStmt = FdoStringP::Format(
        L"drop synonym %ls",
        (FdoString*) GetDbQName()
    );

    objOwner->ActivateAndExecute( sqlStmt );

    return true;
}

bool FdoSmPhGrdSynonym::Modify()
{
    // Dropping and re-creating the sequence is the simplest way to handle 
    // modifications.
    Delete();
    SetElementState( FdoSchemaElementState_Added );
    return false;
}

