#ifndef FDOSMPHMYSQLCOLLATION_H
#define FDOSMPHMYSQLCOLLATION_H       1
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

#include <Sm/Ph/Collation.h>

// MySql Provider implementation of a Collation.
class FdoSmPhMySqlCollation : public FdoSmPhCollation
{
public:
    // Constructs an instance of a Collation object.
    //
    // Parameters:
    //      name: collation name
    //      pDatabase: database instance where collation defined.
    //      reader: initialize the collation members from this collation
    //          reader.
    FdoSmPhMySqlCollation(
        FdoStringP name,
        const FdoSmPhDatabase* pDatabase,
        FdoSmPhRdCollationReader* reader
    );

    ~FdoSmPhMySqlCollation(void);

protected:
    // Collation Modification not supported. 
    // These functions do nothing.
    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

private:
    FdoSmPhMySqlCollation() {};
};

typedef FdoPtr<FdoSmPhMySqlCollation> FdoSmPhMySqlCollationP;

#endif
