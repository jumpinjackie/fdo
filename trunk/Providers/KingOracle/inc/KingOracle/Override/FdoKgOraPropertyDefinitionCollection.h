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
#ifndef FdoKgOraPropertyDefinitionCollection_H
#define FdoKgOraPropertyDefinitionCollection_H

#ifdef _WIN32
#pragma once
#endif //_WIN32

#include <KingOracle/Override/FdoKgOraPropertyDefinition.h>

class FdoKgOraPropertyDefinitionCollection : public FdoPhysicalElementMappingCollection<FdoKgOraPropertyDefinition>
{
public:
    FDOKGORA_API static FdoKgOraPropertyDefinitionCollection* Create();
    FDOKGORA_API static FdoKgOraPropertyDefinitionCollection* Create(FdoPhysicalElementMapping *parent);

protected:
    FdoKgOraPropertyDefinitionCollection();
    FdoKgOraPropertyDefinitionCollection(FdoPhysicalElementMapping *parent);
    virtual ~FdoKgOraPropertyDefinitionCollection(void);
    virtual void Dispose();
};

typedef FdoPtr<FdoKgOraPropertyDefinitionCollection> FdoKgOraPropertyDefinitionCollectionP;


#endif // FdoKgOraPropertyDefinitionCollection_H


