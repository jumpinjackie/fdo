// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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

#include "stdafx.h"
#include "slt.h"
#include "SltSpatialContextReader.h"
#include "SltProvider.h"
#include "SltGeomUtils.h"
#include "SltConversionUtils.h"
#include "StringUtil.h"

SltSpatialContextReader::SltSpatialContextReader(SltConnection* conn)
{
    m_bHasScName = false;
    m_connection = conn;
    m_connection->AddRef();

    sqlite3* db = m_connection->GetDbConnection();

    //We will attempt two ways to get the info -- one is in case
    //there is an sr_name column (i.e. data was created by FDO),
    //and the other in case there is no sr_name column
    const char* sql1 = "SELECT srid,auth_srid,srtext,sr_name FROM spatial_ref_sys;";
    const char* sql2 = "SELECT srid,auth_srid,srtext FROM spatial_ref_sys;";

    m_pStmt = NULL;
    const char* zTail = NULL;
    int rc = SQLITE_OK;
    if (sqlite3_prepare_v2(db, sql1, -1, &m_pStmt, &zTail) == SQLITE_OK)
        m_bHasScName = true;
    else if ((rc = sqlite3_prepare_v2(db, sql2, -1, &m_pStmt, &zTail)) != SQLITE_OK)
    {
        const char* err = sqlite3_errmsg(db);
        if (err != NULL)
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);
        else
            throw FdoException::Create(L"Query of spatial_ref_sys table failed.", rc);
    }
}

SltSpatialContextReader::~SltSpatialContextReader()
{
    if (m_pStmt)
        sqlite3_finalize(m_pStmt);
    m_connection->Release();
}

void SltSpatialContextReader::Dispose()
{
    delete this;
}

FdoString* SltSpatialContextReader::GetName()
{
    //First, check if we stored an FDO name for spatial context
    //in the sr_name column. If not, we will fall back to using
    //the SRID as the name.
    const char* srname = NULL;
    
    if (m_bHasScName)
        srname = (const char*)sqlite3_column_text(m_pStmt, 3);
    
    if (srname && *srname)
    {
        //sr_name is non-empty, use it.
        m_name = A2W_SLOW(srname);
    }
    else
    {
        //sr_name is empty, use SRID as name instead
        int name = sqlite3_column_int(m_pStmt, 0);
        wchar_t wname[16];
        swprintf(wname, 16, L"%d", name);
        m_name = wname;
    }

    return m_name.c_str();
}

FdoString* SltSpatialContextReader::GetDescription()
{
    return L"";
}

FdoString* SltSpatialContextReader::GetCoordinateSystem()
{
    const char* auth_srid = (const char*)sqlite3_column_text(m_pStmt, 1);
    m_cs = auth_srid ? A2W_SLOW(auth_srid) : L"";
    return m_cs.c_str();
}

FdoString* SltSpatialContextReader::GetCoordinateSystemWkt()
{
    const char* wkt = (const char*)sqlite3_column_text(m_pStmt, 2);
    m_wkt = wkt ? A2W_SLOW(wkt) : L"";
    return m_wkt.c_str();
}

FdoSpatialContextExtentType SltSpatialContextReader::GetExtentType()
{
    return FdoSpatialContextExtentType_Dynamic;
}

FdoByteArray* SltSpatialContextReader::GetExtent()
{
    FdoString* myname = GetName();
    double ext[4];
    ext[0] = ext[1] = DBL_MAX;
    ext[2] = ext[3] = -DBL_MAX;
    StringBuffer sb(100);
    int defaultSpatialContext = m_connection->GetDefaultSpatialContext();
    int spContext = m_connection->FindSpatialContext(myname, defaultSpatialContext);
    if (defaultSpatialContext != spContext)
    {
        sb.Append("SELECT f_table_name FROM geometry_columns WHERE srid=", 53);
        sb.Append(spContext);
        sb.Append(";", 1);
    }
    else
    {
        sb.Append("SELECT geometry_columns.f_table_name from geometry_columns, spatial_ref_sys WHERE geometry_columns.srid=0", 105);
        if (spContext != 0)
        {
            sb.Append(" OR geometry_columns.srid=", 26);
            sb.Append(spContext);
        }
        sb.Append(" OR (geometry_columns.srid NOT IN (SELECT spatial_ref_sys.srid FROM spatial_ref_sys));", 86);
    }
    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    if (sqlite3_prepare_v2(m_connection->GetDbConnection(), sb.Data(), -1, &pstmt, &pzTail) == SQLITE_OK)
    {
        while (sqlite3_step(pstmt) == SQLITE_ROW)
        {
            const char* colName = (const char*)sqlite3_column_text(pstmt, 0);
            if (colName != NULL && *colName != '\0' && m_connection->GetExtents(A2W_SLOW(colName).c_str(), ext))
                break;
        }
        sqlite3_finalize(pstmt);
    }

    // in case the extent is "empty" return null
    if (*ext > *(ext+2))
        return NULL;

    FgfPolygon p;

    //generate FGF polygon and return as refcounted byte array
    p.p[0] = ext[0];    p.p[1] = ext[1];
    p.p[2] = ext[2];    p.p[3] = ext[1];
    p.p[4] = ext[2];    p.p[5] = ext[3];
    p.p[6] = ext[0];    p.p[7] = ext[3];
    p.p[8] = ext[0];    p.p[9] = ext[1];

    return FdoByteArray::Create((const unsigned char*)&p, sizeof(p));
}

const double SltSpatialContextReader::GetXYTolerance()
{
    return 0.0;
}

const double SltSpatialContextReader::GetZTolerance()
{    
    return 0.0;
}

const bool SltSpatialContextReader::IsActive()
{
    return false;
}

bool SltSpatialContextReader::ReadNext()
{
    if (sqlite3_step(m_pStmt) == SQLITE_ROW)
        return true;

    return false;
}

