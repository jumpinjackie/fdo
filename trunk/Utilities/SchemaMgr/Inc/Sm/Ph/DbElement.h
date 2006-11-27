#ifndef FDOSMPHDBELEMENT_H
#define FDOSMPHDBELEMENT_H		1
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

#include <Sm/Ph/SchemaElement.h>

class FdoSmPhMgr;

// This class represents a database element (database, owner, table, view, etc.)
class FdoSmPhDbElement : public FdoSmPhSchemaElement
{
public:
    /// Returns true if this database element exists 
	bool GetExists() const;

    virtual FdoStringP GetDbName() const;

    /// Get the fully qualified name for this element.
    /// This varies between element type so some types override this
    /// functions
	virtual FdoStringP GetQName() const;

    /// Post modifications to the current connection
    virtual void Commit( bool fromParent = false, bool isBeforeParent = false );
    // Event that notifies this object that a commit has taken place.
    // Does nothing by default.
    virtual void OnAfterCommit();

    /// Get the state at the start of the current commit operation.
    /// returns FdoSchemaElementState_Unchange if not currently in a commit
    /// operation.
    FdoSchemaElementState GetCommitState();

/* TODO
    /// Gather all errors for this element and child elements into a chain of exceptions.
    /// Adds each error as an exception, to the given exception chain and returns
    /// the chain.
	//
    /// parameters:
    /// 	pFirstException: a chain of exceptions.
	virtual FdoSchemaException* Errors2Exception( FdoSchemaException* pFirstException = NULL ) const;
*/
protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhDbElement() {}

    /// Constructs an instance of a database element.
    FdoSmPhDbElement(
        FdoStringP name,                            // Database element name
        FdoPtr<FdoSmPhMgr> pMgr,                    // Physical Schema Manager
    /// if NULL, parent must be specified
    /// since, in this case, the manager
    /// will be extracted from the parent.
        const FdoSmPhSchemaElement* parent = NULL,   // element parent.
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    /// Indicates whether element already exists.
    );

	virtual ~FdoSmPhDbElement(void);

    /// Checks if this object can be committed, based on its parent's state. 
    /// Makes sure that if parent is new, children are committed after parent and
    /// if child is new then it is committed after parent.
    virtual bool CheckCommitDependencies( bool fromParent = false, bool isBeforeParent = false );

    /// Derived elements can override this function to commit child elements.
    virtual void CommitChildren( bool isBeforeParent ) {}

    /// Records this database element as having been updated so that modification can be 
    /// undone on rollback.
    /// Must be overridden in order to actually do something; not all elements
    /// need to be tracked (just tables and columns).
    virtual void UpdRollbackCache() {}

    /// Called when this object is deleted, removes this object from its parent object's
    /// collection. SubClasses that have parents must override this function.
    virtual void Discard() {}

    /// Various functions for posting changes to the current connection.
    virtual bool Add() = 0;
    virtual bool Modify() = 0;
    virtual bool Delete() = 0;


private:
    FdoSchemaElementState mCommitState;
/* TODO:
    virtual void Finalize();

*/

};

typedef FdoPtr<FdoSmPhDbElement> FdoSmPhDbElementP;

#endif


