#ifndef FDOSMPHSCHEMAELEMENT_H
#define FDOSMPHSCHEMAELEMENT_H		1
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
 *
 */


#ifdef _WIN32
#pragma once
#endif

#include <Sm/SchemaElement.h>

class FdoSmPhMgr;

// SchemaElement is an abstract class that acts as a base class for all classes
// for Physical schema elements.
class FdoSmPhSchemaElement : public FdoSmSchemaElement
{
public:

    /// Gets the Physical Schema Manager for this element
    FdoPtr<FdoSmPhMgr> GetManager();

protected:
    /// unused constructor needed only to build on Linux
    FdoSmPhSchemaElement() {}

    /// Constructs an instance of a SchemaElement using the specified arguments.
    FdoSmPhSchemaElement(
        FdoString* name,                            // element name
        FdoString* description,                     // element description
        FdoPtr<FdoSmPhMgr> pMgr,                    // Physical Schema Manager
    /// if NULL, parent must be specified
    /// since, in this case, the manager
    /// will be extracted from the parent.
        const FdoSmPhSchemaElement* parent = NULL   // element parent.
    );

    virtual ~FdoSmPhSchemaElement(void);

    void SetManager( FdoPtr<FdoSmPhMgr> mgr );

private:

    /// Physical Schema Manager
   FdoSmPhMgr* mpManager;

};

#endif


