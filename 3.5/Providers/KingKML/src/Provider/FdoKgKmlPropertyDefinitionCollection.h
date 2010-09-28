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
#ifndef FdoKgKmlPropertyDefinitionCollection_H
#define FdoKgKmlPropertyDefinitionCollection_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <FdoKgKmlPropertyDefinition.h>

class FdoKgKmlPropertyDefinitionCollection : public FdoPhysicalElementMappingCollection<FdoKgKmlPropertyDefinition>
{
public:
    FDOKGKML_API static FdoKgKmlPropertyDefinitionCollection* Create();
    FDOKGKML_API static FdoKgKmlPropertyDefinitionCollection* Create(FdoPhysicalElementMapping *parent);

protected:
    FdoKgKmlPropertyDefinitionCollection();
    FdoKgKmlPropertyDefinitionCollection(FdoPhysicalElementMapping *parent);
    virtual ~FdoKgKmlPropertyDefinitionCollection(void);
    virtual void Dispose();
};

typedef FdoPtr<FdoKgKmlPropertyDefinitionCollection> FdoKgKmlPropertyDefinitionCollectionP;


#endif // FdoKgOraPropertyDefinitionCollection_H


