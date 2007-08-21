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
#ifndef _c_KgMssOverClassCollection_h
#define _c_KgMssOverClassCollection_h

#ifdef _WIN32
#pragma once
#endif //_WIN32



class c_KgMssOverClassCollection : public FdoPhysicalElementMappingCollection<c_KgMssOverClassDefinition>
{
public:
    FDOKGMSS_API static c_KgMssOverClassCollection* Create();
    FDOKGMSS_API static c_KgMssOverClassCollection* Create(FdoPhysicalElementMapping *Parent);

protected:
    c_KgMssOverClassCollection();
    c_KgMssOverClassCollection(FdoPhysicalElementMapping *Parent);
    virtual ~c_KgMssOverClassCollection(void);
    virtual void Dispose();
};

typedef FdoPtr<c_KgMssOverClassCollection> c_KgMssOverClassCollectionP;

#endif // FDOKGORACLASSCOLLECTION_H


