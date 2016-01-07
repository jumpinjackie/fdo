#pragma once

class c_Util
{
public:
  c_Util(void);
public:
  ~c_Util(void);
  
  static void DeleteTestNewKML();
  static FdoIConnection* OpenUnitTestConnection();
  static FdoIConnection* OpenUnitTestConnection_NewKML();
  static FdoIConnection* OpenUnitTestConnection_ParceleKMZ();
  static FdoIConnection* OpenUnitTestConnection_ParceleSDF();
  static void CreateTestKML();
  static kmldom::PlacemarkPtr CreatePlacemarkPoint(const char*Name,const char* Desc,double X,double Y,double Z);
  
  static FdoFeatureSchema* CreateTestFdoSchema();
  static FdoFeatureClass* CreateTestFdofeatureClass();
  
  
  static bool CompareSchema(FdoFeatureSchema* Schema1,FdoFeatureSchema* Schema2);
  static bool CompareClass( FdoClassDefinition* Class1,FdoClassDefinition* Class2 );
  static bool CompareFdoPropertyDefinition( FdoPropertyDefinition* Prop1,FdoPropertyDefinition* Prop2 );
  

};
