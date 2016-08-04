
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
#include "stdafx.h"

#ifndef REGISTERMAPPINGS_ORACLE
# include "RegisterMappings.h"
#endif

void RegisterClasses(oracle::occi::Environment* envOCCI_)
{
  oracle::occi::Map *mapOCCI_ = envOCCI_->getMap();
  mapOCCI_->put("MDSYS.SDO_POINT_TYPE", &SDO_POINT_TYPE2::readSQL, &SDO_POINT_TYPE2::writeSQL);
  mapOCCI_->put("MDSYS.SDO_GEOMETRY", &SDO_GEOMETRY::readSQL, &SDO_GEOMETRY::writeSQL);
  //mapOCCI_->put("MDSYS.SDO_TOPO_GEOMETRY", &SDO_TOPO_GEOMETRY::readSQL, &SDO_TOPO_GEOMETRY::writeSQL);
  //mapOCCI_->put("MDSYS.SDO_TOPO_OBJECT", &SDO_TOPO_OBJECT::readSQL, &SDO_TOPO_OBJECT::writeSQL);
  mapOCCI_->put("MDSYS.SDO_DIM_ELEMENT", &SDO_DIM_ELEMENT::readSQL, &SDO_DIM_ELEMENT::writeSQL);
}
