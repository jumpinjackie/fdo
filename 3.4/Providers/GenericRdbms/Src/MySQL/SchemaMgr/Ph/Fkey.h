#ifndef FDOSMPHMYSQLFKEY_H
#define FDOSMPHMYSQLFKEY_H        1
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

#include "../../../SchemaMgr/Ph/Fkey.h"
#include <Sm/Ph/Table.h>

// MySql Provider implementation of a Foreign Key.
class FdoSmPhMySqlFkey : public FdoSmPhGrdFkey
{
public:
    // Constructs an instance of a Foreign Key object.
    //
    // Parameters:
    //      name: Foreign Key  name
    //      pFkeytable: the Foreign Table..
	//		pkeyTableName: name of primary table referenced by this foreign key.
	//		pkeyTableOwner: name of primary table owner.
    //      elementState: indicates whether foreign key is new or already exists.

    FdoSmPhMySqlFkey(
        FdoStringP name,
        const FdoSmPhTable* pFkeyTable,
        FdoStringP pkeyTableName,
        FdoStringP pkeyTableOwner,
        FdoSchemaElementState elementState = FdoSchemaElementState_Added
    );

    ~FdoSmPhMySqlFkey(void);

};

typedef FdoPtr<FdoSmPhMySqlFkey> FdoSmPhMySqlFkeyP;

#endif
