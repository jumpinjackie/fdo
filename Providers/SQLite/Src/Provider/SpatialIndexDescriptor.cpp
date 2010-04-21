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
#include "StringUtil.h"
#include "SltProvider.h"
#ifndef _MSC_VER
#include "SpatialIndex.h"
#else
#include "DiskSpatialIndex.h"
#endif
#include "SpatialIndexDescriptor.h"

FdoInt64 SpatialIndexMaxRecordInfo::GetLastInsertedIdx()
{
    return (m_IsReleased || !m_spIndexDesc) ? -1 : m_spIndexDesc->GetSpatialIndex()->GetLastInsertedIdx();
}

void SpatialIndexMaxRecordInfo::ForceInsertRowIdToSI(FdoInt64 id)
{
    m_spIndexDesc->AddRowIdToInsertList(id);
    // in this case we commit the changes since other selects could need them
    m_spIndexDesc->CommitChanges();
}

// for classes only
SpatialIndexDescriptor::SpatialIndexDescriptor(SltConnection* conn, const char* table, SpatialIndex* spIndex)
    : m_spIndex(spIndex),
    m_conn(conn),
    m_sRows(NULL),
    m_sMaxRow(NULL),
    m_sGetRow(NULL),
    m_bAutoDelSi(true)
{
    sqlite3* db = conn->GetDbConnection();
    StringBuffer sb;
    sb.Append("SELECT * FROM ", 14);
    sb.AppendDQuoted(table);
    sb.Append(" WHERE ROWID>?;", 15);
    const char* tail = NULL;
    int rc = sqlite3_prepare_v2(db, sb.Data(), -1, &m_sRows, &tail);
    sb.Reset();
    
    sb.Append("SELECT * FROM ", 14);
    sb.AppendDQuoted(table);
    sb.Append(" WHERE ROWID=?;", 15);
    tail = NULL;
    rc = sqlite3_prepare_v2(db, sb.Data(), -1, &m_sGetRow, &tail);
    sb.Reset();
    
    sb.Append("SELECT MAX(ROWID) FROM ", 23);
    sb.AppendDQuoted(table);
    sb.Append(";", 1);
    rc = sqlite3_prepare_v2(db, sb.Data(), -1, &m_sMaxRow, &tail);
}

// for views only
SpatialIndexDescriptor::SpatialIndexDescriptor(SpatialIndex* spIndex, bool bAutoDelSi)
    : m_spIndex(spIndex),
    m_sRows(NULL),
    m_sMaxRow(NULL),
    m_sGetRow(NULL),
    m_bAutoDelSi(bAutoDelSi)
{
}

SpatialIndexDescriptor::~SpatialIndexDescriptor()
{
    if (m_recInfo != NULL)
        m_recInfo->SetReleased(true);
    if (NULL != m_sRows)
        sqlite3_finalize(m_sRows);
    if (NULL != m_sMaxRow)
        sqlite3_finalize(m_sMaxRow);
    if (NULL != m_sGetRow)
        sqlite3_finalize(m_sGetRow);
    if (m_bAutoDelSi)
        delete m_spIndex;
}

FdoInt64 SpatialIndexDescriptor::GetFeatureCount()
{
    sqlite3_reset(m_sMaxRow);
    if (sqlite3_step(m_sMaxRow) == SQLITE_ROW)
        return sqlite3_column_int64(m_sMaxRow, 0);
    return -1;
}

// do not call sqlite3_finalize on this stmt or cache it
sqlite3_stmt* SpatialIndexDescriptor::GetNewFeatures(FdoInt64 lastKnownId)
{
    sqlite3_reset(m_sRows);
    sqlite3_bind_int64(m_sRows, 1, lastKnownId);

    return m_sRows;
}

void SpatialIndexDescriptor::AddRowIdToDeleteList(FdoInt64 id)
{
    m_delRows.push_back(id);
}

void SpatialIndexDescriptor::AddRowIdToUpdateList(FdoInt64 id)
{
    DBounds ext;
    sqlite3_reset(m_sGetRow);
    sqlite3_bind_int64(m_sGetRow, 1, id);
    
    if (sqlite3_step(m_sGetRow) == SQLITE_ROW)
    {
        FdoInt64 idToAdd = sqlite3_column_int64(m_sGetRow, 0);
        const unsigned char* ptr = (const unsigned char*)sqlite3_column_blob(m_sGetRow, 1);
        int len = sqlite3_column_bytes(m_sGetRow, 1);
        m_conn->GetGeometryExtent(ptr, len, &ext);
    }

    m_updRows.push_back(std::make_pair(id, ext));
}

void SpatialIndexDescriptor::AddRowIdToInsertList(FdoInt64 id)
{
    DBounds ext;
    sqlite3_reset(m_sGetRow);
    sqlite3_bind_int64(m_sGetRow, 1, id);
    
    if (sqlite3_step(m_sGetRow) == SQLITE_ROW)
    {
        FdoInt64 idToAdd = sqlite3_column_int64(m_sGetRow, 0);
        const unsigned char* ptr = (const unsigned char*)sqlite3_column_blob(m_sGetRow, 1);
        int len = sqlite3_column_bytes(m_sGetRow, 1);
        m_conn->GetGeometryExtent(ptr, len, &ext);
    }

    m_spInsRows.push_back(std::make_pair(id, ext));
}

void SpatialIndexDescriptor::CommitChanges()
{
    for(RowIdList::iterator it = m_delRows.begin(); it < m_delRows.end(); it++)
    {
        m_spIndex->Delete(*it);
    }
    m_delRows.clear();
    for(RowIdxUpdateList::iterator it = m_updRows.begin(); it < m_updRows.end(); it++)
    {
        m_spIndex->Update(it->first, it->second);
    }
    m_updRows.clear();
    for(RowIdxUpdateList::iterator it = m_spInsRows.begin(); it < m_spInsRows.end(); it++)
    {
        m_spIndex->Insert(it->first, it->second);
    }
    m_spInsRows.clear();
}

void SpatialIndexDescriptor::RollbackChanges()
{
    m_delRows.clear();
    m_updRows.clear();
    m_spInsRows.clear();
}
    
bool SpatialIndexDescriptor::ChangesAvailable()
{
    return (m_delRows.size() != 0 || m_updRows.size() != 0 || m_spInsRows.size() != 0);
}

SpatialIndexMaxRecordInfo* SpatialIndexDescriptor::GetMaxRecordInfo()
{
    return FDO_SAFE_ADDREF(m_recInfo.p);
}

void SpatialIndexDescriptor::SetMaxRecordInfo(SpatialIndexMaxRecordInfo* recInfo)
{
    if (m_recInfo != NULL)
        m_recInfo->SetReleased(true);

    m_recInfo = FDO_SAFE_ADDREF(recInfo);
}
