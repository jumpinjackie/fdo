#ifndef FDOSMPHVIEW_H
#define FDOSMPHVIEW_H		1
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

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Ph/DbObject.h>

class FdoSmPhMgr;
class FdoSmPhOwner;

// Table represents a table object which describes a 
// table in the schema that stores class properties.
class FdoSmPhView : virtual public FdoSmPhDbObject
{
public:
    void SetRootObject( FdoSmPhDbObjectP rootObject );

    /// Serialize the table to an XML file.
    /// Mainly for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmPhView() {}

    /// Create an instance of a view.
	//
    /// Parameters:
    /// 	viewName: the name of the view to created.
    /// 	database: RDBMS instance where foreign table resides.
    /// 	owner:  RDBMS schema where foreign table resides.
    /// 	tableName: the name of the foreigntable.
    /// 	pSchema: holds the connection for the database containing this column.
    FdoSmPhView(
        FdoStringP viewName, 
        FdoStringP rootDatabase, 
        FdoStringP rootOwner, 
        FdoStringP rootObjectName, 
        const FdoSmPhOwner* pOwner,
		FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

	~FdoSmPhView(void);

    /// Commit modifications to child objects
    virtual void CommitChildren( bool isBeforeParent );

    /// Get SQL "create view" statement
    virtual FdoStringP GetAddSql();

    /// Get SQL "drop view" statement
    virtual FdoStringP GetDeleteSql();

    /// Get SQL "create view" column list
    virtual FdoStringsP GetAddColsSql();

    /// Get SQL "create view" "select from root object" clause
    virtual FdoStringP GetAddRootSql();

    /// Get SQL name for the root object
    virtual FdoStringP GetRootNameSql() = 0;

private:
};

typedef FdoPtr<FdoSmPhView> FdoSmPhViewP;

#endif


