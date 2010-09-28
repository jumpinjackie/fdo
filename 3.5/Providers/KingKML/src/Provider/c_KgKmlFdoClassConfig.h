#pragma once


class c_KgKmlFdoClassAttributeConfig : public FdoIDisposable
{
public:
  c_KgKmlFdoClassAttributeConfig();
  void Dispose ();
  ~c_KgKmlFdoClassAttributeConfig();
  
  
protected:
  FdoStringP m_AttrName;
  
  FdoStringP m_Exp;
  FdoPtr<FdoExpression> m_FdoExp;
  bool m_IsParsed;
  
public:
  FDOKGKML_API FdoString* GetAttrName() const { return m_AttrName; }
  FDOKGKML_API void SetAttrName(FdoString* val) { m_AttrName = val; }


  FDOKGKML_API FdoString* GetExp() const { return m_Exp; }
  FDOKGKML_API void SetExp(FdoString* val) { m_Exp = val; m_IsParsed=false; }
  FDOKGKML_API bool GetIsFdoExp() ; // if false than no value should be written for placemark name
  FDOKGKML_API FdoExpression* GetFdoExpr() ;  
  
protected:
  void ParseExp();  
  
};


class  c_KgKmlFdoClassAttributeConfigCollection : public FdoCollection<c_KgKmlFdoClassAttributeConfig,FdoException>
{

protected:
  virtual void Dispose() { delete this; } 

public:

};

 class c_KgKmlFdoClassConfig : public FdoIDisposable
{
public:
  c_KgKmlFdoClassConfig(void);
   void Dispose (); 

  ~c_KgKmlFdoClassConfig(void);
  
  friend class c_KgKmlFdoClassConfigCollection;
  
  FDOKGKML_API static c_KgKmlFdoClassConfig* Create();
  
protected:
  FdoStringP m_ClassName;        // full name of the class
  FdoStringP m_FdoProp_KmlFdoId;        // which FDO property is used for KmlFdoId
  
  c_KgKmlFdoClassAttributeConfig m_PlacemarkNameAttr;
  
  c_KgKmlFdoClassAttributeConfig m_PlacemarkDescAttr;
  
  c_KgKmlFdoClassAttributeConfig m_GeomAltModeAttr;
  
  c_KgKmlFdoClassAttributeConfig m_GeomTessellateAttr;
  
 
  FdoPtr<c_KgKmlFdoClassAttributeConfigCollection> m_AttrCollection;
  
  
public:
  FDOKGKML_API FdoString* GetClassName() const { return m_ClassName; }
  void SetClassName(FdoString* val) { m_ClassName = val; }
  
  FDOKGKML_API FdoString* GetFdoProp_KmlFdoId() const { return m_FdoProp_KmlFdoId; }
  void SetFdoProp_KmlFdoId(FdoString* val) { m_FdoProp_KmlFdoId = val; } 
  
  FDOKGKML_API FdoString* GetPlacemarkNameExp() const { return m_PlacemarkNameAttr.GetExp(); }
  void SetPlacemarkNameExp(FdoString* val) { m_PlacemarkNameAttr.SetExp(val); }
  FDOKGKML_API bool GetIsPlacemarkNameFdoExp() { return m_PlacemarkNameAttr.GetIsFdoExp(); } ; // if false than no value should be written for placemark name
  FDOKGKML_API FdoExpression* GetPlacemarkNameFdoExpr() { return m_PlacemarkNameAttr.GetFdoExpr(); };
  
  FDOKGKML_API FdoString* GetPlacemarkDescExp() const { return m_PlacemarkDescAttr.GetExp(); }
  void SetPlacemarkDescExp(FdoString* val) { m_PlacemarkDescAttr.SetExp(val); }
  FDOKGKML_API bool GetIsPlacemarkDescFdoExp() { return m_PlacemarkDescAttr.GetIsFdoExp(); } ; // if false than no value should be written for placemark name
  FDOKGKML_API FdoExpression* GetPlacemarkDescFdoExpr() { return m_PlacemarkDescAttr.GetFdoExpr(); };

  FDOKGKML_API FdoString* GetGeomAltModeExp() const { return m_GeomAltModeAttr.GetExp(); }
  void SetGeomAltModeExp(FdoString* val) { m_GeomAltModeAttr.SetExp(val); }
  FDOKGKML_API bool GetIsGeomAltModeFdoExp() { return m_GeomAltModeAttr.GetIsFdoExp(); } ; // if false than no value should be written for placemark name
  FDOKGKML_API FdoExpression* GetGeomAltModeFdoExpr() { return m_GeomAltModeAttr.GetFdoExpr(); };

  FDOKGKML_API FdoString* GetGeomTessellateExp() const { return m_GeomTessellateAttr.GetExp(); }
  void SetGeomTessellateExp(FdoString* val) { m_GeomTessellateAttr.SetExp(val); }
  FDOKGKML_API bool GetIsGeomTessellateFdoExp() { return m_GeomTessellateAttr.GetIsFdoExp(); } ; // if false than no value should be written for placemark name
  FDOKGKML_API FdoExpression* GetGeomTessellateFdoExpr() { return m_GeomTessellateAttr.GetFdoExpr(); };
  
  
  FDOKGKML_API void AddAttr(FdoString* Name,FdoString* Expression);
  c_KgKmlFdoClassAttributeConfigCollection* GetAttrCollection() { return FDO_SAFE_ADDREF(m_AttrCollection.p); }

  
protected:
  void ParseExp(FdoString* Exp,FdoPtr<FdoExpression>& FdoExp,bool& IsParsed);   


};

class  c_KgKmlFdoClassConfigCollection : public FdoCollection<c_KgKmlFdoClassConfig,FdoException>
{

protected:
  virtual void Dispose(); 

public:

  c_KgKmlFdoClassConfig* FindKmlFdoClassConfig(FdoString* ClassName);
  void ReadConfigXML();
};