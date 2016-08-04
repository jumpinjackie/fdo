#ifndef FDO_XML_LPSCHEMAELEMENT_H_
#define FDO_XML_LPSCHEMAELEMENT_H_
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

class FdoXmlLpSchemaElement : 
    public FdoIDisposable
{
    template <class OBJ> friend class FdoXmlLpCollection;

private:
    FdoXmlLpSchemaElement*               m_parent;

public:
    virtual FdoXmlLpSchemaElement* GetParent() { return FDO_SAFE_ADDREF(m_parent); }

//    virtual FdoXmlLpSchema* GetLpSchema();

    virtual FdoString* GetName() = 0;

    virtual FdoBoolean CanSetName() { return false; }

protected:
    FdoXmlLpSchemaElement() {}

    virtual ~FdoXmlLpSchemaElement() {}

    // Sets the parent of this SchemaObject. Can only be called from within the
    // Schema package.
    virtual void SetParent(FdoXmlLpSchemaElement* value) { m_parent = value; }

};

typedef FdoPtr<FdoXmlLpSchemaElement> FdoXmlLpSchemaElementP;

#endif

