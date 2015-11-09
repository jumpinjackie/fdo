#ifndef FDOSMLPSAD_H
#define FDOSMLPSAD_H		1
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

// Represents a Schema Attribute Dictionary Element, containing a name and value
// pair of strings.

class FdoSmLpSADElement: public FdoDictionaryElement
{
public:
	FdoSmLpSADElement( FdoStringP name, FdoStringP value ) :
		FdoDictionaryElement( (FdoString*) name, (FdoString*) value )
	{
	}

    /// Gets the SAD element's name
    virtual FdoString* GetName() const
    {
        return ((FdoDictionaryElement*) this)->GetName();
    }

    /// Gets the SAD element's value
    virtual FdoString* GetValue() const
    {
        return ((FdoDictionaryElement*) this)->GetValue();
    }

protected:
    /// Unused constructor needed only to build on Linux
    FdoSmLpSADElement() {}

	virtual ~FdoSmLpSADElement(void) {}
    virtual void Dispose() {delete this;}
};

typedef FdoPtr<FdoSmLpSADElement> FdoSmLpSADElementP;

// A Schema Attribute Dictionary, containing an arbitrary set of Dictionary
// elements.
class FdoSmLpSAD : public FdoSmNamedCollection<FdoSmLpSADElement>
{
protected:
    virtual void Dispose() 
    {
        delete this;
    }
};

typedef FdoPtr<FdoSmLpSAD> FdoSmLpSADP;

#endif


