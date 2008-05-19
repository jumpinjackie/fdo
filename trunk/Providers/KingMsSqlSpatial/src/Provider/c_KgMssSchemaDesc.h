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
#ifndef _c_KgMssSchemaDesc_h
#define _c_KgMssSchemaDesc_h




class c_KgMssSchemaDesc : public FdoDisposable
{

public:  
  c_KgMssSchemaDesc() : m_FeatureSchemas(NULL),m_PhysicalSchemaMapping(NULL) { }
  
  static c_KgMssSchemaDesc* Create() { return new c_KgMssSchemaDesc(); }
  virtual void Dispose() { delete this; }

  FdoFeatureSchemaCollection* GetFeatureSchema() { return FDO_SAFE_ADDREF(m_FeatureSchemas.p); }
  c_KgMssOverPhysicalSchemaMapping* GetPhysicalSchemaMapping() { return FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p); }
  c_KgMssSpatialContextCollection* GetSpatialContexts() { return FDO_SAFE_ADDREF(m_SpatialContextColl.p); }
  
  void SetDesc( FdoFeatureSchemaCollection* FSchema,c_KgMssOverPhysicalSchemaMapping* PhSchemaMapping
                ,c_KgMssSpatialContextCollection* SpContextColl  
              );
 
  
  FdoClassDefinition* FindClassDefinition(FdoIdentifier* ClassId);
  c_KgMssOverClassDefinition* FindClassMapping(FdoIdentifier* ClassId);

protected:
  FdoPtr<FdoFeatureSchemaCollection> m_FeatureSchemas;
	FdoPtr<c_KgMssOverPhysicalSchemaMapping> m_PhysicalSchemaMapping;
	FdoPtr<c_KgMssSpatialContextCollection> m_SpatialContextColl;
};




#endif