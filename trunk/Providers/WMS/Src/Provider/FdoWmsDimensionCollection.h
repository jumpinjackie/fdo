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

#ifdef _WIN32
#pragma once
#endif

#ifndef _FDOWMSXMLDIMENSIONCOLLECTION__H_
#define _FDOWMSXMLDIMENSIONCOLLECTION__H_

#include "FdoWmsDimension.h"

class FdoWmsDimensionCollection : public FdoNamedCollection<FdoWmsDimension, FdoException>
{
public:
    static FdoWmsDimensionCollection* Create(void);

protected:
    FdoWmsDimensionCollection(void);
    virtual ~FdoWmsDimensionCollection(void);
    void Dispose();
};

typedef FdoPtr<FdoWmsDimensionCollection> FdoWmsDimensionCollectionP;

#endif // _FDOWMSXMLDIMENSIONCOLLECTION__H_


