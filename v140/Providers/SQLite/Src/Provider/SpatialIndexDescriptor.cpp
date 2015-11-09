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

#include "stdafx.h"
#include "StringUtil.h"
#include "SltProvider.h"
#include "SltGeomUtils.h"

#include "SpatialIndexDescriptor.h"

SpatialIndexDescriptor::SpatialIndexDescriptor(const char* tableName, SltSpatialIndex* spIndex, bool bAutoDelSi)
    : m_spIndex(spIndex),
    m_tablename(tableName),
    m_refCount(1),
    m_bHasUpdates(false),
    m_IsReleased(false),
    m_bAutoDelSi(bAutoDelSi),
    m_xyTolerance(0.0),
    m_zTolerance(0.0)
{
}

SpatialIndexDescriptor::~SpatialIndexDescriptor()
{
    if (m_bAutoDelSi)
        delete m_spIndex;
}


void SpatialIndexDescriptor::Insert(__int64 id, const DBounds& ext)
{
#ifdef USE_RTREE
    m_spIndex->insert(id, *(bvh::dbox*)&ext);
#else
    m_spIndex->Insert(id, const_cast<DBounds&>(ext));
#endif
}

//The skip list spatial index does not need to compute the extent of the blob,
//so this is a shortcut for it, instead of calling the API which accepts ID and Extent.
void SpatialIndexDescriptor::Delete(__int64 id, const unsigned char* blob, int len)
{
#ifdef USE_RTREE
    DBounds ext;
    GetFgfExtents((const unsigned char*)blob, len, (double*)&ext);
    Delete(id, ext);
#else
    m_spIndex->Delete(id);
#endif
}

void SpatialIndexDescriptor::Delete(__int64 id, const DBounds& ext)
{
#ifdef USE_RTREE
    m_spIndex->erase(id, *(bvh::dbox*)&ext);
#else
    m_spIndex->Delete(id);
#endif
}

void SpatialIndexDescriptor::Update(__int64 id, const DBounds& before, const DBounds& after)
{
#ifdef USE_RTREE
    //m_spIndex->erase(id, *(bvh::dbox*)&before);
    m_spIndex->insert(id, *(bvh::dbox*)&after);
#else
    m_spIndex->Update(id, const_cast<DBounds&>(after));
#endif
}

void SpatialIndexDescriptor::Reset()
{
#ifdef USE_RTREE

#else
    m_spIndex->ResetToEmpty();
#endif
}

void SpatialIndexDescriptor::GetTotalExtent(DBounds& ext)
{
#ifdef USE_RTREE
        m_spIndex->get_total_extent(*(bvh::dbox*)&ext);
#else
        m_spIndex->GetTotalExtent(ext);
#endif
}

void SpatialIndexDescriptor::Enumerate(VectorMF* res, const DBounds& bbox)
{
#ifdef USE_RTREE
            bvh::rtree_iterator siter(m_spIndex, *(bvh::dbox*)&bbox);
            fid_t fid;
            while (fid = siter.next())
                res->push_back(fid);
#else
            SpatialIterator siter(const_cast<DBounds&>(bbox), m_spIndex);
            int start = -1;
            int end = -1;
            while (siter.NextRange(start, end))
            {
                for (int i=start; i<end; i++)
                    res->push_back(siter[i]);
            }
#endif
}

void SpatialIndexDescriptor::ReOpen()
{
#ifdef USE_RTREE
#else
    m_spIndex->ReOpen();
#endif
}

SltSpatialIterator* SpatialIndexDescriptor::GetIterator(const DBounds& bbox)
{
    if (IsReleased())
        return new SltSpatialIterator(NULL);

#ifdef USE_RTREE
    bvh::rtree_iterator* iter = new bvh::rtree_iterator(m_spIndex, *(bvh::dbox*)&bbox);
#else
    SpatialIterator* iter = new SpatialIterator(const_cast<DBounds&>(bbox), m_spIndex);
#endif
    return new SltSpatialIterator(iter);
}


SltSpatialIterator::SltSpatialIterator()
{
    m_impl = NULL;
    m_mustBeDeleted = false;

#ifdef USE_RTREE
#else
    m_curfid = 0; //position prior to first record
    m_siEnd = -1;
#endif
}

SltSpatialIterator::SltSpatialIterator(SpatialIteratorImpl* impl)
{
    m_impl = impl;
    m_mustBeDeleted = true;

#ifdef USE_RTREE
#else
    m_curfid = 0; //position prior to first record
    m_siEnd = -1;
#endif
}

SltSpatialIterator::~SltSpatialIterator()
{
    delete m_impl;
}

__int64 SltSpatialIterator::Next()
{
    if (!m_impl)
        return 0;

#ifdef USE_RTREE
    return m_impl->next();
#else
    m_curfid++;
    if (m_curfid >= m_siEnd)
    {
        int start;
        //spatial reader is done, so we are done
        if (m_impl == NULL || !m_impl->NextRange(start, m_siEnd))
            return 0;
        m_curfid = (FdoInt64)(start ? start : 1); //make sure we skip fid=0, which is not valid
    }
    return (*m_impl)[(int)m_curfid];
#endif
}

void SltSpatialIterator::Reset()
{
    if (!m_impl)
        return;

#ifdef USE_RTREE
    m_impl->reset();
#else
    m_curfid = 0; //position prior to first record
    m_siEnd = -1;
    if (m_impl != NULL)
        m_impl->Reset();
#endif
}

SltSpatialIterator SltSpatialIterator::m_emptyIt;

const SltSpatialIterator* SltSpatialIterator::EmptyIterator()
{
    return &SltSpatialIterator::m_emptyIt;
}
