#ifndef FDOSMPHGRDTABLE_H
#define FDOSMPHGRDTABLE_H       1
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

#include <Sm/Ph/Table.h>

// Generic RDBMS implementation of a Table.
class FdoSmPhGrdTable : public FdoSmPhTable
{
public:
    // Constructs an instance of a Table object.
    //
    // Parameters:
    //      name: table name
    //      pOwner: the table's owning schema.
    //      elementState: indicates whether this is a new or existing table.
    //      pkeyName: name for table's primary key. Applies only to new tables.
    //          Primary key name on existing tables not changed.
    FdoSmPhGrdTable(
        FdoStringP name,
        const FdoSmPhOwner* pOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added,
        FdoStringP pkeyName = L""
    );

    ~FdoSmPhGrdTable(void);

    // Executes a DDL statement that affects this table (e.g. add/delete an index
    // or foreign key on this table).
    // The default implementation simply executes the statement. However, providers
    // that need to do special things can override this function.
    // For example, the Oracle provider must execute these statements within a 
    // DDL transaction when this table is OWM-enabled.
    virtual void ExecuteDDL( 
        FdoStringP sqlStmt, // the DDL statement
        FdoSmPhGrdTable* refTable = NULL, // referenced table for foreign key.
                                          // must be specified if the statement operates
                                          // on a foreign key.
        bool isDDL = true                 // true if the statement must be executed
                                          // as a DDL statement. 
    );

    // Deletes all rows from this table.
    virtual void ClearRows();

    // This function is used by the LogicalPhysical level to set the locking
    // mode to FDO when the table supports FDO persistent locking. The Physical 
    // level does not have enough knowledge to determine the locking mode of 
    // a pre-existing table so the LogicalPhysical level 
    // (FdoSmLpGrdClassDefinition) figures it out.
    // If a class has a LockId system property then it calls this function for 
    // its class table, passing in the name of the column for this system property.
    // If this table contains the column the its locking mode must be FDO and this
    // function sets the table's locking mode accordingly.
    void InitFdoLocking( FdoStringP colName );

    // The LT mode is set in a similar manner (see InitFdoLocking).
    void InitFdoLt( FdoStringP colName );

protected:
    // unused constructor needed only to build on Linux
    FdoSmPhGrdTable() {}

    // Table modification implementors
    virtual bool AddColumn( FdoSmPhColumnP column );
    virtual bool ModifyColumn( FdoSmPhColumnP column );
    virtual bool DeleteColumn( FdoSmPhColumnP column );
};

typedef FdoPtr<FdoSmPhGrdTable> FdoSmPhGrdTableP;

#endif
