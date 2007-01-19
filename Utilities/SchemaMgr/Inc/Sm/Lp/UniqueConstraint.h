#ifndef FDOSMLPUNIQUECONSTRAINT_H
#define FDOSMLPUNIQUECONSTRAINT_H		1
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

#include <Sm/Lp/DataPropertyDefinitionCollection.h>
#include <Sm/Collection.h>

// Represents a collection of DataPropertyDefinitions.
class FdoSmLpUniqueConstraint : public FdoIDisposable
{
	friend class FdoSmLpClassBase;

public:
	
	FdoSmLpDataPropertyDefinitionCollection *GetProperties();
	const FdoSmLpDataPropertyDefinitionCollection *RefProperties() const;

    // Get the base constraint if constraint actually defined on base class.
    // Returnes NULL if this constraint does not have a base
    FdoPtr<FdoSmLpUniqueConstraint> GetBaseConstraint();
    const FdoSmLpUniqueConstraint* RefBaseConstraint() const;

    // Compare this constraint with another one.
    // Returns 0 if the two costraints have the same properties by name.
    // Return -1 otherwise.
    FdoInt32 Compare( FdoPtr<FdoSmLpUniqueConstraint> other ) const;

    // Create a unique constraint inherited from this constraint.
    // Returns NULL if not all the constrained properties are in the subClass.
    FdoPtr<FdoSmLpUniqueConstraint> CreateInherited( FdoSmLpClassBase* pSubClass, FdoSmLpPropertyDefinitionCollection* pSubClassProperties );

    // Sets the base constraint for this constraint.
    void SetBaseConstraint( FdoPtr<FdoSmLpUniqueConstraint> baseConstraint );

    /// Serialize this class to an XML file.
    /// Primarily for unit testing.
	virtual void XMLSerialize( FILE* xmlFp, int ref ) const;

protected:

	FdoSmLpUniqueConstraint();

	virtual void Dispose();

private:
	FdoSmLpDataPropertiesP	mProperties;
    FdoPtr<FdoSmLpUniqueConstraint> mBaseConstraint;

};

typedef FdoPtr<FdoSmLpUniqueConstraint> FdoSmLpUniqueConstraintP;



#endif


