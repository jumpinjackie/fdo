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
#ifndef FdoKgOraPropertyDefinitionCollection_H
#define FdoKgOraPropertyDefinitionCollection_H

#ifdef _WIN32
#pragma once
#endif //_WIN32



class c_KgInfOverPropertyDefinitionCollection : public FdoPhysicalElementMappingCollection<c_KgInfOverPropertyDefinition>
{
public:
    FDOKGINF_API static c_KgInfOverPropertyDefinitionCollection* Create();
    FDOKGINF_API static c_KgInfOverPropertyDefinitionCollection* Create(FdoPhysicalElementMapping *parent);

protected:
    c_KgInfOverPropertyDefinitionCollection();
    c_KgInfOverPropertyDefinitionCollection(FdoPhysicalElementMapping *parent);
    virtual ~c_KgInfOverPropertyDefinitionCollection(void);
    virtual void Dispose();
};




#endif // FdoKgOraPropertyDefinitionCollection_H


