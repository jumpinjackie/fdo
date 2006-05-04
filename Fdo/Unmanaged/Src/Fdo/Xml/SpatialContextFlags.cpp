// 
//  
//  Copyright (C) 2004-2006  Autodesk, Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  
#include <Std.h>
#include <Fdo/Xml/SpatialContextFlags.h>

FdoXmlSpatialContextFlags* FdoXmlSpatialContextFlags::Create( 
	FdoString*				url,
	ErrorLevel				errorLevel,
	FdoBoolean				nameAdjust,
	ConflictOption		    conflictOption,
	FdoBoolean				includeDefault
)
{
    return new FdoXmlSpatialContextFlags( url, errorLevel, nameAdjust, conflictOption, includeDefault );
}

FdoXmlSpatialContextFlags::FdoXmlSpatialContextFlags( 
	FdoString*				url,
	ErrorLevel				errorLevel,
	FdoBoolean				nameAdjust,
	ConflictOption		    conflictOption,
	FdoBoolean				includeDefault
) : FdoXmlFlags( url, errorLevel, nameAdjust ) 
{
    SetConflictOption( conflictOption );
    SetIncludeDefault( includeDefault );
}

FdoXmlSpatialContextFlags::~FdoXmlSpatialContextFlags() 
{
}

void FdoXmlSpatialContextFlags::SetConflictOption( FdoXmlSpatialContextFlags::ConflictOption conflictOption )
{
    mConflictOption = conflictOption;
}

FdoXmlSpatialContextFlags::ConflictOption FdoXmlSpatialContextFlags::GetConflictOption() const
{
    return mConflictOption;
}

void FdoXmlSpatialContextFlags::SetIncludeDefault( FdoBoolean includeDefault )
{
    mIncludeDefault = includeDefault;
}

FdoBoolean FdoXmlSpatialContextFlags::GetIncludeDefault() const
{
    return mIncludeDefault;
}

