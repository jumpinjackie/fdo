// 
//  
//  Copyright (C) 2008-2011 Autodesk Inc.
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

//#define USE_RTREE

#ifdef USE_RTREE
    #include "SpatialIndex/rtree.h"
    typedef bvh::rtree SltSpatialIndex;
    typedef bvh::rtree_iterator SpatialIteratorImpl;
    #include "VectorMF.h"
#else
    #ifndef _MSC_VER
        #include "SpatialIndex.h"
    #else
        #include "DiskSpatialIndex.h"
    #endif
    typedef SpatialIndex SltSpatialIndex;
    typedef SpatialIterator SpatialIteratorImpl;
#endif

class SltSpatialIterator;

class SpatialIndexDescriptor : public FdoIDisposable
{
public:
    SpatialIndexDescriptor(const char* tableName, SltSpatialIndex* spIndex, bool bAutoDelSi = true);
    ~SpatialIndexDescriptor();

    SLT_IMPLEMENT_REFCOUNTING

public:

    inline SltSpatialIndex* DetachSpatialIndex()
    {
        SltSpatialIndex* retval = m_spIndex;
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

    inline double GetXYTolerance() { return m_xyTolerance; }
    inline void SetXYTolerance(double xyTolerance) { m_xyTolerance = xyTolerance; }
    
    inline double GetZTolerance() { return m_zTolerance; }
    inline void SetZTolerance(double zTolerance) { m_zTolerance = zTolerance; }

    inline bool SupportsTolerance() { return (m_xyTolerance>0.0); }

    void Enumerate(VectorMF* res, const DBounds& bbox);
    void Insert(__int64 id, const DBounds& ext);
    void Delete(__int64 id, const DBounds& ext);
    void Delete(__int64 id, const unsigned char* blob, int len);
    void Update(__int64 id, const DBounds& before, const DBounds& after);
    void GetTotalExtent(DBounds& ext);
    void ReOpen();
    void Reset();
    SltSpatialIterator* GetIterator(const DBounds& bbox);

private:
    bool m_bAutoDelSi;
    SltSpatialIndex* m_spIndex;
    bool m_bHasUpdates;
    bool m_IsReleased;
    std::string m_tablename;
    double m_xyTolerance;
    double m_zTolerance;
};

class SltSpatialIterator
{
public:
    SltSpatialIterator(SpatialIteratorImpl* impl);
    ~SltSpatialIterator();

    __int64 Next();
    void Reset();

private:
    SpatialIteratorImpl* m_impl;

#ifdef USE_RTREE
#else
    int m_curfid;
    int m_siEnd;
#endif
};


#endif