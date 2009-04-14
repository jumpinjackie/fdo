#ifndef FDOSMPHCOLLATION_H
#define FDOSMPHCOLLATION_H       1
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

#include <Sm/NamedCollection.h>
#include <Sm/Ph/CharacterSet.h>

class FdoSmPhRdCollationReader;
class FdoSmPhDatabase;

// This class represents a collation defined by the current RDBMS. 
// Collations typically define case-sensitivity and sort ordering for characters
class FdoSmPhCollation : public FdoSmPhDbElement
{
public:
    // Constructs an instance of a Collation object.
    //
    // Parameters:
    //      name: collation name
    //      pDatabase: database instance where collation defined.
    //      reader: initialize the collation members from this collation
    //          reader.
    FdoSmPhCollation(
        FdoStringP name,
        const FdoSmPhDatabase* pDatabase,
        FdoSmPhRdCollationReader* reader
    );

    ~FdoSmPhCollation(void);

    // Get the character set to which this collation applies.
    FdoSmPhCharacterSetP GetCharacterSet();

protected:
    FdoSmPhCollation() {};

private:
    FdoStringP mCharacterSetName;

};

// Smart pointer type provided for convenience
typedef FdoPtr<FdoSmPhCollation> FdoSmPhCollationP;

// Collection of collations.
class FdoSmPhCollationCollection : public FdoSmNamedCollection<FdoSmPhCollation>
{
public:
	FdoSmPhCollationCollection(void) :
		FdoSmNamedCollection<FdoSmPhCollation>(NULL)
	{}

	~FdoSmPhCollationCollection(void) {}

};

// Smart pointer type provided for convenience
typedef FdoPtr<FdoSmPhCollationCollection> FdoSmPhCollationsP;

#endif
