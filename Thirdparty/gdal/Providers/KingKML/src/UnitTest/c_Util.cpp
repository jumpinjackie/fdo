#include "StdAfx.h"
#include "c_Util.h"

c_Util::c_Util(void)
{
}

c_Util::~c_Util(void)
{
}

kmldom::PlacemarkPtr  c_Util::CreatePlacemarkPoint(const char*Name,const char* Desc,double X,double Y,double Z) 
{
  // Get the factory singleton to create KML elements.
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();

  // Create <coordinates>.
  kmldom::CoordinatesPtr coordinates = factory->CreateCoordinates();
  // Create <coordinates>-122.0816695,37.42052549<coordinates>
  coordinates->add_latlngalt(Y,X,Z);

  // Create <Point> and give it <coordinates>.
  kmldom::PointPtr point = factory->CreatePoint();
  point->set_coordinates(coordinates);  // point takes ownership

  // Create <Placemark> and give it a <name> and the <Point>.
  kmldom::PlacemarkPtr placemark1 = factory->CreatePlacemark();
  placemark1->set_name(Name);
  placemark1->set_description(Desc);
  placemark1->set_geometry(point);  // placemark takes ownership
  
  return placemark1;

}

void  c_Util::CreateTestKML() 
{
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();

  // Create <kml> and give it <Placemark>.
  kmldom::KmlPtr kml = factory->CreateKml();
  kmldom::DocumentPtr doc = factory->CreateDocument();
  
  kmldom::PlacemarkPtr placemark1 = CreatePlacemarkPoint("Name 1","Desc 1",1,-1,0);
  doc->add_feature(placemark1);
  
  kmldom::PlacemarkPtr placemark2 = CreatePlacemarkPoint("Name 2","Desc 2",2,-2,0);
  doc->add_feature(placemark2);
  
  kml->set_feature(doc);  // kml takes ownership.

  // Serialize to XML
  std::string xml = kmldom::SerializePretty(kml);

  if( !kmlbase::File::WriteStringToFile(xml,"UnitTest.kml") )
  {
     CPPUNIT_FAIL( "c_Util::CreateTestKML  Unable to write UnitTest.kml" );
  }
 
}

void  c_Util::DeleteTestNewKML() 
{
  kmlbase::File::Delete("TestNew.kml");
  
}


FdoIConnection* c_Util::OpenUnitTestConnection()
{
  try
  {
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.KML");
    //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
    conn->SetConnectionString(L"File=UnitTest.kml");

    conn->Open();

    return FDO_SAFE_ADDREF(conn.p);

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  return NULL;
}



FdoIConnection* c_Util::OpenUnitTestConnection_NewKML()
{
  try
  {
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.KML");
    //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
    conn->SetConnectionString(L"File=TestNew.kml");

    conn->Open();

    return FDO_SAFE_ADDREF(conn.p);

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  return NULL;
}


FdoIConnection* c_Util::OpenUnitTestConnection_ParceleKMZ()
{
  try
  {
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"King.KML");
    //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
    conn->SetConnectionString(L"File=Parcele.kmz");

    conn->Open();

    return FDO_SAFE_ADDREF(conn.p);

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  return NULL;
}

FdoIConnection* c_Util::OpenUnitTestConnection_ParceleSDF()
{
  try
  {
    FdoPtr<IConnectionManager> manager = FdoFeatureAccessManager::GetConnectionManager ();
    FdoPtr<FdoIConnection> conn = manager->CreateConnection (L"OSGeo.SDF");
    //conn->SetConnectionString(L"UserName=ose;Password=ose;ServiceName=temp");
    conn->SetConnectionString(L"File=c:\\testdata\\Parcele.sdf");

    conn->Open();

    return FDO_SAFE_ADDREF(conn.p);

  }  
  catch(FdoException* ex)
  {
    FdoStringP str = ex->GetExceptionMessage();
    ex->Release();
    CPPUNIT_FAIL( (const char*)str );
  }
  return NULL;
}

FdoFeatureSchema* c_Util::CreateTestFdoSchema()
{
  FdoFeatureSchema* schema = FdoFeatureSchema::Create(L"UnitTestFdoSchema", L"");          
  

  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  
  FdoPtr<FdoFeatureClass> fdoclass = c_Util::CreateTestFdofeatureClass();
  classes->Add(fdoclass);
  return schema;
}

FdoFeatureClass* c_Util::CreateTestFdofeatureClass()
{
  FdoFeatureClass* fclass = FdoFeatureClass::Create(L"UnitTestFdoClass",L"");

  FdoInt32 fdo_geom_type = FdoGeometricType_Point|FdoGeometricType_Curve|FdoGeometricType_Surface;
  

  FdoPtr<FdoPropertyDefinitionCollection> propcol = fclass->GetProperties();

  FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(L"KML_FDO_GEOMETRY", L"");                        

  gpd->SetGeometryTypes(fdo_geom_type);  

  gpd->SetSpatialContextAssociation( D_SPATIALCONTEXT_DEFAULT_NAME );

  propcol->Add(gpd);
  fclass->SetGeometryProperty(gpd);

  // Add name  
  {
    FdoPtr<FdoDataPropertyDefinition> kml_fdo_name = FdoDataPropertyDefinition::Create(L"KML_FDO_NAME", L"");
    kml_fdo_name->SetReadOnly(false);
    kml_fdo_name->SetDataType(FdoDataType_String);
    kml_fdo_name->SetNullable(true);
    //kml_fdo_name->SetLength(2048);  
    propcol->Add(kml_fdo_name);
    
  }

  // Add property description
  {
    FdoPtr<FdoDataPropertyDefinition> kml_fdo_desc = FdoDataPropertyDefinition::Create(L"KML_FDO_DESCRIPTION", L"");
    kml_fdo_desc->SetReadOnly(false);
    kml_fdo_desc->SetDataType(FdoDataType_String);
    kml_fdo_desc->SetNullable(true);
    //kml_fdo_desc->SetLength(2048);
    propcol->Add(kml_fdo_desc);
  
  }

  // Add FDO identifier  
  { 
    FdoPtr<FdoDataPropertyDefinition> kml_fdo_id = FdoDataPropertyDefinition::Create(L"KML_FDO_ID", L"");
    kml_fdo_id->SetDataType(FdoDataType_Int32);
    kml_fdo_id->SetNullable(false);
    //kml_fdo_id->SetIsAutoGenerated(true);
    kml_fdo_id->SetReadOnly(false);
    propcol->Add(kml_fdo_id);
  
    FdoPtr<FdoDataPropertyDefinitionCollection> identcol = fclass->GetIdentityProperties();
    identcol->Add( kml_fdo_id );
  }

  // Now from Placemark extended data create other properties
  {
    FdoStringP s1 = "StringAttr1";
    FdoPtr<FdoDataPropertyDefinition> kml_data = FdoDataPropertyDefinition::Create(s1,L"");
    kml_data->SetDataType(FdoDataType_String);
    kml_data->SetLength(2048);
    propcol->Add(kml_data);
  }

  {
    FdoStringP s1 = "Int32Attr1";
    FdoPtr<FdoDataPropertyDefinition> kml_data = FdoDataPropertyDefinition::Create(s1,L"");
    kml_data->SetDataType(FdoDataType_Int32);
    propcol->Add(kml_data);
  }


  return fclass;
}//end of c_Util::CreateTestFdofeatureClass

bool c_Util::CompareSchema( FdoFeatureSchema* Schema1,FdoFeatureSchema* Schema2 )
{
  FdoStringP n1 =   Schema1->GetName();
  if( n1.ICompare(Schema2->GetName()) != 0 ) return false;
  
  FdoPtr<FdoClassCollection> classes1 = Schema1->GetClasses();
  FdoPtr<FdoClassCollection> classes2 = Schema2->GetClasses();
   
  if( classes1->GetCount() != classes2->GetCount()) return false;
  
  int count = classes1->GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<FdoClassDefinition> class1 = classes1->GetItem(0);
    FdoPtr<FdoClassDefinition> class2 = classes2->FindItem(class1->GetName());
    
    if( !class2.p ) return false;
    
    if (!c_Util::CompareClass( class1,class2) ) return false;
  } 
  
  
  return true;
}

bool c_Util::CompareClass( FdoClassDefinition* Class1,FdoClassDefinition* Class2 )
{
  FdoStringP n1 =   Class1->GetName();
  if( n1.ICompare(Class2->GetName()) != 0 ) return false;
  
  
  FdoPtr<FdoPropertyDefinitionCollection> propdef_coll1 = Class1->GetProperties();
  FdoPtr<FdoPropertyDefinitionCollection> propdef_coll2 = Class2->GetProperties();
  
  
  
  if( propdef_coll1->GetCount() != propdef_coll2->GetCount()) return false;
  
  
  int count = propdef_coll1->GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<FdoPropertyDefinition> propdef1 = propdef_coll1->GetItem(0);
    FdoPtr<FdoPropertyDefinition> propdef2 = propdef_coll2->FindItem(propdef1->GetName());

    if( !propdef2.p ) return false;
    
    if (!c_Util::CompareFdoPropertyDefinition( propdef1,propdef2) ) return false;
  } 
  
  return true;
}


bool c_Util::CompareFdoPropertyDefinition( FdoPropertyDefinition* Prop1,FdoPropertyDefinition* Prop2 )
{
  FdoStringP n1 =   Prop1->GetName();
  if( n1.ICompare(Prop2->GetName()) != 0 ) return false;


  if( Prop1->GetPropertyType() != Prop2->GetPropertyType() ) return false;
  
  if( Prop1->GetPropertyType() == FdoPropertyType_DataProperty )
  {
    FdoDataPropertyDefinition* datadef1 = (FdoDataPropertyDefinition*)Prop1;
    FdoDataPropertyDefinition* datadef2 = (FdoDataPropertyDefinition*)Prop2;
    
    if( datadef1->GetDataType() != datadef2->GetDataType() ) return false;
    
    
  }
  return true;
}
