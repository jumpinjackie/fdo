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

// This template extends FdoGeometryImpl with some common code, also in aid
// of avoiding "diamond inheritance", but is specifically for FGF-related
// implementation code.

template <class FDO_GEOMETRY_CLASS> class FdoFgfGeometryImpl: public FdoGeometryImpl<FDO_GEOMETRY_CLASS>
{

public:

    // Constructor that just latches onto a stream (of FGF data).
    FdoFgfGeometryImpl(FdoFgfGeometryFactory * factory, FdoByteArray * byteArray, const FdoByte * data, FdoInt32 count)
        : FdoGeometryImpl<FDO_GEOMETRY_CLASS>(), m_factory(factory),
          m_data(NULL), m_streamEnd(NULL), m_streamPtr(NULL)
    {
        if ((NULL == byteArray && NULL == data) ||
            (NULL != byteArray && byteArray->GetCount() <= 0) ||
            (NULL != data && count <= 0))
		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));

	    SetFgf(byteArray, data, count);
    }

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
            ba = FdoByteArray::Create(m_data, m_streamEnd-m_data);
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
        if (m_byteArray != NULL)
            m_factory->TakeReleasedByteArray(m_byteArray);

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
		    throw FdoException::Create(FdoException::NLSGetMessage(FDO_NLSID(FDO_2_BADPARAMETER)));
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

    FdoFgfGeometryFactory *        m_factory;  // FGF geometry factory (lightweight to avoid circular reference)


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

    // This constructor is extended by some geometry types that copy an input
    // FGF data stream.
    FdoFgfGeometryImpl(FdoFgfGeometryFactory * factory)
        : FdoGeometryImpl<FDO_GEOMETRY_CLASS>(), m_factory(factory),
          m_data(NULL), m_streamEnd(NULL), m_streamPtr(NULL)
    {
    };

    // Destructor
	virtual ~FdoFgfGeometryImpl()
    {
    }
};


#endif

