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

// Generates the precompiled header for the unit tests.

#include "Pch.h"

void prependTabs(FdoInt32 tabLevel)
{
    for (FdoInt32 i = 0; i < tabLevel; i++)
        wprintf(L"  ");
}

void PrintException(FdoException* exception)
{
	FdoException*	currentException = exception;
	FdoInt32		tabLevel = 0;
	while (currentException != NULL) 
	{
		prependTabs(tabLevel++);
		wprintf(L"%s\n", currentException->GetExceptionMessage());
		if (currentException != exception)
			currentException->Release();
		currentException = currentException->GetCause();
	}
}


