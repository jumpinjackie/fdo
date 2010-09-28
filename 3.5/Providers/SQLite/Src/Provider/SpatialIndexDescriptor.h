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

typedef std::vector<FdoInt64> RowIdList;
typedef std::vector<std::pair<FdoInt64, DBounds> > RowIdxUpdateList;

class SpatialIndexDescriptor
{
public:
    // for classes only
    SpatialIndexDescriptor(SltConnection* conn, const char* table, SpatialIndex* spIndex)
        : m_spIndex(spIndex),
        m_conn(conn),
        m_sRows(NULL),
        m_sMaxRow(NULL),
        m_sGetRow(NULL)
    {
        sqlite3* db = conn->GetDbRead();
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
    SpatialIndexDescriptor(SpatialIndex* spIndex)
        : m_spIndex(spIndex),
        m_sRows(NULL),
        m_sMaxRow(NULL),
        m_sGetRow(NULL)
    {
    }

    ~SpatialIndexDescriptor()
    {
        if (NULL != m_sRows)
            sqlite3_finalize(m_sRows);
        if (NULL != m_sMaxRow)
            sqlite3_finalize(m_sMaxRow);
        if (NULL != m_sGetRow)
            sqlite3_finalize(m_sGetRow);
        delete m_spIndex;
    }

    FdoInt64 GetFeatureCount()
    {
        sqlite3_reset(m_sMaxRow);
        if (sqlite3_step(m_sMaxRow) == SQLITE_ROW)
            return sqlite3_column_int64(m_sMaxRow, 0);
        return -1;
    }

    // do not call sqlite3_finalize on this stmt or cache it
    sqlite3_stmt* GetNewFeatures(FdoInt64 lastKnownId)
    {
        sqlite3_reset(m_sRows);
        sqlite3_bind_int64(m_sRows, 1, lastKnownId);

        return m_sRows;
    }
    
    inline bool IsView()
    {
        return (m_sRows == NULL || m_sMaxRow == NULL);
    }

    inline SpatialIndex* GetSpatialIndex()
    {
        return m_spIndex;
    }

    inline SpatialIndex* DetachSpatialIndex()
    {
        SpatialIndex* retval = m_spIndex;
        m_spIndex = NULL;
        return retval;
    }

    void AddRowIdToDeleteList(FdoInt64 id)
    {
        m_delRows.push_back(id);
    }
    
    void AddRowIdToUpdateList(FdoInt64 id)
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
    
    void CommitChanges()
    {
        for(RowIdList::iterator it = m_delRows.begin(); it < m_delRows.end(); it++)
        {
            m_spIndex->Delete((unsigned)*it);
        }
        m_delRows.clear();
        for(RowIdxUpdateList::iterator it = m_updRows.begin(); it < m_updRows.end(); it++)
        {
            m_spIndex->Update((unsigned)it->first, it->second);
        }
        m_updRows.clear();
    }

    void RollbackChanges()
    {
        m_delRows.clear();
        m_updRows.clear();
    }
    
    bool ChangesAvailable()
    {
        return (m_delRows.size() != 0 || m_updRows.size() != 0);
    }

private:
    sqlite3_stmt* m_sRows;
    sqlite3_stmt* m_sMaxRow;
    sqlite3_stmt* m_sGetRow;
    SpatialIndex* m_spIndex;
    RowIdxUpdateList m_updRows;
    RowIdList m_delRows;
    SltConnection* m_conn;
};

