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
#include "StdAfx.h"
//#include "c_KgOraFilterProcessor.h"

c_KgKmlUpdate::c_KgKmlUpdate(c_KgKmlConnection *Conn)
  : c_KgKmlFdoFeatureCommand(Conn)
{
}

c_KgKmlUpdate::~c_KgKmlUpdate(void)
{
}



/// \brief
/// Gets the FdoPropertyValueCollection that specifies the names and values
/// of the properties for the instance to be inserted.
/// 
/// \return
/// Returns the list of properties and their values.
/// 
FdoPropertyValueCollection* c_KgKmlUpdate::GetPropertyValues()
{
  if( !m_PropertyValues.p )
  {
    m_PropertyValues = FdoPropertyValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_PropertyValues.p );
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
FdoInt32 c_KgKmlUpdate::Execute()
{
  FdoPtr<c_KgKmlFdoData> kmlfdodata = m_Connection->GetKmlFdoData();
  FdoPtr<c_KgKmlSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();

  FdoPtr<FdoIdentifier> classid = GetFeatureClassName();
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(classid);
  if( !classdef.p ) 
  {
    throw FdoException::Create(L"c_KgKmlUpdate::Execute: Unable to find class definition!");

  }

  

  FdoPtr<SdfConnection> sdfconn = kmlfdodata->GetSdfConn();
  FdoPtr<FdoIUpdate> sdfupdate = (FdoIUpdate*)sdfconn->CreateCommand(FdoCommandType_Update);


  FdoPtr<FdoIdentifier> fclass = GetFeatureClassName();
  sdfupdate->SetFeatureClassName(fclass);
  
  FdoPtr<FdoFilter> filter = GetFilter();
  sdfupdate->SetFilter(filter);

  FdoPtr<FdoPropertyValueCollection> sdfpropvals = sdfupdate->GetPropertyValues();


  // Find KML_FDO_ID in property values  
/*  
  int count = m_PropertyValues->GetCount();
  FdoPtr<FdoPropertyValue> propid = m_PropertyValues->FindItem(L"KML_FDO_ID");
  if( propid )
  {
    FdoPtr<FdoPropertyValue> prop_kmlfdoid = FdoPropertyValue::Create(L"KML_FDO_ID",NULL);
    FdoPtr<FdoDataValue> val = FdoDataValue::Create((FdoInt32) kmlfdodata->GetNextKmlFdoId() );
    prop_kmlfdoid->SetValue(val);
    m_PropertyValues->Add(prop_kmlfdoid);
  }
  else
  {

    FdoPtr<FdoDataValue> val = FdoDataValue::Create((FdoInt32) kmlfdodata->GetNextKmlFdoId() );
    propid->SetValue(val);
  }
*/
  int count = m_PropertyValues->GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
    sdfpropvals->Add(propval);
  }

  FdoInt32 numchanged = sdfupdate->Execute();

  if( numchanged>0 )
  { 
    kmlfdodata->SyncPlacemarksFromSDF() ;
    
  }
  
  return numchanged;
}//end of c_KgKmlUpdate::Execute

FdoILockConflictReader* c_KgKmlUpdate::GetLockConflicts()
{
  return NULL;
}