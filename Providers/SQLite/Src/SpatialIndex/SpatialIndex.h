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

#include "SltGeomUtils.h"

//Each tree node will contain 2^BATCH_SHIFT bounding boxes
#define BATCH_SHIFT 3
#define BATCH_MASK ((~0) << BATCH_SHIFT)

#ifndef _MSC_VER
#ifndef __int64
    typedef long long int __int64;
#endif
#endif

typedef std::map<__int64, unsigned int> LinkMap;
typedef std::vector<__int64> VectorMF;

//We will have up to this many levels
//in the skip list hierarchy
//NOTE: MAX_LEVELS * BATCH_SHIFT must be less than 32,
//otherwise the code needs to be converted to use 64 bit
//array indices.
#define MAX_LEVELS 10

//=============================================
struct Node
{
    Bounds b;
};

//===============================================
class SpatialIndex
{
friend class SpatialIterator;

public:
    SpatialIndex(const wchar_t*); //argument unused, for compatibility with disk backed implementation only
    ~SpatialIndex();
    
    void Insert(__int64 dbId, DBounds& ext);
    void Update(__int64 dbId, DBounds& ext);
    void Delete(__int64 dbId);
    void GetTotalExtent(DBounds& ext);
    void ReOpen(); //for API compatibility with disk-backed index
    __int64 GetLastInsertedIdx() { return _lastInsertedIdx; }

    __int64 operator[](int fid);
private:
    void FullSpatialIndexUpdate();
    void Insert(unsigned fid, Bounds& b);
private:

    // last inserted index
    __int64   _lastInsertedIdx;

    // count of changes
    unsigned   _countChanges;

    //current height of root level
    int        _rootLevel;

    //the skip list arrays, one per level
    Node*      _levels[MAX_LEVELS];

    //allocation sizes per level
    unsigned   _sizes[MAX_LEVELS];

    //item counts per level
    unsigned   _counts[MAX_LEVELS];

    //translation from double precision space
    //to local space (which we keep in float)
    double     _offset[SI_DIM];
    bool       _haveOffset;

    // used to link ID with SI id
    LinkMap    _linkMap;
    VectorMF    _backMap;
    unsigned   _positionIdx;
};

//==============================================
class SpatialIterator
{
public:

    SpatialIterator(DBounds& ext, SpatialIndex* si);

    void Reset();

    bool NextRange(int& start, int& end);

    __int64 operator[](int fid);

private:

    SpatialIndex* _si;
    int _prevStopIndex;
    int _prevStopLevel;

    //allocate enough to be able to align the bounds
    //on a 16 byte boundary
    char _barr[sizeof(Bounds)*2]; 
    Bounds* _b;
};

class SpatialIteratorStep
{
public:
    SpatialIteratorStep(SpatialIterator* siit)
    {
        m_siit = siit;
        m_curfid = 0; //position prior to first record
        m_siEnd = -1;
    }
    ~SpatialIteratorStep()
    {
        delete m_siit;
    }
    __int64 ReadNext()
    {
        m_curfid++;
        if (m_curfid >= m_siEnd)
        {
            int start;
            //spatial reader is done, so we are done
            if (m_siit == NULL || !m_siit->NextRange(start, m_siEnd))
                return -1;
            m_curfid = (__int64)(start ? start : 1); //make sure we skip fid=0, which is not valid
        }
        return (*m_siit)[(int)m_curfid];
    }
    void Reset()
    {
        m_curfid = 0; //position prior to first record
        m_siEnd = -1;
        if (m_siit != NULL)
            m_siit->Reset();
    }
private:
    int m_curfid;
    int m_siEnd;
    SpatialIterator* m_siit;
};
