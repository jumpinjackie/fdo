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

#ifndef SLT_SID_H
#define SLT_SID_H

class DBounds;
class SpatialIndex;
class SpatialIndexDescriptor;
class SltConnection;

typedef std::vector<FdoInt64> RowIdList;
typedef std::vector<std::pair<FdoInt64, DBounds> > RowIdxUpdateList;

class SpatialIndexMaxRecordInfo : public FdoIDisposable
{
private:
    bool m_IsReleased;
    SpatialIndexDescriptor* m_spIndexDesc;
public:
    SpatialIndexMaxRecordInfo(SpatialIndexDescriptor* spIndexDesc = NULL): 
      m_spIndexDesc(spIndexDesc), m_IsReleased(false)
    {
        
    }
    virtual ~SpatialIndexMaxRecordInfo()
    {
    }
    //-------------------------------------------------------
    // FdoIDisposable implementation
    //-------------------------------------------------------

    SLT_IMPLEMENT_REFCOUNTING

    //-------------------------------------------------------
    // FdoIConnection implementation
    //-------------------------------------------------------
public:
    bool IsReleased(){return m_IsReleased;}
    void SetReleased(bool value){m_IsReleased = value;}
    void SetSpatialIndexDescriptor(SpatialIndexDescriptor* value){m_spIndexDesc = value;}
    FdoInt64 GetLastInsertedIdx();
    void ForceInsertRowIdToSI(FdoInt64 id);
};

class SpatialIndexDescriptor
{
public:
    SpatialIndexDescriptor(SltConnection* conn, const char* table, SpatialIndex* spIndex); // for classes only
    SpatialIndexDescriptor(SpatialIndex* spIndex, bool bAutoDelSi = true); // for views only
    ~SpatialIndexDescriptor();

    FdoInt64 GetFeatureCount();
    // do not call sqlite3_finalize on this stmt or cache it
    sqlite3_stmt* GetNewFeatures(FdoInt64 lastKnownId);
    
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

    void AddRowIdToDeleteList(FdoInt64 id);
    void AddRowIdToUpdateList(FdoInt64 id);
    void AddRowIdToInsertList(FdoInt64 id);
    void CommitChanges();
    void RollbackChanges();
    bool ChangesAvailable();
    SpatialIndexMaxRecordInfo* GetMaxRecordInfo();
    void SetMaxRecordInfo(SpatialIndexMaxRecordInfo* recInfo);

private:
    sqlite3_stmt* m_sRows;
    sqlite3_stmt* m_sMaxRow;
    sqlite3_stmt* m_sGetRow;
    bool m_bAutoDelSi;
    SpatialIndex* m_spIndex;
    RowIdxUpdateList m_updRows;
    RowIdxUpdateList m_spInsRows;
    RowIdList m_delRows;
    SltConnection* m_conn;
    FdoPtr<SpatialIndexMaxRecordInfo> m_recInfo;
};

#endif