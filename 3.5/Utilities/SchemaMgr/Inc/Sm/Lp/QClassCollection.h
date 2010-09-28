#ifndef FDOSMLPQCLASSCOLLECTION_H
#define FDOSMLPQCLASSCOLLECTION_H		1
//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifdef _WIN32
#pragma once
#endif

#include <Sm/Lp/ClassDefinition.h>
#include <Sm/NamedCollection.h>

// Qualified class collection element.
class FdoSmLpQClassDefinition : public FdoSmDisposable
{
public:
    FdoSmLpQClassDefinition( const FdoSmLpClassDefinition* classDef );
    ~FdoSmLpQClassDefinition();

    /// Get the class's qualified name.
    FdoString* GetName() const;

    /// Required by collection
    FdoBoolean CanSetName() const
    {
        return false;
    }

    /// Get the underlying LogicalPhysical class definition
    const FdoSmLpClassDefinition* RefClassDefinition() const;

protected:
    //Unused constructor needed only to build on Linux
    FdoSmLpQClassDefinition() {}

private:
    const FdoSmLpClassDefinition* mClassDef;
    FdoStringP mQName;
};

typedef FdoPtr<FdoSmLpQClassDefinition> FdoSmLpQClassDefinitionP;

// Represents a collection of Classes named by their qualified names.
// Using the qualified names allows this collection to contain 
// LogicalPhysical class definitions from multiple feature schemas.
class FdoSmLpQClassCollection : public FdoSmNamedCollection<FdoSmLpQClassDefinition>
{
public:
	FdoSmLpQClassCollection(void) :
		FdoSmNamedCollection<FdoSmLpQClassDefinition>(NULL)
	{}
	~FdoSmLpQClassCollection(void) {}

    /// Getter functions for retrieving a class definition from the collection.
    const FdoSmLpClassDefinition* RefClassDefinition(FdoInt32 item) const;
    /// Note: name must be the qualified name of the class. 
    const FdoSmLpClassDefinition* RefClassDefinition(FdoString* name) const;
};

typedef FdoPtr<FdoSmLpQClassCollection> FdoSmLpQClassesP;


#endif


