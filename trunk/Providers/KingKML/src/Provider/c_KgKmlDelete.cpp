/*
* Copyright (C) 2010  SL-King d.o.o
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
#include "StdAfx.h"
//#include "c_KgOraFilterProcessor.h"

c_KgKmlDelete::c_KgKmlDelete(c_KgKmlConnection *Conn)
  : c_KgKmlFdoFeatureCommand(Conn)
{
}

c_KgKmlDelete::~c_KgKmlDelete(void)
{
}






/// \brief
/// Executes the insert command and returns a reference to an
/// FdoIFeatureReader. Some feature providers can generate automatic identity
/// values for features. This will happen automatically as the features are
/// inserted. The returned FdoIFeatureReader allows the client to obtain the
/// automatic identity property value(s) of newly inserted object(s). The
/// returned feature reader at a minimum will read the unique identity
/// properties of the objects just inserted. Multiple objects will be
/// returned through the reader in the case of a batch insert.
/// 
/// \return
/// Returns an FdoIFeatureReader
/// 
FdoInt32 c_KgKmlDelete::Execute()
{
  FdoPtr<c_KgKmlFdoData> kmlfdodata = m_Connection->GetKmlFdoData();
  FdoPtr<c_KgKmlSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();

  FdoPtr<FdoIdentifier> classid = GetFeatureClassName();
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
  if( !classdef.p ) 
  {
    throw FdoException::Create(L"c_KgKmlDelete::Execute: Unable to find class definition!");

  }

  

  FdoPtr<SdfConnection> sdfconn = kmlfdodata->GetSdfConn();
  FdoPtr<FdoIDelete> sdfdelete = (FdoIDelete*)sdfconn->CreateCommand(FdoCommandType_Delete);


  FdoPtr<FdoIdentifier> fclass = GetFeatureClassName();
  sdfdelete->SetFeatureClassName(fclass);
  
  FdoPtr<FdoFilter> filter = GetFilter();
  sdfdelete->SetFilter(filter);


  FdoInt32 numchanged = sdfdelete->Execute();

  if( numchanged>0 )
  { 
    kmlfdodata->ClearPlacemarks();
    kmlfdodata->SyncPlacemarksFromSDF() ;
    
  }
  
  return numchanged;
}//end of c_KgKmlDelete::Execute

FdoILockConflictReader* c_KgKmlDelete::GetLockConflicts()
{
  return NULL;
}