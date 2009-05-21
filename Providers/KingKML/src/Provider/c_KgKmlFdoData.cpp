/*
* Copyright (C) 2008  SL-King d.o.o
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
#include "stdafx.h"
#include <FdoExpressionEngine.h>


#include "c_KgKmlFdoData.h"
#include "c_FdoKml_API.h"

#include "Sdf.h"
#include "SdfConnection.h"
#include "Sdfprovider.h"



 FdoPtr<FdoFeatureSchema> c_KgKmlFdoData::m_LastApplySchema_FdoSchema;
 std::string c_KgKmlFdoData::m_LastApplySchema_KmlFile;

c_KgKmlFdoData::c_KgKmlFdoData (void) 
{
  m_FdoIdentCounter=1;
  m_IsDataChanged=false;
  m_KmlFdoClassConfig = new c_KgKmlFdoClassConfig();;
}

c_KgKmlFdoData::~c_KgKmlFdoData (void)
{

}

void c_KgKmlFdoData::Dispose ()
{
  if( m_SdfConn.p )
  {
    m_SdfConn->Close();
    m_SdfConn = NULL;
  }
  delete this;
}

c_KgKmlSpatialContext* c_KgKmlFdoData::CreateDefaultSpatialContext()
{

  // Create Deafult Spatial Context   
  FdoPtr<c_KgKmlSpatialContext> spatial_context;
  spatial_context = new c_KgKmlSpatialContext();
  spatial_context->SetName(D_SPATIALCONTEXT_DEFAULT_NAME);          // L"Default";

  spatial_context->SetCoordSysName(D_SPATIALCONTEXT_CSYS_DEFAULT_NAME); // "EPSG:4326";

  spatial_context->SetCoordinateSystemWkt(D_SPATIALCONTEXT_CSYS_DEFAULT_WKT);

  //spatial_context->SetOraSridDesc(orasriddesc);

  


  return FDO_SAFE_ADDREF(spatial_context.p);  
}


void c_KgKmlFdoData::CreateDefaultSchemaDesc()
{
  // Now create Default schema and Spatail Context  
  m_FdoSchemaDesc = NULL;

  FdoPtr<FdoFeatureSchemaCollection> fschema=NULL;
  FdoPtr<FdoKgKmlPhysicalSchemaMapping> phschema;

  fschema = FdoFeatureSchemaCollection::Create(NULL);

  phschema = FdoKgKmlPhysicalSchemaMapping::Create();
  FdoPtr<c_KgKmlSpatialContextCollection> sc_collection = new c_KgKmlSpatialContextCollection();

  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(D_KML_FDO_DEFAULT_SCHEMA, L"");          
  fschema->Add(schema.p);

  FdoPtr<FdoClassCollection> classes = schema->GetClasses();

  FdoPtr<FdoKgKmlClassCollection> phys_classes = phschema->GetClasses();

 
  // Create Deafult Spatial Context   
  FdoPtr<c_KgKmlSpatialContext> spatial_context = CreateDefaultSpatialContext();
  
  sc_collection->Insert(0,spatial_context);


  m_FdoSchemaDesc = c_KgKmlSchemaDesc::Create();
  m_FdoSchemaDesc->SetDesc(fschema,phschema,sc_collection); 
}

void c_KgKmlFdoData::CreateFdoClass(kmldom::Placemark* Place)
{

  FdoPtr<FdoFeatureSchemaCollection> fschema=NULL;

  fschema = m_FdoSchemaDesc->GetFeatureSchema();

  FdoPtr<FdoFeatureSchema> schema = fschema->GetItem(0);          
 
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();

 
  m_KmlFdoClassDef = c_FdoKml_API::CreateFdoFeatureClass(D_KML_FDO_DEFAULT_CLASS,Place,D_SPATIALCONTEXT_DEFAULT_NAME,m_KmlFdoClassConfig);
  classes->Add(m_KmlFdoClassDef);

}

void c_KgKmlFdoData::ApplySchemaToSDF()
{
  FdoPtr<FdoFeatureSchemaCollection> fschema_coll = m_FdoSchemaDesc->GetFeatureSchema();
  if( fschema_coll->GetCount()>0 )
  {
    FdoPtr<FdoFeatureSchema> fschema = fschema_coll->GetItem(0);
    // now apply new schema
    FdoPtr<FdoIApplySchema> apply_comm = (FdoIApplySchema*)m_SdfConn->CreateCommand(FdoCommandType_ApplySchema);  
    apply_comm->SetFeatureSchema(fschema);
    apply_comm->Execute();
  }
}
void c_KgKmlFdoData::ApplySpatialContextToSDF()
{
  FdoPtr<c_KgKmlSpatialContextCollection> sc_coll = m_FdoSchemaDesc->GetSpatialContexts();
  
  if( !sc_coll || (sc_coll->GetCount()==0) ) return ;
  
  FdoPtr<c_KgKmlSpatialContext> spatial_context = sc_coll->GetItem(0);
  
  
  // CReate Spatial context in SDF  
  FdoPtr<FdoICreateSpatialContext> comm_create_sc = (FdoICreateSpatialContext*)m_SdfConn->CreateCommand(FdoCommandType_CreateSpatialContext);  
  if( comm_create_sc.p )
  {
    comm_create_sc->SetName( spatial_context->GetName() );
    comm_create_sc->SetDescription( spatial_context->GetDescription() );  
    comm_create_sc->SetCoordinateSystem( spatial_context->GetCoordinateSystem() );
    comm_create_sc->SetCoordinateSystemWkt( spatial_context->GetCoordinateSystem() );
    comm_create_sc->SetExtentType( spatial_context->GetExtentType() );
    comm_create_sc->SetXYTolerance( spatial_context->GetXYTolerance() );
    comm_create_sc->SetZTolerance( spatial_context->GetZTolerance() );    
    comm_create_sc->Execute();
  }
}

// This funcion iscalled when AplpySchema is executed against KML provider.
// In will be sucesfull
void c_KgKmlFdoData::SetFdoSchema(FdoFeatureSchema* FdoSchema)
{
  FdoPtr<FdoFeatureSchema> newschema = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchema(FdoSchema);
  
  FdoPtr<FdoClassCollection> class_coll = newschema->GetClasses();
  int count = class_coll->GetCount();
  
  bool found=false;
  int ind=0;
  while(!found && ind<count)
  {
    FdoPtr<FdoClassDefinition> fdoclass = class_coll->GetItem(0);
    if( fdoclass->GetClassType() == FdoClassType_FeatureClass)
    {
      m_KmlFdoClassDef = (FdoFeatureClass*)FdoCommonSchemaUtil::DeepCopyFdoClassDefinition((FdoClassDefinition*)fdoclass); //(FdoFeatureClass*)FDO_SAFE_ADDREF(fdoclass.p);
      found=true;
    }
    ind++;
  }
  
  if( !found ) return ;
  
  
  // Create Deafult Spatial Context   
  FdoPtr<c_KgKmlSpatialContextCollection> sc_collection = new c_KgKmlSpatialContextCollection();
  FdoPtr<c_KgKmlSpatialContext> spatial_context = CreateDefaultSpatialContext();
  sc_collection->Insert(0,spatial_context);
  
  
  // Now create Default schema and Spatial Context  
  

  FdoPtr<FdoFeatureSchemaCollection> fschema_coll=NULL;
  FdoPtr<FdoKgKmlPhysicalSchemaMapping> phschema = FdoKgKmlPhysicalSchemaMapping::Create();
  fschema_coll = FdoFeatureSchemaCollection::Create(NULL);
  FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(newschema->GetName(), L"");          
  FdoPtr<FdoClassCollection> classes = schema->GetClasses();
  classes->Add(m_KmlFdoClassDef);
  
  fschema_coll->Add(schema);
  
  
  // Add FDO identifier  
  { 
    FdoPtr<FdoPropertyDefinitionCollection> propcol = m_KmlFdoClassDef->GetProperties();
    FdoPtr<FdoPropertyDefinition> prop1 = propcol->FindItem(L"KML_FDO_ID");
    if( !prop1.p )
    {
      
      FdoPtr<FdoDataPropertyDefinition> kml_fdo_id = FdoDataPropertyDefinition::Create(L"KML_FDO_ID", L"");
      kml_fdo_id->SetDataType(FdoDataType_Int32);
      kml_fdo_id->SetNullable(false);
      //kml_fdo_id->SetIsAutoGenerated(true);
      kml_fdo_id->SetReadOnly(false);
      propcol->Add(kml_fdo_id);
      
      FdoPtr<FdoDataPropertyDefinitionCollection> identcol = m_KmlFdoClassDef->GetIdentityProperties();
      identcol->Clear();
      identcol->Add( kml_fdo_id );
    }
  }
  m_KmlFdoClassConfig->SetFdoProp_KmlFdoId(L"KML_FDO_ID");

// On geometry property set spatial context name to be used  
  FdoPtr<FdoGeometricPropertyDefinition> geomprop = m_KmlFdoClassDef->GetGeometryProperty();
  if( geomprop ) geomprop->SetSpatialContextAssociation(spatial_context->GetName());


  m_FdoSchemaDesc = NULL;
  m_FdoSchemaDesc = c_KgKmlSchemaDesc::Create();
  m_FdoSchemaDesc->SetDesc(fschema_coll,phschema,sc_collection); 
  
  if( m_SdfConn.p ) m_SdfConn->Close();
  OpenMemorySDF();
  ApplySpatialContextToSDF();
  ApplySchemaToSDF();
  
  
  m_LastApplySchema_FdoSchema = FDO_SAFE_ADDREF(FdoSchema);
  m_LastApplySchema_KmlFile = m_KmlFileName;
}

int c_KgKmlFdoData::ParseKML(const string& FileName)
{
  m_KmlFileName = FileName;
  if( kmlbase::File::Exists(FileName))
  {
    return ParseExistingKML(FileName);
  }
  
  
  kmldom::KmlFactory * factory = kmldom::KmlFactory::GetFactory();  
  kmldom::KmlPtr kml = factory->CreateKml(); 
  m_KmlRoot =  kml;
  m_KmlKml = kml;

  kmldom::DocumentPtr doc = factory->CreateDocument();
  m_KmlContainer = doc;
  m_KmlKml->set_feature(doc);
  
  
  
  if( m_LastApplySchema_FdoSchema.p && m_KmlFileName.compare(m_LastApplySchema_KmlFile)==0 )
  {
    SetFdoSchema(m_LastApplySchema_FdoSchema);
    return 0;
  }
  
    

  
// ce KML file ne obstaja  
  OpenMemorySDF();
  
//
  CreateDefaultSchemaDesc();
  CreateFdoClass(NULL);
  
  
  
  
  
// Apply Spatial Context to SDF
  ApplySpatialContextToSDF();



  
// now apply new schema
  ApplySchemaToSDF();
  
  return 0;
}

int c_KgKmlFdoData::ParseExistingKML(const string& FileName)
{


  // Read the file.
  std::string file_data;
  if (!kmlbase::File::ReadFileToString(FileName, &file_data)) 
  {
    throw FdoConnectionException::Create(L"Failed opening KMZ file!");

  }
  
  
  // If the file was KMZ, extract the KML file.
  std::string kml;
  if (kmlengine::KmzFile::IsKmz(file_data)) 
  {
    m_IsKMZ = true;
    
    boost::scoped_ptr<kmlengine::KmzFile> kmz_file(kmlengine::KmzFile::OpenFromFile(FileName.c_str()));
    if (!kmz_file.get()) 
    {
      //cout << "Failed opening KMZ file" << endl;
      //return NULL;
      throw FdoConnectionException::Create(L"Failed opening KMZ file!");
    }
    if (!kmz_file->ReadKml(&kml)) 
    {
      throw FdoConnectionException::Create(L"Failed to read KML from KMZ!");

    }
  } 
  else 
  {
    m_IsKMZ = false;
    kml = file_data;
  }

// Parse it.
  std::string errors;
  m_KmlRoot = kmldom::Parse(kml, &errors);
  if( m_KmlRoot.get() )
  {
    const kmldom::KmlPtr kml = kmldom::AsKml(m_KmlRoot);
    if( kml.get() )
    {
      m_KmlKml = kml;
    }
    else
    {
      kmldom::KmlFactory * factory = kmldom::KmlFactory::GetFactory();
      m_KmlKml = factory->CreateKml();
      
      m_KmlRoot->AddElement(m_KmlKml);
     
    }
    
  }
  else
  {
    kmldom::KmlFactory * factory = kmldom::KmlFactory::GetFactory();
    m_KmlKml = factory->CreateKml();
    m_KmlRoot = m_KmlKml;
    
  }
 
// Find first container - place were new placemarks will e added 
  kmlengine::element_vector_t folder_vector;
  kmlengine::GetElementsById(m_KmlKml, kmldom::Type_Folder, &folder_vector);
  if( folder_vector.size()>=1 )
  {
    const kmldom::FolderPtr folder = kmldom::AsFolder(folder_vector[0]);
    m_KmlContainer = folder;
    
  }
  else
  {
    kmlengine::element_vector_t doc_vector;
    kmlengine::GetElementsById(m_KmlKml, kmldom::Type_Document, &doc_vector);
    if( doc_vector.size()>=1 )
    {
      const kmldom::DocumentPtr doc = kmldom::AsDocument(doc_vector[0]);
      m_KmlContainer = doc;

    }
  }
  
  if( !m_KmlContainer.get() )
  {
    kmldom::KmlFactory * factory = kmldom::KmlFactory::GetFactory();
    
    kmldom::DocumentPtr doc = factory->CreateDocument();
    m_KmlContainer = doc;

    m_KmlKml->set_feature(doc);
     
  }
  
// Now create Default schema and Spatial Context  
  CreateDefaultSchemaDesc();

  FdoPtr<FdoFeatureSchemaCollection> schema_coll = m_FdoSchemaDesc->GetFeatureSchema();
  FdoPtr<FdoKgKmlPhysicalSchemaMapping> phschema = m_FdoSchemaDesc->GetPhysicalSchemaMapping();

  FdoPtr<FdoFeatureSchema> fschema = schema_coll->GetItem(0);

  FdoPtr<FdoClassCollection> classes = fschema->GetClasses();

  FdoPtr<FdoKgKmlClassCollection> phys_classes = phschema->GetClasses();


  
  
// Now I should go and parse placemark geometries and Create Identifiers ( FdoId ).
  m_FdoIdentCounter=1;
  kmlengine::element_vector_t elements_vector;
  kmlengine::GetElementsById(m_KmlKml, kmldom::Type_Placemark, &elements_vector);
  
  // From first placemark create Faeture class
  
  if( elements_vector.size() >= 1 )
  {
    const kmldom::PlacemarkPtr firstplacemark = kmldom::AsPlacemark(elements_vector[0]);
    CreateFdoClass(firstplacemark.get());
   
  }
  else
  {
    CreateFdoClass(NULL);
  }
  
  for (size_t i = 0; i < elements_vector.size(); ++i) 
  {
    const kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(elements_vector[i]);
    if( placemark )
    {
      FdoPtr<c_KgFdoPlacemark> fdoplacemark = new c_KgFdoPlacemark(m_FdoIdentCounter++,placemark);
      fdoplacemark->ParsePlacemarkGeometry();
      fdoplacemark->ParsePlacemarkData();
      m_FdoPlacemarks.Add(fdoplacemark);
    }
    
  }

  
  m_KmlFileName = FileName;
  
  
// Create SDF for it
  OpenMemorySDF();
  
  
  ApplySpatialContextToSDF();

  // now apply new schema
  ApplySchemaToSDF();
    
    
// Insert Data into SDF
    
   // create insert comm on sdf
  FdoPtr<FdoIInsert> inscomm = (FdoIInsert*)m_SdfConn->CreateCommand(FdoCommandType_Insert);  
  inscomm->SetFeatureClassName(m_KmlFdoClassDef->GetName());
  FdoPtr<FdoPropertyValueCollection> insprop_col = inscomm->GetPropertyValues();
   
  
   
   
  int count = m_FdoPlacemarks.GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<c_KgFdoPlacemark> fdoplacemark = m_FdoPlacemarks.GetItem(ind);
    //if( fdoplacemark->HasGeometry() )
    {
      insprop_col->Clear();
      
      FdoPtr<FdoPropertyValueCollection> placeprops = fdoplacemark->GetPropertyValues();
      {
        int count = placeprops->GetCount();
        for(int ind=0;ind<count;ind++)
        {
          FdoPtr<FdoPropertyValue> val = placeprops->GetItem(ind);
          
          //FdoPtr<FdoValueExpression> exp = val->GetValue();          
          //FdoPtr<FdoPropertyValue> val2 = FdoPropertyValue::Create(val->GetName(),exp);
          
          insprop_col->Add(val);    
          
       
        }
      }
        
      FdoPtr<FdoPropertyDefinitionCollection> fdoprops = m_KmlFdoClassDef->GetProperties();
      {
        int count = fdoprops->GetCount();
        for(int ind=0;ind<count;ind++)
        {     
          FdoPtr<FdoPropertyDefinition> prop = fdoprops->GetItem(ind);
          switch(prop->GetPropertyType())
          {
            case FdoPropertyType_DataProperty:
            {
              //FdoPtr<FdoPropertyValue> propval = FdoPropertyValue::Create(prop->GetName(),NULL);
              //insprop_col->Add(propval);      
            }
            break;
            case FdoPropertyType_GeometricProperty:
            {
              FdoPtr<FdoGeometryValue> geomval;
              FdoPtr<FdoPropertyValue> propval;
              
              if( fdoplacemark->HasGeometry() )
              {
                FdoPtr<FdoByteArray> fdogeom = fdoplacemark->GetFdoGeometry();
                geomval = FdoGeometryValue::Create( fdogeom );
                propval = FdoPropertyValue::Create(prop->GetName(),geomval);
                insprop_col->Add(propval);      
              }
              
              
            }
            break;
          }
        } 
      }
      
      
                
      FdoPtr<FdoIFeatureReader>  insreader = inscomm->Execute();
    }
  }
    
  return 0;

}//end of c_KgKmlFdoData::ParseKML




c_KgKmlSchemaDesc* c_KgKmlFdoData::GetSchemaDesc()
{
  if( m_FdoSchemaDesc.p == NULL )
  {
    FdoPtr<FdoFeatureSchemaCollection> fschema=NULL;
    FdoPtr<FdoKgKmlPhysicalSchemaMapping> phschema;
    
    fschema = FdoFeatureSchemaCollection::Create(NULL);

    phschema = FdoKgKmlPhysicalSchemaMapping::Create();
    FdoPtr<c_KgKmlSpatialContextCollection> sc_collection = new c_KgKmlSpatialContextCollection();
    
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"KingKmlFdo", L"");          
    fschema->Add(schema.p);

    FdoPtr<FdoClassCollection> classes = schema->GetClasses();

    FdoPtr<FdoKgKmlClassCollection> phys_classes = phschema->GetClasses();

   
// Create Deafult Spatial Context   
    FdoPtr<c_KgKmlSpatialContext> spatial_context;
    spatial_context = new c_KgKmlSpatialContext();
    spatial_context->SetName(D_SPATIALCONTEXT_DEFAULT_NAME);          // L"Default";

    spatial_context->SetCoordSysName(D_SPATIALCONTEXT_CSYS_DEFAULT_NAME); // "EPSG:4326";

    spatial_context->SetCoordinateSystemWkt(D_SPATIALCONTEXT_CSYS_DEFAULT_WKT);

    //spatial_context->SetOraSridDesc(orasriddesc);

    sc_collection->Insert(0,spatial_context);
    
    
    m_FdoSchemaDesc = c_KgKmlSchemaDesc::Create();
    m_FdoSchemaDesc->SetDesc(fschema,phschema,sc_collection);

  }
  return FDO_SAFE_ADDREF(m_FdoSchemaDesc.p);
}//end of c_KgKmlFdoData::GetSchemaDesc

SdfConnection* c_KgKmlFdoData::GetSdfConn()
{
  return FDO_SAFE_ADDREF(m_SdfConn.p);
}



void c_KgKmlFdoData::SyncPlacemarksFromReader( FdoIReader* Reader )
{
  if( !m_KmlFdoClassDef ) 
  {
    throw FdoException::Create(L"KmlFdo Class is not defined!");
  }
  
  FdoPtr<FdoPropertyDefinitionCollection> fdopropcol = m_KmlFdoClassDef->GetProperties();
  FdoPtr<FdoGeometricPropertyDefinition> fdogeomprop = m_KmlFdoClassDef->GetGeometryProperty();
  
  while(Reader->ReadNext())
  {
    
    int kmlfdoid=0;
    if( !Reader->IsNull(L"KML_FDO_ID")) 
      kmlfdoid = Reader->GetInt32(L"KML_FDO_ID");
    
    FdoPtr<c_KgFdoPlacemark> fdoplace = m_FdoPlacemarks.FindFdoPlacemark(kmlfdoid);
    if( fdoplace )    
    {
      UpdateFdoPlacemark(fdoplace,Reader);
    }
    else
    {
      kmldom::KmlFactory * factory = kmldom::KmlFactory::GetFactory();  
      kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
      
      m_KmlContainer->add_feature(placemark);
    
      
      FdoPtr<c_KgFdoPlacemark> fdoplace = new c_KgFdoPlacemark(kmlfdoid,placemark);
      m_IsDataChanged = true;
      UpdateFdoPlacemark(fdoplace,Reader);
      m_FdoPlacemarks.Add(fdoplace);
     
    }
  }
}//end of c_KgKmlFdoData::SyncPlacemarksFromReader


void c_KgKmlFdoData::SyncPlacemarksFromSDF()
{
  FdoPtr<SdfConnection> sdfconn = GetSdfConn();
  FdoPtr<FdoISelect> selectcomm = (FdoISelect*)sdfconn->CreateCommand(FdoCommandType_Select);
  
  selectcomm->SetFeatureClassName(m_KmlFdoClassDef->GetName())   ;
  FdoPtr<FdoIFeatureReader> reader = selectcomm->Execute();
  
  SyncPlacemarksFromReader(reader);
}

/*
void c_KgKmlFdoData::UpdateFdoPlacemark(c_KgFdoPlacemark * FdoPlacemark, FdoIReader* Reader )
{
  if( !m_KmlFdoClassDef ) 
  {
    throw FdoException::Create(L"KmlFdo Class is not defined!");
  }

  FdoPtr<FdoPropertyDefinitionCollection> fdopropcol = m_KmlFdoClassDef->GetProperties();
  FdoPtr<FdoGeometricPropertyDefinition> fdogeomprop = m_KmlFdoClassDef->GetGeometryProperty();

  
  m_IsDataChanged = true;
  
// Update Geometry
  if( fdogeomprop )
  {
    try
    {
      FdoPtr<FdoByteArray> fdogeom = Reader->GetGeometry(fdogeomprop->GetName());
      FdoPlacemark->SetGeometry(fdogeom);
    }
    catch (...)
    {

    }

  }   
  

// Update Placemark Description           
  
// Update attributes  
  int count = fdopropcol->GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<FdoPropertyDefinition> fdoprop = fdopropcol->GetItem(ind);
    try
    {
      if( fdoprop->GetPropertyType() == FdoPropertyType_DataProperty )
      {
        FdoDataPropertyDefinition* dataprop = (FdoDataPropertyDefinition*)fdoprop.p;
        
        FdoStringP fdostr_name = dataprop->GetName();
        std::string kml_attr_name = (const char * )fdostr_name;
        
        bool isattr = true;
        // Check for Placemark Name
        FdoString * fdo_placename = m_KmlFdoClassConfig->GetFdoProp_PlacemarkName();
        if(  fdo_placename && fdostr_name.ICompare( fdo_placename)==0 )
        {
          isattr = false;
          try
          {
            if( !Reader->IsNull(fdo_placename))
            {
              FdoStringP fdostr_val = Reader->GetString(fdo_placename);
              FdoPlacemark->SetPlacemarkName(fdostr_val);
            }
          }
          catch (...)
          {
          }
        }
        // Check for Placemark Description
        FdoString * fdo_placedesc = m_KmlFdoClassConfig->GetFdoProp_PlacemarkDesc();
        if(  fdo_placedesc && fdostr_name.ICompare( fdo_placedesc)==0 )
        {
          isattr = false;
          try
          {
            if( !Reader->IsNull(fdo_placedesc))
            {
              FdoStringP fdostr_val = Reader->GetString(fdo_placedesc);
              FdoPlacemark->SetPlacemarkDescription(fdostr_val);
            }
          }
          catch (...)
          {
          }
        }   
        // Check for KML_FDO_ID
        FdoString * fdo_kmlid = m_KmlFdoClassConfig->GetFdoProp_KmlFdoId();
        if(  fdo_kmlid && fdostr_name.ICompare( fdo_kmlid)==0 )
        {
          isattr = false;
          try
          {
           // do nothing with kml_fdo_id
          }
          catch (...)
          {
          }
        }       
        
        //if( isattr && !Reader->IsNull(dataprop->GetName()))
        if( isattr )
        {
          switch(dataprop->GetDataType())
          {
            case FdoDataType_String:
            {
              
              FdoStringP fdostr_val = Reader->GetString(dataprop->GetName());          
              std::string str_val = (const char * )fdostr_val;
              
              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Int16:
            {
              FdoInt16 val = Reader->GetInt16(dataprop->GetName());
              
              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Int32:
            {
              FdoInt32 val = Reader->GetInt32(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Int64:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoInt64 val = Reader->GetInt64(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Double:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoDouble val = Reader->GetDouble(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val,FdoDataType_Double);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Single:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoFloat val = Reader->GetSingle(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val,FdoDataType_Single);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Boolean:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoBoolean val = Reader->GetBoolean(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_Byte:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoByte val = Reader->GetByte(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            case FdoDataType_DateTime:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoDateTime val = Reader->GetDateTime(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
            
            
          }
        }
      }
      
    }
    catch (...)
    {
    	
    }
  }
  
}//end of c_KgKmlFdoData::UpdateFdoPlacemark
*/

void c_KgKmlFdoData::UpdateFdoPlacemark(c_KgFdoPlacemark * FdoPlacemark, FdoIReader* Reader )
{
  if( !m_KmlFdoClassDef ) 
  {
    throw FdoException::Create(L"KmlFdo Class is not defined!");
  }

  FdoPtr<FdoPropertyDefinitionCollection> fdopropcol = m_KmlFdoClassDef->GetProperties();
  FdoPtr<FdoGeometricPropertyDefinition> fdogeomprop = m_KmlFdoClassDef->GetGeometryProperty();


  m_IsDataChanged = true;

  // Update Geometry
  if( fdogeomprop )
  {
    try
    {
      FdoPtr<FdoByteArray> fdogeom = Reader->GetGeometry(fdogeomprop->GetName());
      FdoPlacemark->SetGeometry(fdogeom);
    }
    catch (...)
    {

    }

  }   

  FdoPtr<FdoExpressionEngine> expengine = FdoExpressionEngine::Create(Reader,m_KmlFdoClassDef,NULL); 
  
  
  // Set Placemark Name
  if( m_KmlFdoClassConfig->GetIsPlacemarkNameFdoExp() ) 
  {
    FdoPtr<FdoExpression> exp = m_KmlFdoClassConfig->GetPlacemarkNameFdoExpr();
    FdoPtr<FdoLiteralValue> litvalue = expengine->Evaluate(exp);
    if ( litvalue && (litvalue->GetLiteralValueType() == FdoLiteralValueType_Data) ) 
    {
      FdoDataValue *dataValue = static_cast<FdoDataValue *> (litvalue.p);

      FdoStringP strp;
      if( dataValue->GetDataType() == FdoDataType_String )
      {
        FdoStringValue * stringvalue = (FdoStringValue*)dataValue;
        strp = stringvalue->GetString();
      }
      else
      {
        strp = dataValue->ToString();
      }
      FdoPlacemark->SetPlacemarkName(strp);
    }
  }
  
  // Set Placemark Desc
  if( m_KmlFdoClassConfig->GetIsPlacemarkDescFdoExp() )
  {
     
    FdoPtr<FdoExpression> exp = m_KmlFdoClassConfig->GetPlacemarkDescFdoExpr();
    FdoPtr<FdoLiteralValue> litvalue = expengine->Evaluate(exp);
    if ( litvalue && (litvalue->GetLiteralValueType() == FdoLiteralValueType_Data) )
    {
      FdoDataValue *dataValue = static_cast<FdoDataValue *> (litvalue.p);

      FdoStringP strp;
      if( dataValue->GetDataType() == FdoDataType_String )
      {
        FdoStringValue * stringvalue = (FdoStringValue*)dataValue;
        strp = stringvalue->GetString();
      }
      else
      {
        strp = dataValue->ToString();
      }
      FdoPlacemark->SetPlacemarkDescription(strp);
    }
  }


  // Update Placemark Description  
  
           

  // Update attributes  
/*  
  int count = fdopropcol->GetCount();
  for(int ind=0;ind<count;ind++)
  {
    FdoPtr<FdoPropertyDefinition> fdoprop = fdopropcol->GetItem(ind);
    try
    {
      if( fdoprop->GetPropertyType() == FdoPropertyType_DataProperty )
      {
        FdoDataPropertyDefinition* dataprop = (FdoDataPropertyDefinition*)fdoprop.p;

        FdoStringP fdostr_name = dataprop->GetName();
        std::string kml_attr_name = (const char * )fdostr_name;

        bool isattr = true;
        
          
        // Check for KML_FDO_ID
        FdoString * fdo_kmlid = m_KmlFdoClassConfig->GetFdoProp_KmlFdoId();
        if(  fdo_kmlid && fdostr_name.ICompare( fdo_kmlid)==0 )
        {
          isattr = false;
          try
          {
            // do nothing with kml_fdo_id
          }
          catch (...)
          {
          }
        }       

        //if( isattr && !Reader->IsNull(dataprop->GetName()))
        if( isattr )
        {
          switch(dataprop->GetDataType())
          {
          case FdoDataType_String:
            {

              FdoStringP fdostr_val = Reader->GetString(dataprop->GetName());          
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Int16:
            {
              FdoInt16 val = Reader->GetInt16(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Int32:
            {
              FdoInt32 val = Reader->GetInt32(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Int64:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoInt64 val = Reader->GetInt64(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Double:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoDouble val = Reader->GetDouble(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val,FdoDataType_Double);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Single:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoFloat val = Reader->GetSingle(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val,FdoDataType_Single);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Boolean:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoBoolean val = Reader->GetBoolean(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_Byte:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoByte val = Reader->GetByte(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;
          case FdoDataType_DateTime:
            {
              FdoStringP fdostr_name = dataprop->GetName();
              FdoDateTime val = Reader->GetDateTime(dataprop->GetName());

              FdoPtr<FdoDataValue> dataval = FdoDataValue::Create(val);
              FdoStringP fdostr_val = dataval->ToString();

              std::string str_name = (const char * )fdostr_name;
              std::string str_val = (const char * )fdostr_val;

              FdoPlacemark->SetAttribute(kml_attr_name,str_val);
            }
            break;


          }
        }
      }

    }
    catch (...)
    {

    }
  }
*/
}//end of c_KgKmlFdoData::UpdateFdoPlacemark

void c_KgKmlFdoData::OpenMemorySDF()
{
  // Create SDF for it
  m_SdfConn = SdfConnection::Create();  
  FdoPtr<FdoIConnectionInfo> conninfo = m_SdfConn->GetConnectionInfo();
  FdoPtr<FdoIConnectionPropertyDictionary> connprop = conninfo->GetConnectionProperties();

  connprop->SetProperty(PROP_NAME_FILE,L":memory:");
  connprop->SetProperty(PROP_NAME_RDONLY,L"false");

  m_SdfConn->Open();
}

void c_KgKmlFdoData::Flush()
{
  if( m_IsDataChanged && m_KmlFileName.length()>0 )
  {
    if( !m_KmlRoot.get() )
    {
      kmldom::KmlFactory * factory = kmldom::KmlFactory::GetFactory();  
      kmldom::KmlPtr kml = factory->CreateKml(); 
      m_KmlRoot =  kml;
      
      kmldom::DocumentPtr doc = factory->CreateDocument();
      
      int count = m_FdoPlacemarks.GetCount();
      for(int ind=0;ind<count;ind++)
      {
        FdoPtr<c_KgFdoPlacemark> fdoplace = m_FdoPlacemarks.GetItem(ind);
        
        //kml->AddElement();
        doc->add_feature(fdoplace->GetPlacemark());
        
      }
      
      kml->set_feature(doc);
      
    }
    std::string xmldata = kmldom::SerializePretty(m_KmlRoot);
    
       
    kmlbase::File::WriteStringToFile(xmldata,m_KmlFileName);
    m_IsDataChanged=false;
  }
}