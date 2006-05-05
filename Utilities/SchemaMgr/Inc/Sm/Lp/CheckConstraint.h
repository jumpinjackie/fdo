#ifndef FDOSMLPCHECKCONSTRAINT_H
#define FDOSMLPCHECKCONSTRAINT_H		1
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

#include <Sm/Disposable.h>

// Represents a LpCheckConstraint object.
class FdoSmLpCheckConstraint : public FdoIDisposable
{
public:

	FdoSmLpCheckConstraint() {}

	FdoSmLpCheckConstraint( FdoStringP propName, FdoStringP columnName, FdoStringP checkClause) 
	{
		mCheckClause = checkClause;	
		mPropertyName = propName;
		mColumnName = columnName;
	}

	~FdoSmLpCheckConstraint() {}

	FdoStringP	GetClause()			{ return mCheckClause; }
	FdoStringP	GetPropertyName()	{ return mPropertyName; }
	FdoStringP	GetColumnName()		{ return mColumnName; }


protected:
	virtual void Dispose() { 	
		delete this; 
	}

private:
	FdoStringP	mCheckClause;
	FdoStringP	mPropertyName;
	FdoStringP	mColumnName;
};

typedef FdoPtr<FdoSmLpCheckConstraint> FdoSmLpCheckConstraintP;

#endif


