/*
 * Copyright (C) 2007  Haris Kurtagic
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
#ifndef _c_KgInfOverClassCollection_h
#define _c_KgInfOverClassCollection_h

#ifdef _WIN32
#pragma once
#endif //_WIN32



class c_KgInfOverClassCollection : public FdoPhysicalElementMappingCollection<c_KgInfOverClassDefinition>
{
public:
    FDOKGINF_API static c_KgInfOverClassCollection* Create();
    FDOKGINF_API static c_KgInfOverClassCollection* Create(FdoPhysicalElementMapping *Parent);

protected:
    c_KgInfOverClassCollection();
    c_KgInfOverClassCollection(FdoPhysicalElementMapping *Parent);
    virtual ~c_KgInfOverClassCollection(void);
    virtual void Dispose();
};

typedef FdoPtr<c_KgInfOverClassCollection> c_KgInfOverClassCollectionP;

#endif // FDOKGORACLASSCOLLECTION_H


