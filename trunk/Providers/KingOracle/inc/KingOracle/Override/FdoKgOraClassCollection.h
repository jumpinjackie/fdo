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
#ifndef FDOKGORACLASSCOLLECTION_H
#define FDOKGORACLASSCOLLECTION_H


#include <KingOracle/Override/FdoKgOraClassDefinition.h>

class FdoKgOraClassCollection : public FdoPhysicalElementMappingCollection<FdoKgOraClassDefinition>
{
public:
    FDOKGORA_API static FdoKgOraClassCollection* Create();
    FDOKGORA_API static FdoKgOraClassCollection* Create(FdoPhysicalElementMapping *Parent);

protected:
    FdoKgOraClassCollection();
    FdoKgOraClassCollection(FdoPhysicalElementMapping *Parent);
    virtual ~FdoKgOraClassCollection(void);
    virtual void Dispose();
};

typedef FdoPtr<FdoKgOraClassCollection> FdoKgOraClassCollectionP;

#endif // FDOKGORACLASSCOLLECTION_H


