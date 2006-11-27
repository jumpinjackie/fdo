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
 */

#include "stdafx.h"
#include <Sm/Ph/DbElement.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhDbElement::FdoSmPhDbElement(
    FdoStringP name, 
    FdoPtr<FdoSmPhMgr> pMgr,  
    const FdoSmPhSchemaElement* parent,   
    FdoSchemaElementState elementState
) : 
    FdoSmPhSchemaElement((FdoString *)name, L"", pMgr, parent ),
    mCommitState(FdoSchemaElementState_Unchanged)
{
    FdoSmPhSchemaElement::SetElementState(elementState);
}

FdoSmPhDbElement::~FdoSmPhDbElement(void)
{
}

bool FdoSmPhDbElement::GetExists() const
{
    FdoSchemaElementState elementState = GetElementState();

	return (elementState != FdoSchemaElementState_Added);
}

FdoStringP FdoSmPhDbElement::GetDbName() const
{
    if ( (wcslen(GetName()) > 0) && ((FdoSmPhDbElement*) this)->GetManager()->SupportsAnsiQuotes() ) 
        return FdoStringP(L"\"") + GetName() + L"\"";
    else
        return GetName();
}

FdoStringP FdoSmPhDbElement::GetQName() const
{
    FdoStringP qName;
    const FdoSmSchemaElement* parent = GetParent();

    if ( parent ) {
        qName = parent->GetQName();

        if ( qName.GetLength() > 0 ) 
            qName += L".";
    }

    qName += GetName();

    return qName;
}

void FdoSmPhDbElement::Commit( bool fromParent, bool isBeforeParent )
{
    bool actionComplete = false;

    mCommitState = GetElementState();

    if ( !CheckCommitDependencies(fromParent, isBeforeParent) ) {
        mCommitState = FdoSchemaElementState_Unchanged;
        return;
    }

    if ( GetElementState() != FdoSchemaElementState_Unchanged ) {
        // Check for errors before committing.
        FdoSchemaExceptionP exception = Errors2Exception();
        if ( exception ) 
            throw FDO_SAFE_ADDREF( (FdoSchemaException*) exception );
    }

    // Perform mods to children (modify, delete) that must happen before this
    // element is committed.
    CommitChildren( true );

    // Call provider-specific implementor depending on the modification action.
    switch ( GetElementState() ) {
	case FdoSchemaElementState_Added:
        actionComplete = Add();
		break;

	case FdoSchemaElementState_Deleted:

        if ( GetExists() )
    		actionComplete = Delete();
        else 
            actionComplete = true;
		break;

	case FdoSchemaElementState_Modified:	
        actionComplete = Modify();
		break;
	}

    if ( actionComplete ) {
        // Add this element to the rollback cache so modification can be undone on
        // rollback.
        UpdRollbackCache();

        if ( mCommitState == FdoSchemaElementState_Deleted ) {
            SetElementState( FdoSchemaElementState_Detached );
            // remove deleted objects from parent (also removes this object from
            // the cache).
            Discard();
        }
        else {
            SetElementState( FdoSchemaElementState_Unchanged );
        }
    }

    // Perform mods to children (add) that must happen after this
    // element is committed.
    CommitChildren( false );

    // Notify Physical Schema Manager that physical schema has changed.
    // Skip when commit invoked from parent since parent will do the notification
    // when all committing complete.
    if ( !fromParent )
        GetManager()->OnAfterCommit();

    // Check for errors that happened while committing.
	if ( !fromParent ) 
	{
		FdoSchemaExceptionP exception = Errors2Exception();
		if ( exception ) 
			throw FDO_SAFE_ADDREF( (FdoSchemaException*) exception );
	}

    mCommitState = FdoSchemaElementState_Unchanged;
}
   
void FdoSmPhDbElement::OnAfterCommit()
{
}

bool FdoSmPhDbElement::CheckCommitDependencies( bool fromParent, bool isBeforeParent )
{
    // Get the parent's element state
    FdoSchemaElementState parentElementState = FdoSchemaElementState_Detached;
    const FdoSmSchemaElement* pParent = GetParent();

    if ( pParent ) 
        parentElementState = pParent->GetElementState();

    if ( fromParent && (parentElementState == FdoSchemaElementState_Added) ) {
        // Parent is new. Commit must wait until parent is added.
        return false;
    }

    if ( isBeforeParent &&
         (mCommitState == FdoSchemaElementState_Added)
    ) {
        // new elements must be added after mods to parent are committed.
        // modified and deleted elements are processed before parent is committed.
        return false;
    }

    return true;
}

FdoSchemaElementState FdoSmPhDbElement::GetCommitState()
{
    return mCommitState;
}

