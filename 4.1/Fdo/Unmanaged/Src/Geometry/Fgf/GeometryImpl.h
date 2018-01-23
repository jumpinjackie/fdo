#ifndef _FGFGEOMETRYIMPL_H_
#define _FGFGEOMETRYIMPL_H_
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
*/

#ifdef _WIN32
#pragma once
#endif

#include "../GeometryImpl.h"

#include <Geometry/Fgf/Factory.h>
//#include "GeometryFactory2.h"
class FdoFgfGeometryPools;  // Forward declaration
#include "Util.h"

// This template extends FdoGeometryImpl with some common code, also in aid
// of avoiding "diamond inheritance", but is specifically for FGF-related
// implementation code.

template <class FDO_GEOMETRY_CLASS> class FdoFgfGeometryImpl: public FdoGeometryImpl<FDO_GEOMETRY_CLASS>
{

public:

	// Get the FGF array from the geometry.
	virtual FdoByteArray * GetFgf()
    {
        // This is a plain pointer, rather than a FdoPtr, due to this method
        // being very performance-sensitive.
        FdoByteArray * ba = NULL;

        if (m_byteArray == NULL)
        {
            // We don't own the array data, so we must copy it to get it into
            // a FdoByteArray.
            ba = FdoByteArray::Create(m_data, (FdoInt32)(m_streamEnd-m_data));
        }
        else
        {
            // We own the array data via its encapsulating FdoByteArray object.
            // Just share it with the caller.
            (m_byteArray.p)->AddRef();
            ba = m_byteArray;
        }
	    return ba;
    }

    // Assign a new FGF array, effectively re-creating the geometry.
    // The array can be either in a FdoByteArray or a native C++ array.
	virtual void SetFgf(FdoByteArray * fgf, const FdoByte* fgfData, FdoInt32 count)
    {
        SurrenderByteArray();

        if (NULL != fgf)
        {
            m_byteArray = FDO_SAFE_ADDREF(fgf);
            m_data = m_byteArray->GetData();
            m_streamEnd = m_data + m_byteArray->GetCount();
        }
        else if (NULL != fgfData && count > sizeof(FdoInt32))   // Need at least Int32 for geometry type.
        {
            m_byteArray = NULL;
            m_data = fgfData;
            m_streamEnd = m_data + count;
        }
        else
        {
		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_1_INVALID_INPUT_ON_CLASS_FUNCTION),
                                                                   L"FdoFgfGeometryImpl::SetFgf",
                                                                   L"fgf/fgfData/count"));
        }
        m_streamPtr = m_data;

        ReleaseText();
    }

    void ReleaseText()
    {
        if (NULL != this->m_fgft)
        {
            delete [] this->m_fgft;
            this->m_fgft = NULL;
        }
    }

    FdoFgfGeometryFactory *         m_factoryy;  // FGF geometry factory (lightweight to avoid circular reference)
    FdoFgfGeometryPools *           m_pools;  // Geometry pools (lightweight to avoid circular reference)


	// Storage of FGF bytestream data (non-NULL if the data is at least partly owned here)
	FdoPtr<FdoByteArray>            m_byteArray;

	// Array data (whether or not data is owned by m_byteArray)
	const FdoByte *                 m_data;

	// End of stream (1 byte past end of array denoted by m_data)
	const FdoByte *                 m_streamEnd;

	// Current position in the stream (somewhere in array denoted by m_data)
	const mutable FdoByte *         m_streamPtr;


protected:
    // Protect default constructor so that a factory must be used.
    FdoFgfGeometryImpl() {};

    // This constructor is used during auto-initialization in most 
    // geometry types.
    FdoFgfGeometryImpl(FdoFgfGeometryFactory * factory, FdoFgfGeometryPools * pools)
        : FdoGeometryImpl<FDO_GEOMETRY_CLASS>(), m_factoryy(factory), m_pools(pools),
          m_data(NULL), m_streamEnd(NULL), m_streamPtr(NULL)
    {
    };

    // Destructor
	virtual ~FdoFgfGeometryImpl()
    {
        SurrenderByteArray();
    }

    // Get pointer to geometry factory.
    inline FdoFgfGeometryFactory * GetFactory() const
    {
        FdoFgfGeometryFactory * factory =
            (NULL == m_factoryy) ?
            FdoFgfGeometryFactory::GetInstance() :
            factory = FDO_SAFE_ADDREF(m_factoryy);

        return factory;
    }

    inline void SurrenderByteArray()
    {
        if (m_byteArray != NULL)
        {
            FdoFgfGeometryPools * pools = FgfUtil::GetPoolsNoRef(m_pools);
            if (NULL != pools)
                FgfUtil::TakeReleasedByteArray(pools, m_byteArray);
            m_byteArray = NULL;
        }
    }
};

#endif

