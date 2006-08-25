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
 *
 */
#include "stdafx.h"
#include "GdbiStatement.h"
#include "GdbiCommands.h"
#include "FdoCommonOSUtil.h"


GdbiStatement::GdbiStatement(GdbiCommands* command, int qid):
m_pGdbiCommands( command )
{
	m_QueryId = new GdbiQueryIdentifier(command, qid);
}

GdbiStatement::~GdbiStatement(void)
{
	FDO_SAFE_RELEASE(m_QueryId);
}

int GdbiStatement::Bind( int parmIndex, int size, const char* szValue, GDBI_NI_TYPE *nullInd )
{
	if (size == 1)
		return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_CHAR,  size, (char*)szValue, nullInd );
	else
		return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_STRING,  size, (char*)szValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, int size, const wchar_t* szValue, GDBI_NI_TYPE *nullInd )
{
	return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_WSTRING,  size, (char*)szValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, short *sValue, GDBI_NI_TYPE *nullInd )
{
	return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_SHORT,  sizeof(short), (char*)sValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, int *nValue, GDBI_NI_TYPE *nullInd )
{
	return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_INT,  sizeof(int), (char*)nValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, double *dValue, GDBI_NI_TYPE *nullInd )
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_DOUBLE,  sizeof(double), (char*)dValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, float *fValue, GDBI_NI_TYPE *nullInd )
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_FLOAT,  sizeof(float), (char*)fValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, FdoInt64 *i64Value, GDBI_NI_TYPE *nullInd )
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_LONGLONG,  sizeof(FdoInt64), (char*)i64Value, nullInd );
}

int GdbiStatement::Bind( int parmIndex, FdoIGeometry* gValue, GDBI_NI_TYPE *nullInd )
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), RDBI_GEOMETRY,  sizeof(FdoIGeometry), (char*)gValue, nullInd );
}

int GdbiStatement::Bind( int parmIndex, FdoDateTime *dValue, GDBI_NI_TYPE *nullInd )
{
    return 1;
}

int GdbiStatement::Bind(
        int parmIndex,
        int   datatype,     /* A data type from Inc/rdbi.h              */
        int   size,         /* binary size                              */
        char *address,      /* data address                             */
        GDBI_NI_TYPE *null_ind     /* pointer to null indicator variables      */
            )
{
    return m_pGdbiCommands->bind( m_QueryId->GetQueryId(), FdoCommonOSUtil::itoa(parmIndex, buffer), datatype,  size, address, null_ind );
}

int GdbiStatement::BindNull( int parmIndex )
{

    return 1;
}

int GdbiStatement::ParmCount( )
{

    return 1;
}

int GdbiStatement::ExecuteNonQuery( )
{
    if( m_pGdbiCommands->execute( m_QueryId->GetQueryId() ) == RDBI_SUCCESS )
    {
        return m_pGdbiCommands->crsr_nrows( m_QueryId->GetQueryId());
    }

    return 0;
}

GdbiQueryResult*  GdbiStatement::ExecuteQuery( )
{
	if( m_QueryId )
	{
        GdbiQueryResult* ret = new GdbiQueryResult( m_pGdbiCommands, m_QueryId);
		return ret;
	}

    throw new GdbiException(L"Not a valid query");
}

int GdbiStatement::Free()
{
	
	FDO_SAFE_RELEASE(m_QueryId);
	return RDBI_SUCCESS;
}

bool GdbiStatement::SupportsInt64Binding()
{ 
	if (m_QueryId)
        return (m_pGdbiCommands->SupportsInt64Binding());
	throw new GdbiException(L"Not a valid query");
}

int GdbiStatement::set_array_size(int arraySize)
{
	if (m_QueryId)	{
		m_pGdbiCommands->set_array_size(arraySize);
		return RDBI_SUCCESS;
	}
	throw new GdbiException(L"Not a valid query");
}
