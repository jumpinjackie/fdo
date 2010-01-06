/*
* Copyright (C) 2008  SL-King d.o.o
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


#ifndef _c_KmlToSdfFileMapping_h
#define _c_KmlToSdfFileMapping_h

#include "c_KmlSdfFileData.h"

// c_KmlToSdfFileMapping will create SDF connection for specified KML File.
// c_KmlToSdfFileMapping creates SDF file and  creates FDO connection to the file.
// Before creating new SDF file c_KmlToSdfFileMapping will check if SDF file for specified KML file allready exists. 
// If exists it will create new SDF connection to existing SDF file.
class c_KmlToSdfFileMapping
{
public:
  c_KmlToSdfFileMapping(void);
public:
  ~c_KmlToSdfFileMapping(void);
  
protected:
  FdoPtr<c_KmlSdfFileDataCollection> m_KmlSdfFileDataCollection;  
  
public:
  SdfConnection* OpenSdfConnection(const string& KmlFileName);  
  
  
};

#endif