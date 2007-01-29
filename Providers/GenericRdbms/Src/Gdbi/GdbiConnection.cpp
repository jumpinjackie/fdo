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
#include "GdbiConnection.h"
#include "GdbiQueryResult.h"
#include "GdbiStatement.h"


GdbiConnection::GdbiConnection( rdbi_context_def* rdbi_context ):
m_pRdbiContext( rdbi_context ),
m_pGdbiCommands( 0 ),
mIsGeometryFromOrdinatesWanted('\0'),
mOpen(FdoConnectionState_Open) // Temporary untill we move all the connection task from the DbiConnection
{
    m_pGdbiCommands = new GdbiCommands( m_pRdbiContext );
}

GdbiConnection::~GdbiConnection(void)
{
    if( m_pGdbiCommands )
        delete m_pGdbiCommands;
}

FdoConnectionState GdbiConnection::Open ()
{
    return mOpen;
}

void GdbiConnection::Close ()
{
	mOpen = FdoConnectionState_Closed;
}

void GdbiConnection::SetConnectionString( const wchar_t* connString )
{
}

GdbiQueryResult* GdbiConnection::ExecuteQuery( const char* sql )
{
    int     qid = -1;

    m_pGdbiCommands->CheckDB();

    if( m_pGdbiCommands->sql( (char*)sql, &qid ) == RDBI_SUCCESS )
    {
        return new GdbiQueryResult( m_pGdbiCommands, qid );
    }

    m_pGdbiCommands->ThrowException();
    return NULL; // to supress a compiler warning
}

GdbiQueryResult* GdbiConnection::ExecuteQuery( const wchar_t* sql )
{
    int     qid = -1;

    m_pGdbiCommands->CheckDB();

    if( m_pGdbiCommands->sql( (wchar_t*)sql, &qid ) == RDBI_SUCCESS )
    {
        return new GdbiQueryResult( m_pGdbiCommands, qid );
    }

    m_pGdbiCommands->ThrowException();
    return NULL; // to supress a compiler warning
}

int GdbiConnection::ExecuteNonQuery( const wchar_t* sqlStr, bool isDDL )
{
    m_pGdbiCommands->CheckDB();
    int rc;
    int qid = -1;
    int row_count = 0;

	if( (rc = m_pGdbiCommands->run_sql( (wchar_t*) sqlStr, isDDL, &row_count )) != RDBI_SUCCESS )
    {
        m_pGdbiCommands->ThrowException();
    }

    return row_count;
}

int GdbiConnection::ExecuteNonQuery( const char* sqlStr, bool isDDL )
{
    m_pGdbiCommands->CheckDB();
    int rc;
    int qid = -1;
    int row_count = 0;

	if( (rc = m_pGdbiCommands->run_sql( (char*) sqlStr, isDDL, &row_count )) != RDBI_SUCCESS )
    {
        m_pGdbiCommands->ThrowException();
    }

    return row_count;
}

GdbiStatement*  GdbiConnection::Prepare( const wchar_t *sqlStr )
{
    m_pGdbiCommands->CheckDB();
    int     qid = -1;

    if( m_pGdbiCommands->sql( (wchar_t*)sqlStr, &qid ) == RDBI_SUCCESS )
    {
        return new GdbiStatement( m_pGdbiCommands, qid );
    }

    m_pGdbiCommands->ThrowException();
    return NULL; // to supress a compiler warning
}

GdbiStatement*  GdbiConnection::Prepare( const char *sqlStr )
{
    m_pGdbiCommands->CheckDB();
    int     qid = -1;

    if( m_pGdbiCommands->sql( (char*)sqlStr, &qid ) == RDBI_SUCCESS )
    {
        return new GdbiStatement( m_pGdbiCommands, qid );
    }

    m_pGdbiCommands->ThrowException();
    return NULL; // to supress a compiler warning
}

char GdbiConnection::IsGeometryFromOrdinatesWanted()
{
    return mIsGeometryFromOrdinatesWanted;
}

void GdbiConnection::SetIsGeometryFromOrdinatesWanted(char * wanted)
{
    if (NULL != wanted)
        mIsGeometryFromOrdinatesWanted = wanted[0];
}

int GdbiConnection::GetLastError()
{
	// Note that this is valid only if there is no other sql execution
	// in parallel. 
	return m_pGdbiCommands->err_stat();
}
