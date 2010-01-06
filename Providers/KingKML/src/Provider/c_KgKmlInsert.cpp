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
#include "c_KgKmlInsert.h"
#include "c_FdoKml_API.h"



c_KgKmlInsert::c_KgKmlInsert(c_KgKmlConnection *Conn)
  : c_KgKmlFdoCommand(Conn)
{
}

c_KgKmlInsert::~c_KgKmlInsert(void)
{
}

/// \brief
/// Gets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \return
/// Returns the class name.
/// 
FdoIdentifier* c_KgKmlInsert::GetFeatureClassName()
{
  return FDO_SAFE_ADDREF(m_ClassId.p);
}

/// \brief
/// Sets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \param value 
/// Input the identifier for the class.
/// 
/// \return
/// Returns nothing
/// 
void c_KgKmlInsert::SetFeatureClassName(FdoIdentifier* ClassId)
{
  m_ClassId = ClassId;
  FDO_SAFE_ADDREF(m_ClassId.p);
}

/// \brief
/// Sets the name of the class to be operated upon as an FdoIdentifier.
/// 
/// \param value 
/// Input the class name.
/// 
/// \return
/// Returns nothing
/// 
void c_KgKmlInsert::SetFeatureClassName(FdoString* ClassName)
{
 
  m_ClassId = FdoIdentifier::Create( ClassName );  
}

/// \brief
/// Gets the FdoPropertyValueCollection that specifies the names and values
/// of the properties for the instance to be inserted.
/// 
/// \return
/// Returns the list of properties and their values.
/// 
FdoPropertyValueCollection* c_KgKmlInsert::GetPropertyValues()
{
  if( !m_PropertyValues.p )
  {
    m_PropertyValues = FdoPropertyValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_PropertyValues.p );
}

/// \brief
/// Gets the FdoBatchParameterValueCollection that can be used for optimized
/// batch inserts of multiple features with a single insert command. Batch
/// inserts can be performed by using Parameters for each of the property
/// values, then adding collections of parameter values to the
/// FdoBatchParameterValueCollection. Each FdoParameterValueCollection in the
/// FdoBatchParameterValueCollection should contain one FdoParameterValue for each
/// of the parameters specified for property values.
/// 
/// \return
/// Returns FdoBatchParameterValueCollection
/// 
FdoBatchParameterValueCollection* c_KgKmlInsert::GetBatchParameterValues()
{
  return NULL;
  
  if( !m_BatchParameterValues.p )
  {
    m_BatchParameterValues = FdoBatchParameterValueCollection::Create();
  }

  return FDO_SAFE_ADDREF( m_BatchParameterValues.p );
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
FdoIFeatureReader* c_KgKmlInsert::Execute()
{
  FdoPtr<c_KgKmlFdoData> kmlfdodata = m_Connection->GetKmlFdoData();
  FdoPtr<c_KgKmlSchemaDesc> schemadesc = m_Connection->GetSchemaDesc();
  
  FdoPtr<FdoClassDefinition> classdef = schemadesc->FindClassDefinition(m_ClassId);
  if( !classdef.p ) 
  {
    throw FdoException::Create(L"c_KgKmlInsert::Execute: Unable to find class definition!");
    
  }
  
  FdoPtr<FdoIFeatureReader> sdfinsertreader;
  
  FdoPtr<SdfConnection> sdfconn = kmlfdodata->GetSdfConn();
  FdoPtr<FdoIInsert> sdfinsert = (FdoIInsert*)sdfconn->CreateCommand(FdoCommandType_Insert);
  
  
  FdoPtr<FdoIdentifier> fclass = GetFeatureClassName();
  sdfinsert->SetFeatureClassName(fclass);
  
  FdoPtr<FdoPropertyValueCollection> sdfpropvals = sdfinsert->GetPropertyValues();
  
  
 // Find KML_FDO_ID in property values  
  
  FdoPtr<FdoPropertyValue> propid = m_PropertyValues->FindItem(L"KML_FDO_ID");
  if( !propid.p )
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
  
  int count = m_PropertyValues->GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<FdoPropertyValue> propval = m_PropertyValues->GetItem(ind);
    sdfpropvals->Add(propval);
  }
  
  
  sdfinsertreader = sdfinsert->Execute();
  
  
  if( sdfinsertreader )
  {  
    
    kmlfdodata->SyncPlacemarksFromReader(sdfinsertreader);
    
  }

   
 return FDO_SAFE_ADDREF(sdfinsertreader.p);
  
}//end of c_KgKmlInsert::Execute

