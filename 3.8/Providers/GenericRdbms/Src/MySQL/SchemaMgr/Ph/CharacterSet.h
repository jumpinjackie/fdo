#ifndef FDOSMPHMYSQLCHARACTERSET_H
#define FDOSMPHMYSQLCHARACTERSET_H       1
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

#include <Sm/Ph/CharacterSet.h>

// MySql Provider implementation of a Character Set.
// Represents an MySql schema (user)
class FdoSmPhMySqlCharacterSet : public FdoSmPhCharacterSet
{
public:
    // Constructs an instance of a CharacterSet object.
    //
    // Parameters:
    //      name: character set name
    //      pDatabase: database instance where character set defined.
    //      reader: initialize the character set members from this character
    //          set reader.
    FdoSmPhMySqlCharacterSet(
        FdoStringP name,
        const FdoSmPhDatabase* pDatabase,
        FdoSmPhRdCharacterSetReader* reader
    );

    ~FdoSmPhMySqlCharacterSet(void);

    // Get the maximum character size in bytes. Example sizes are:
    //      Latin1: 1
    //      CP932:  2
    //      UTF8:   3
    FdoInt64 GetCharLen() const;

protected:
    // Character Set Modification not supported. 
    // These functions do nothing.
    virtual bool Add();
    virtual bool Modify();
    virtual bool Delete();

private:
    FdoSmPhMySqlCharacterSet() {};

    FdoInt64 mCharLen;
};

typedef FdoPtr<FdoSmPhMySqlCharacterSet> FdoSmPhMySqlCharacterSetP;

#endif
