#ifndef _GDBICONNECTION_
#define _GDBICONNECTION_ 1
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
#ifdef _WIN32
#pragma once
#endif


#include "GdbiCommands.h"
#include "GdbiTypes.h"


class GdbiQueryResult;
class GdbiStatement;

class GdbiConnection
{
private:
    rdbi_context_def* m_pRdbiContext;

    GdbiCommands*     m_pGdbiCommands;

    FdoConnectionState   mOpen;

public:
    GdbiConnection( rdbi_context_def* rdbi_context);

    ~GdbiConnection(void);

    FdoConnectionState Open ();

	void Close();

    void SetConnectionString( const wchar_t* connString );

    GdbiCommands*   GetCommands() { return m_pGdbiCommands; }

    int ExecuteNonQuery( const wchar_t* sqlStr, bool isDDL = false );

    int ExecuteNonQuery( const char* sqlStr, bool isDDL = false );

    GdbiQueryResult* ExecuteQuery( const char* sql );

    GdbiQueryResult* ExecuteQuery( const wchar_t* sql );

    GdbiStatement*  Prepare( const wchar_t *sqlStr );

    char IsGeometryFromOrdinatesWanted();

    void SetIsGeometryFromOrdinatesWanted(char * wanted);

	int  GetLastError();

	// Temporarily required by Schema Manager
	rdbi_context_def* GetRdbiContext()
	{
		return m_pRdbiContext;
	}
private:

	GdbiStatement*  Prepare( const char *sqlStr );
    char    mIsGeometryFromOrdinatesWanted;     // 't'=true; 'f'=false; '\0'=unspecified

};

#endif

