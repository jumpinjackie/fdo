#ifndef FDO_XML_LPCOLLECTION_H_
#define FDO_XML_LPCOLLECTION_H_
//

//
// Copyright (C) 2004-2006  Autodesk, Inc.
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of version 2.1 of the GNU Lesser
// General Public License as published by the Free Software Foundation.
// 
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//

#ifdef _WIN32
#pragma once
#endif
#include <FdoStd.h>

template <class OBJ> class FdoXmlLpCollection : public FdoNamedCollection<OBJ, FdoSchemaException>
{

protected:
	FdoXmlLpCollection()
	{
	}

    FdoXmlLpCollection(FdoXmlLpSchemaElement* parent) : FdoNamedCollection<OBJ, FdoSchemaException>()
    {
        m_parent = parent;
    }

    virtual ~FdoXmlLpCollection()
    {
        Clear();
    }

public:
    FDO_API virtual void SetItem(FdoInt32 index, OBJ* value)
    {
        if (index < FdoNamedCollection<OBJ, FdoSchemaException>::GetCount() && index >= 0)   // verify index is in bounds
        {
            if (value) 
                value->SetParent(m_parent);
        }

        FdoNamedCollection<OBJ, FdoSchemaException>::SetItem(index, value);
    }

    FDO_API virtual FdoInt32 Add(OBJ* value)
    {
        if ( value ) 
            value->SetParent(m_parent);

        return FdoNamedCollection<OBJ, FdoSchemaException>::Add(value);
    }

    FDO_API virtual void Insert(FdoInt32 index, OBJ* value)
    {
        if ( value ) 
            value->SetParent(m_parent);
        FdoNamedCollection<OBJ, FdoSchemaException>::Insert(index, value);
    }

    FDO_API virtual void Clear()
    {
        for(FdoInt32 i = 0; i < FdoNamedCollection<OBJ, FdoSchemaException>::GetCount(); i++)
        {
            OBJ*    pitem = FdoNamedCollection<OBJ, FdoSchemaException>::GetItem(i);
            pitem->SetParent(NULL);
        }

        FdoNamedCollection<OBJ, FdoSchemaException>::Clear();
    }

    FDO_API virtual void Remove(const OBJ* value)
    {
        OBJ*    pitem = (OBJ*)value;       // cast drops const
        pitem->SetParent(NULL);

        FdoNamedCollection<OBJ, FdoSchemaException>::Remove(value);
    }

    FDO_API virtual void RemoveAt(FdoInt32 index)
    {
        OBJ*    pitem = FdoNamedCollection<OBJ, FdoSchemaException>::GetItem(index);
        pitem->SetParent(NULL);

        FdoNamedCollection<OBJ, FdoSchemaException>::RemoveAt(index);
    }

protected:
    FdoXmlLpSchemaElement*   m_parent;
};
#endif

