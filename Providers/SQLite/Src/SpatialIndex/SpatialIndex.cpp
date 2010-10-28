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
#include "SltGeomUtils.h"
#include "SpatialIndex.h"
#include <cstring>

#ifndef _MSC_VER

#define _aligned_free free

void* _aligned_malloc(size_t size, size_t alignment)
{
    void* ret = 0;
    int res = posix_memalign(&ret, alignment, size);
    return ret;
}

#endif

//====================================================================
// This is a spatial skip list structure. 
// It is built on the fly, and makes the assumption that
// the features are somewhat spatially coherent when ordered 
// by FID. It is then capable of returning results from a spatial
// query incrementally, and ordered by FID for faster
// scan through the feature data file.
//====================================================================
SpatialIndex::SpatialIndex(const wchar_t*)
{
    _positionIdx = 1;
    _lastInsertedIdx = 0;
    _countChanges = 0;
    _haveOffset = false;
    _rootLevel = 0;
    memset(_sizes, 0, sizeof(_sizes));
    memset(_counts, 0, sizeof(_sizes));
    memset(_levels, 0, sizeof(_levels));
    memset(_offset, 0, sizeof(_offset));
}

SpatialIndex::~SpatialIndex()
{
    for (int i=0; i<MAX_LEVELS; i++)
        _aligned_free(_levels[i]);
}

static Bounds EMPTY_BOX(true);

// Function used to reset to empty the SI without free the allocated memory
// We can re-use it to avoid free-reallocate
void SpatialIndex::ResetToEmpty()
{
    _positionIdx = 1;
    _lastInsertedIdx = 0;
    _countChanges = 0;

    _haveOffset = false;
    _rootLevel = 0;

    memset(_counts, 0, sizeof(_sizes));
    memset(_levels, 0, sizeof(_levels));
    memset(_offset, 0, sizeof(_offset));
}

void SpatialIndex::Insert(__int64 dbId, DBounds& ext)
{
    _linkMap[dbId] = _positionIdx;
    if (_positionIdx >= _backMap.size()) // allocate a bit more
        _backMap.insert(_backMap.end(), _positionIdx-_backMap.size()+8, 0);
    _backMap[_positionIdx-1] = dbId;

    unsigned int fid = _positionIdx;
    //check if we have got a local offset
    //If not yet, then use the given bounds
    //to set it up
    if (!_haveOffset)
    {
        for (int i=0; i<SI_DIM; i++)
            _offset[i] = ext.min[i];

        _haveOffset = true;
    }

    //translate the given bounds to local space
    Bounds b;
    TranslateBounds(&ext, _offset, &b);
    Insert(fid, b);

    // avoid a update to change this value since update will provide 
    // fid < _lastInsertedIdx and we will force SI update
    if (dbId > _lastInsertedIdx)
        _lastInsertedIdx = dbId;
    _positionIdx++;
}

void SpatialIndex::Insert(unsigned int fid, Bounds& b)
{
    //insert into the skip lists
    unsigned int index = fid;
    unsigned int oldIndex = index;
    unsigned int* counts = _counts;
    Node** levels = _levels;

    for (int i=0; i<MAX_LEVELS; i++)
    {
        //make sure the current levels array is big enough
        if (_sizes[i] <= index)
        {
            //double the arrays when resizing -- wasteful but fast.
            int nsize = 2 * index + 1; 
            Node* n = (Node*)_aligned_malloc(nsize * sizeof(Node), 16);
            
            //copy existing data into newly allocated array
            //and free the old array
            memcpy(n, levels[i], sizeof(Node) * counts[i]);
            _aligned_free(levels[i]);
 
            //fill newly allocated section with empty boxes
            FillMem(&n[counts[i]].b, &EMPTY_BOX, nsize - counts[i]);

            _sizes[i] = nsize;
            levels[i] = n;
        }

        Node& n = levels[i][index];

        if (index >= counts[i]) 
            counts[i] = index+1;

        //expand bounds of node at this level
        Bounds::Add(&n.b, &b); 
        
        if (index == 0)
        {
            //update root level (it's the one where we there 
            //is a single node, with index 0) and then bail out.
            if (i > _rootLevel)
            {
                _rootLevel = i;
                
                //The node we just added forced the root up one level
                //So we need to add the bounds of the old root to the
                //bounds of the new node to make up the new root node
                //TODO: I have some reservations with this code -- it
                //needs a further walkthrough.
                for (unsigned int j=0; j<oldIndex; j++)
                {
                    Node& oldRoot = levels[i-1][j];
                    Bounds::Add(&n.b, &oldRoot.b);
                }
            }
            else if (fid < _positionIdx)
            {
                // in case we do an update we need to propagate the value till root
                index = (unsigned int)fid;
                // i=1 jump over the first level
                for (int j=1; j<=_rootLevel; j++)
                {
                    index = index >> BATCH_SHIFT;
                    Node* upperRoot = &levels[j][index];
                    Bounds::Add(&upperRoot->b, &b);
                }
            }
            break;
        }

        oldIndex = index;

        //index of Node containing our bounds at next skip level
        index = index >> BATCH_SHIFT; 
    }
}

void SpatialIndex::Update(__int64 dbId, DBounds& ext)
{
    // avoid getting NaN added to the SI when we update a geometry to null
    if (ext.IsEmpty())
    {
        Delete(dbId);
        return;
    }

    LinkMap::iterator it = _linkMap.find(dbId);
    if (it == _linkMap.end())
    {
        // avoid out-of-sync in case user update rowid only
        Insert(dbId, ext);
        return;
    }
    // we can use insert, however we need to count number of changes 
    // and later force a rebuild when the number of changes becomes too high
    unsigned int fid = it->second;
    //check if we have got a local offset
    //If not yet, then use the given bounds
    //to set it up
    if (!_haveOffset)
    {
        for (int i=0; i<SI_DIM; i++)
            _offset[i] = ext.min[i];

        _haveOffset = true;
    }

    //translate the given bounds to local space
    Bounds b;
    TranslateBounds(&ext, _offset, &b);
    Insert(fid, b);
    _countChanges++;
    unsigned int tproc = (unsigned int)(_positionIdx/10.0);
    if (tproc && _countChanges > tproc)
        FullSpatialIndexUpdate();
}

void SpatialIndex::Delete(__int64 dbId)
{
    LinkMap::iterator it = _linkMap.find(dbId);
    if (it == _linkMap.end())
        return;
    //insert into the skip lists
    unsigned int index = it->second;
    Node** levels = _levels;

    if (_sizes[0] > index)
    {
        Node* n = &((Node*)levels[0])[index];
        n->b = EMPTY_BOX;
        _countChanges++;
        unsigned int tproc = (unsigned int)(_positionIdx/10.0);
        if (tproc && _countChanges > tproc)
            FullSpatialIndexUpdate();
    }
}

__int64 SpatialIndex::operator[](int fid)
{
    return _backMap[fid-1];
}

void SpatialIndex::FullSpatialIndexUpdate()
{
    _countChanges = 0;
    Node* n;
    //insert into the skip lists
    unsigned int* counts = _counts;
    Node** levels = _levels;

    for (int i=1; i<MAX_LEVELS; i++)
    {
        // clear the top levels
        if (counts[i] != 0)
            FillMem(&levels[i]->b, &EMPTY_BOX, counts[i]);
    }

    for (unsigned int y = 0; y < counts[0]; y++)
    {
        n = &((Node*)levels[0])[y];
        Insert(y, n->b);
    }
}

//for API compatibility with disk-backed index
void SpatialIndex::ReOpen()
{
} 

void SpatialIndex::GetTotalExtent(DBounds& ext)
{
    //Is the spatial index empty? Return some default
    //extent.
    //TODO: what exactly is a good empty extent? Perhaps
    //it is coord sys dependent
    if (_counts[0] == 0)
    {
        ext.SetEmpty();
        return;
    }
    
    Bounds* b = &_levels[_rootLevel][0].b;

    for (int i=0; i<SI_DIM; i++)
    {
        ext.min[i] = (double)b->min[i] + _offset[i];
        ext.max[i] = (double)b->max[i] + _offset[i];
    }
}

//====================================================================
// The skip list search structure...
// Customized to return ranges of valid ids, so we do not
// have to ask it one by one.
//====================================================================
SpatialIterator::SpatialIterator(DBounds& ext, SpatialIndex* si)
{
    _si = si;

    //align search Bounds member var on 16 byte boundary
    //NOTE: this adds 16 when the original address is 
    //already aligned but we don't care since we allocated enough.
    char* tmp = _barr;
    tmp += (0x10 - ((unsigned long)tmp & 0xF)); 
    _b = (Bounds*)tmp;

    //translate to the normalized space
    //of the spatial index
    TranslateBounds(&ext, _si->_offset, _b);

    Reset();
}

void SpatialIterator::Reset()
{
    _prevStopIndex = 0;
    _prevStopLevel = _si->_rootLevel;
}

bool SpatialIterator::NextRange(int& start, int& end)
 {
    //start search for next match at the next node of the level 
    //that we left off last time we were called
    start = -1;
    int prevStopIndex = _prevStopIndex;
    int prevStopLevel = _prevStopLevel;
    int sz = _si->_counts[0];
    Bounds* mb = _b;
    
    Bounds* b;
    int intres;
        
    //build up a new continuous range of matches
    while(1)
    {
        end = prevStopIndex << (BATCH_SHIFT * prevStopLevel);

        //have we reached the end of the search?
        if (end >= sz)
        {
            if (start != -1)
            {
                end = sz;
                goto done;
            }
            else
            {
                start = end = -1;
                return false;
            }
        }

        //get next node we need to check
        b = &((&_si->_levels[prevStopLevel][prevStopIndex])->b);
        intres = Bounds::Disjoint(mb, b);

        if (intres) 
        {
            //bounds are disjoint (do not intersect)
            //reached a break in the hit sequence,
            //return current range to caller
            if (start != -1) goto done;
        }
        else
        {
            if (prevStopLevel == 0)
            {
                //bounds either intersect or one is contained in the other
                //If we are working on level 0, both of these conditions 
                //indicate a positive hit
                if (start == -1) start = end;
            }
            else
            {
                //if we are not on level 0, we need to distinguish between
                //intersection and containment -- if the bounds is fully
                //contained in the search bounds, we can return the whole
                //range of ids covered by the contained bounds

                int intersects = Bounds::NotContain(mb, b);

                if (intersects)
                {
                    //case where more granular lower nodes may contain
                    //a positive hit -- we need to drop one level lower
                    //and check there

                    prevStopLevel--; //move vertically (down)
                    prevStopIndex <<= BATCH_SHIFT;
                    continue; //bail now, since we don't want to hit the code below
                              //that moves us up a level (because we just moved down one)
                }
                else
                {
                    //case where search bounds fully contains
                    //the current node bounds
                    if (start == -1) start = end;
                }
            }
        }

        //move to the next node horizontally...
        prevStopIndex++;

        //have we reached a point where we can move up to a less
        //granular level? Move vertically (up) as many levels
        //as possible
        while ((prevStopIndex & ~BATCH_MASK) == 0 
            && (prevStopLevel < _si->_rootLevel))
        {
            prevStopLevel++;
            prevStopIndex >>= BATCH_SHIFT;
        }

        continue;
done:
        _prevStopIndex = prevStopIndex;
        _prevStopLevel = prevStopLevel;
        return true;
    }
}

__int64 SpatialIterator::operator[](int fid)
{
    return (*_si)[fid];
}
