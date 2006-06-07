#ifndef _GDBIQUERYIDENTIFIER_
#define _GDBIQUERYIDENTIFIER_
/*
 * Copyright (C) 2004-2005  Autodesk, Inc.
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

#include "stdafx.h"
#include "GdbiCommands.h"

class GdbiCommands;

class GdbiQueryIdentifier : public GisIDisposable
{
private:
	int mQueryId;
	GdbiCommands*     mpGdbiCommands;
public:

	GdbiQueryIdentifier(GdbiCommands* command, int qId): mpGdbiCommands( command ), 
		                                                 mQueryId(qId)
	{
	}

	int GetQueryId()
	{
		return mQueryId;
	}
	
	void Dispose ()
	{
		mpGdbiCommands->free_cursor( mQueryId );
		delete this;
	}
protected:
	~GdbiQueryIdentifier(void)
	{
	}
};

#endif // _GDBIQUERYIDENTIFIER_
