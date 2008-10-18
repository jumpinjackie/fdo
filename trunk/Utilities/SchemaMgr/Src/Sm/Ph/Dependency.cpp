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

#include "stdafx.h"
#include <Sm/Ph/Dependency.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhDependency::FdoSmPhDependency(
		FdoStringP pkTableName, 
		FdoSmPhColumnListP pPkColumnNames,
		FdoStringP fkTableName, 
		FdoSmPhColumnListP pFkColumnNames,
		FdoStringP identityColumn,
		FdoStringP orderType,
		long lCardinality,
		const FdoSmPhSchemaElement* pParent
) : 
    FdoSmPhSchemaElement(pkTableName + L"_" + fkTableName, L"", (FdoSmPhMgr*) NULL, pParent ),
	mPkTableName(pkTableName), 
	mFkTableName(fkTableName), 
	mIdentityColumn(identityColumn),
	mOrderType(orderType),
	mlCardinality(lCardinality)
{
	mpPkColumnNames = pPkColumnNames;
	mpFkColumnNames = pFkColumnNames;
}

FdoSmPhDependency::~FdoSmPhDependency(void)
{
}

FdoStringP FdoSmPhDependency::GetPkTableName() const
{
	return(mPkTableName);
}

FdoSmPhColumnListP FdoSmPhDependency::GetPkColumnNames() const
{
	return(mpPkColumnNames);
}

FdoStringP FdoSmPhDependency::GetFkTableName() const
{
	return(mFkTableName);
}

FdoSmPhColumnListP FdoSmPhDependency::GetFkColumnNames() const
{
	return(mpFkColumnNames);
}

FdoStringP FdoSmPhDependency::GetIdentityColumn() const
{
	return(mIdentityColumn);
}

FdoStringP FdoSmPhDependency::GetOrderType() const
{
	return(mOrderType);
}

long FdoSmPhDependency::GetCardinality() const
{
	return(mlCardinality);
}
