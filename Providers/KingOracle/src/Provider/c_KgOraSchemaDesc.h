#ifndef _c_KgOraSchemaDesc_h
#define _c_KgOraSchemaDesc_h




class c_KgOraSchemaDesc : public GisDisposable
{

public:  
  c_KgOraSchemaDesc() : m_FeatureSchemas(NULL),m_PhysicalSchemaMapping(NULL) { }
  
  static c_KgOraSchemaDesc* Create() { return new c_KgOraSchemaDesc(); }
  virtual void Dispose() { delete this; }

  FdoFeatureSchemaCollection* GetFeatureSchema() { return FDO_SAFE_ADDREF(m_FeatureSchemas.p); }
  FdoKgOraPhysicalSchemaMapping* GetPhysicalSchemaMapping() { return FDO_SAFE_ADDREF(m_PhysicalSchemaMapping.p); }
  c_KgOraSpatialContextCollection* GetSpatialContexts() { return FDO_SAFE_ADDREF(m_SpatialContextColl.p); }
  
  void SetDesc( FdoFeatureSchemaCollection* FSchema,FdoKgOraPhysicalSchemaMapping* PhSchemaMapping
                ,c_KgOraSpatialContextCollection* SpContextColl  
              );
 
  
  FdoClassDefinition* FindClassDefinition(FdoIdentifier* ClassId);
  FdoKgOraClassDefinition* FindClassMapping(FdoIdentifier* ClassId);

protected:
  FdoPtr<FdoFeatureSchemaCollection> m_FeatureSchemas;
	FdoPtr<FdoKgOraPhysicalSchemaMapping> m_PhysicalSchemaMapping;
	FdoPtr<c_KgOraSpatialContextCollection> m_SpatialContextColl;
};




#endif