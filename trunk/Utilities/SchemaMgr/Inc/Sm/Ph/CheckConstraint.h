#ifndef FDOSMPHCHECKCONSTRAINT_H
#define FDOSMPHCHECKCONSTRAINT_H		1
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

// Represents a PhCheckConstraint object.
class FdoSmPhCheckConstraint : public FdoIDisposable
{
public:

	FdoSmPhCheckConstraint() {}

	FdoSmPhCheckConstraint( FdoStringP name, FdoStringP columnName, FdoStringP checkClause) 
	{
		mName = name;
		mColumnName = columnName;
		mCheckClause = checkClause;	
		mState = FdoSchemaElementState_Unchanged;
	}

	~FdoSmPhCheckConstraint() {}

	FdoStringP	GetName()			{ return mName; }
	FdoStringP	GetClause()			{ return mCheckClause; }
	FdoStringP	GetColumnName()		{ return mColumnName; }

	void		SetElementState(FdoSchemaElementState state) { mState = state; }
	FdoSchemaElementState GetElementState() { return mState; }

protected:
	virtual void Dispose() { delete this; };

private:
	FdoStringP	mName;
	FdoStringP	mCheckClause;
	FdoStringP	mColumnName;
	FdoSchemaElementState	mState;

};

typedef FdoPtr<FdoSmPhCheckConstraint> FdoSmPhCheckConstraintP;



#endif


