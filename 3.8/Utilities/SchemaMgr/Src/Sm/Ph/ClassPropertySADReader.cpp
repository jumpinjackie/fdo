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
#include <Sm/Ph/ClassPropertySADReader.h>
#include <Sm/Ph/Mgr.h>

FdoSmPhClassPropertySADReader::FdoSmPhClassPropertySADReader( FdoStringP schemaName, FdoStringP className, FdoStringP propertyName, FdoSmPhSADReaderP pSADReader ): 
FdoSmPhGroupReader( FdoStringP(schemaName) + L":" + className + L"." + propertyName, pSADReader.p->SmartCast<FdoSmPhReader>()),
	mpSADReader( pSADReader )
{
}

FdoSmPhClassPropertySADReader::~FdoSmPhClassPropertySADReader(void)
{
}

FdoStringP FdoSmPhClassPropertySADReader::GetName()
{
	return( mpSADReader->GetName() );
}

FdoStringP FdoSmPhClassPropertySADReader::GetValue()
{
	return( mpSADReader->GetValue() );
}

FdoStringP FdoSmPhClassPropertySADReader::GetGroupName()
{
	return( mpSADReader->GetOwnerName() + L"." + mpSADReader->GetElementName() );
}
