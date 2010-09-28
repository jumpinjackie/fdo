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
#include "c_KgKmlSchemaDesc.h"


 void c_KgKmlSchemaDesc::SetDesc( FdoFeatureSchemaCollection* FSchema,FdoKgKmlPhysicalSchemaMapping* PhSchemaMapping
                ,c_KgKmlSpatialContextCollection* SpContextColl  
              ) 
  { 
  
    
    m_FeatureSchemas = FSchema;
    FDO_SAFE_ADDREF( m_FeatureSchemas.p );
    m_PhysicalSchemaMapping = PhSchemaMapping;
    FDO_SAFE_ADDREF( m_PhysicalSchemaMapping.p );
    
    m_SpatialContextColl = SpContextColl;
    FDO_SAFE_ADDREF( m_SpatialContextColl.p );
  }//end of c_KgKmlSchemaDesc::SetDesc

FdoClassDefinition* c_KgKmlSchemaDesc::FindClassDefinition(FdoIdentifier* ClassId)
{
  FdoPtr<FdoFeatureSchemaCollection> fschemas = GetFeatureSchema();
  
  FdoPtr<FdoClassDefinition> classdef;
  
  if( fschemas && (fschemas->GetCount() > 0) )
  {
    FdoStringP fname = ClassId->GetText();
    FdoPtr<FdoIDisposableCollection> classes = fschemas->FindClass(fname);
    
    //FdoPtr<FdoFeatureSchema> schm = fschemas->GetItem(0);      
    //FdoPtr<FdoClassCollection> classes = schm->GetClasses();
    
    //classdef = classes->FindItem( ClassId->GetName() );
    
    if( classes->GetCount() > 0 )
    {
      classdef = (FdoClassDefinition*)classes->GetItem( 0 );
    
      return FDO_SAFE_ADDREF(classdef.p);    
    }
  }
  
  return NULL;
}//end of c_KgKmlSchemaDesc::FindClassDefinition

FdoKgKmlClassDefinition* c_KgKmlSchemaDesc::FindClassMapping(FdoIdentifier* ClassId)
{
  return m_PhysicalSchemaMapping->FindByClassName(ClassId->GetName());
    
}//end of c_KgKmlSchemaDesc::FindClassDefinition