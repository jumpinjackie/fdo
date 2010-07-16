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

class SpatialIndex;

class SpatialIndexDescriptor : public FdoIDisposable
{
public:
    SpatialIndexDescriptor(const char* tableName, SpatialIndex* spIndex, bool bAutoDelSi = true);
    ~SpatialIndexDescriptor();

    SLT_IMPLEMENT_REFCOUNTING

public:
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

    const char* GetTableName() { return m_tablename.c_str(); }
    void SetTableName(const char* tname) { m_tablename = tname; }
    
    inline bool GetChangesAvailable() {return m_bHasUpdates;}
    inline void SetChangesAvailable(bool bHasUpdates) {m_bHasUpdates = bHasUpdates;}

    inline bool IsReleased(){return m_IsReleased;}
    inline void SetReleased(bool value){m_IsReleased = value;}
    inline bool IsDeletedAfterRelease() {return (m_refCount==1);};

private:
    bool m_bAutoDelSi;
    SpatialIndex* m_spIndex;
    bool m_bHasUpdates;
    bool m_IsReleased;
    std::string m_tablename;
};

#endif