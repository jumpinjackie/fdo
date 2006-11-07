#include "StdAfx.h"
#include "c_KgOraSchemaDesc.h"


 void c_KgOraSchemaDesc::SetDesc( FdoFeatureSchemaCollection* FSchema,FdoKgOraPhysicalSchemaMapping* PhSchemaMapping
                ,c_KgOraSpatialContextCollection* SpContextColl  
              ) 
  { 
  
    
    m_FeatureSchemas = FSchema;
    FDO_SAFE_ADDREF( m_FeatureSchemas.p );
    m_PhysicalSchemaMapping = PhSchemaMapping;
    FDO_SAFE_ADDREF( m_PhysicalSchemaMapping.p );
    
    m_SpatialContextColl = SpContextColl;
    FDO_SAFE_ADDREF( m_SpatialContextColl.p );
  }//end of c_KgOraSchemaDesc::SetDesc

FdoClassDefinition* c_KgOraSchemaDesc::FindClassDefinition(FdoIdentifier* ClassId)
{
  FdoPtr<FdoFeatureSchemaCollection> fschemas = GetFeatureSchema();
  
  FdoPtr<FdoClassDefinition> classdef;
  if( fschemas && (fschemas->GetCount() > 0) )
  {
    FdoPtr<FdoFeatureSchema> schm = fschemas->GetItem(0);      
    FdoPtr<FdoClassCollection> classes = schm->GetClasses();
    
    classdef = classes->FindItem( ClassId->GetName() );
    
    //if( !classdef.p ) return NULL;
    return FDO_SAFE_ADDREF(classdef.p);    
  }
  
  return NULL;
}//end of c_KgOraSchemaDesc::FindClassDefinition

FdoKgOraClassDefinition* c_KgOraSchemaDesc::FindClassMapping(FdoIdentifier* ClassId)
{
  return m_PhysicalSchemaMapping->FindByClassName(ClassId->GetName());
    
}//end of c_KgOraSchemaDesc::FindClassDefinition