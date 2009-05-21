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
#ifndef _c_KgInfSchemaDesc_h
#define _c_KgInfSchemaDesc_h




class c_KgInfSchemaDesc : public FdoDisposable
{

public:  
  c_KgInfSchemaDesc() : m_FeatureSchemas(NULL),m_PhysicalSchemaMapping(NULL) { }
  
  static c_KgInfSchemaDesc* Create() { return new c_KgInfSchemaDesc(); }
  virtual void Dispose() { delete this; }

  FdoFeatureSchemaCollection* GetFeatureSchema() { return FDO_SAFE_ADDREF(m_FeatureSchemas.p); }
  c_KgInfOverPhysicalSchemaMapping* GetPhysicalSchemaMapping() { return FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p); }
  c_KgInfSpatialContextCollection* GetSpatialContexts() { return FDO_SAFE_ADDREF(m_SpatialContextColl.p); }
  
  void SetDesc( FdoFeatureSchemaCollection* FSchema,c_KgInfOverPhysicalSchemaMapping* PhSchemaMapping
                ,c_KgInfSpatialContextCollection* SpContextColl  
              );
 
  
  FdoClassDefinition* FindClassDefinition(FdoIdentifier* ClassId);
  c_KgInfOverClassDefinition* FindClassMapping(FdoIdentifier* ClassId);

protected:
  FdoPtr<FdoFeatureSchemaCollection> m_FeatureSchemas;
	FdoPtr<c_KgInfOverPhysicalSchemaMapping> m_PhysicalSchemaMapping;
	FdoPtr<c_KgInfSpatialContextCollection> m_SpatialContextColl;
};




#endif