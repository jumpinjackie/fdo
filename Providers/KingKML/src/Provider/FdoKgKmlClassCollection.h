/*
 * Copyright (C) 2006  SL-King d.o.o
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
#ifndef FDOKGKMLCLASSCOLLECTION_H
#define FDOKGKMLCLASSCOLLECTION_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <FdoKgKmlClassDefinition.h>

class FdoKgKmlClassCollection : public FdoPhysicalElementMappingCollection<FdoKgKmlClassDefinition>
{
public:
    FDOKGKML_API static FdoKgKmlClassCollection* Create();
    FDOKGKML_API static FdoKgKmlClassCollection* Create(FdoPhysicalElementMapping *Parent);

protected:
    FdoKgKmlClassCollection();
    FdoKgKmlClassCollection(FdoPhysicalElementMapping *Parent);
    virtual ~FdoKgKmlClassCollection(void);
    virtual void Dispose();
};

typedef FdoPtr<FdoKgKmlClassCollection> FdoKgOraClassCollectionP;

#endif // FDOKGORACLASSCOLLECTION_H


