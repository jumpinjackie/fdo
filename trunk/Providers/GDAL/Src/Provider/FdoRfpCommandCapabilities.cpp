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
//

#include "FDORFP.h"
#include "FdoRfpCommandCapabilities.h"

FdoRfpCommandCapabilities::FdoRfpCommandCapabilities()
{
}

FdoRfpCommandCapabilities::~FdoRfpCommandCapabilities()
{
}

int* FdoRfpCommandCapabilities::GetCommands(int& size)
{
	static const int commands[] = 
		{
			FdoCommandType_Select,
			FdoCommandType_SelectAggregates,
	        FdoCommandType_DescribeSchema,
			FdoCommandType_DescribeSchemaMapping,
		    FdoCommandType_GetSpatialContexts
	    };

	size = sizeof(commands)/sizeof(int);
	return (int*) commands;
}

bool FdoRfpCommandCapabilities::SupportsParameters()
{
	return false;
}

bool FdoRfpCommandCapabilities::SupportsTimeout()
{
	return false;
}

void FdoRfpCommandCapabilities::Dispose()
{
	delete this;
}
