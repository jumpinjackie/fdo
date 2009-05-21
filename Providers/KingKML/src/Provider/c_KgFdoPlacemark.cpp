#include "StdAfx.h"
#include "c_KgFdoPlacemark.h"

c_KgFdoPlacemark::c_KgFdoPlacemark(void)
{
}

c_KgFdoPlacemark::c_KgFdoPlacemark(int FdoId,kmldom::PlacemarkPtr Placemark)
{
  m_FdoId = FdoId;
  m_Placemark = Placemark;
  m_IsKmlGeometryParsed=false;
}


c_KgFdoPlacemark::~c_KgFdoPlacemark(void)
{
}


void c_KgFdoPlacemark::Dispose ()
{
  delete this;
}

bool c_KgFdoPlacemark::HasGeometry()
{
  
  
  if( !m_IsKmlGeometryParsed )
  {
    ParsePlacemarkGeometry();    
  }
  
  if( m_FdoGeometry ) return true;    
  
  return false;
}

FdoIEnvelope* c_KgFdoPlacemark::GetGeometryExtent()
{

  if( !m_IsKmlGeometryParsed )
  {
    ParsePlacemarkGeometry();    
  }

  if( !m_FdoGeometry ) return NULL;    

  
  
  return m_FdoGeometry->GetEnvelope();
    
}

void c_KgFdoPlacemark::ParsePlacemarkGeometry()
{
  m_IsKmlGeometryParsed=true;
  m_FdoGeometry=NULL;
  if( !m_Placemark->has_geometry() )
  {
    return;
  }
  
  kmldom::GeometryPtr kmlgeom = m_Placemark->get_geometry();  
  switch(kmlgeom->Type())
  {
    case kmldom::Type_Point:
    {
      const kmldom::PointPtr kmlpoint = kmldom::AsPoint(kmlgeom);
      if(kmlpoint && kmlpoint->has_coordinates())
      {
        const kmldom::CoordinatesPtr coords = kmlpoint->get_coordinates();
        int coordnum = coords->get_coordinates_array_size();
        if( coordnum >= 1 )
        {
          kmldom::Vec3 kmlvec3 = coords->get_coordinates_array_at(0);
          double ords[3];
          ords[0] = kmlvec3.get_longitude();
          ords[1] = kmlvec3.get_latitude();
          ords[2] = kmlvec3.get_altitude();
          
          FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
          m_FdoGeometry = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z,&ords[0]);
        }
        
      }
    }
    break;
    case kmldom::Type_LineString:
    {
      const kmldom::LineStringPtr kmlline = kmldom::AsLineString(kmlgeom);
      if(kmlline && kmlline->has_coordinates())
      {
        const kmldom::CoordinatesPtr coords = kmlline->get_coordinates();
        int pointnum = coords->get_coordinates_array_size();
        if( pointnum >= 1 )
        {                
          double *ords = new double[3*pointnum];
          double *ordptr=ords;
          for(int ind=0;ind<pointnum;ind++)
          {
            kmldom::Vec3 kmlvec3 = coords->get_coordinates_array_at(ind);
            *ordptr++ = kmlvec3.get_longitude();
            *ordptr++ = kmlvec3.get_latitude();
            *ordptr++ = kmlvec3.get_altitude();            
          }
          FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
          m_FdoGeometry = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z,3*pointnum,ords);
          delete ords;
        }
      }
    }
    break;
    case kmldom::Type_MultiGeometry:
      {
        const kmldom::MultiGeometryPtr kmlmulti = kmldom::AsMultiGeometry(kmlgeom);
        m_FdoGeometry = CreateFdoGeometry(kmlmulti);
       
      }
      break;
    case kmldom::Type_Polygon:
    {
    // For now polygon will read only outer ring
      const kmldom::PolygonPtr kmlpolygon = kmldom::AsPolygon(kmlgeom);
      if(kmlpolygon )
      {
        const kmldom::OuterBoundaryIsPtr outring = kmlpolygon->get_outerboundaryis();
        if( outring && outring->has_linearring() )
        {
          const kmldom::LinearRingPtr ring = outring->get_linearring();
          if( ring && ring->has_coordinates())
          {
            const kmldom::CoordinatesPtr coords = ring->get_coordinates();            
            int pointnum = coords->get_coordinates_array_size();
            if( pointnum >= 1 )
            {                
              double *ords = new double[3*pointnum];
              double *ordptr=ords;
              for(int ind=0;ind<pointnum;ind++)
              {
                kmldom::Vec3 kmlvec3 = coords->get_coordinates_array_at(ind);
                *ordptr++ = kmlvec3.get_longitude();
                *ordptr++ = kmlvec3.get_latitude();
                *ordptr++ = kmlvec3.get_altitude();            
              }
              FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
              //m_FdoGeometry = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z,3*pointnum,ords);
              
              FdoPtr<FdoILinearRing> fdoring = gf->CreateLinearRing(FdoDimensionality_XY|FdoDimensionality_Z,3*pointnum,ords);
              m_FdoGeometry = gf->CreatePolygon(fdoring,NULL);
              delete ords;
            }
          }
        }
      }
    }
    break;
  }
  
}//end of c_KgFdoPlacemark::ParsePlacemarkGeometry

FdoIGeometry* c_KgFdoPlacemark::CreateFdoGeometry(const kmldom::GeometryPtr& kmlgeom)
{
 
  FdoIGeometry* fdogeom=NULL;
  
  switch(kmlgeom->Type())
  {
  case kmldom::Type_Point:
    {
      const kmldom::PointPtr kmlpoint = kmldom::AsPoint(kmlgeom);
      if(kmlpoint && kmlpoint->has_coordinates())
      {
        const kmldom::CoordinatesPtr coords = kmlpoint->get_coordinates();
        int coordnum = coords->get_coordinates_array_size();
        if( coordnum >= 1 )
        {
          kmldom::Vec3 kmlvec3 = coords->get_coordinates_array_at(0);
          double ords[3];
          ords[0] = kmlvec3.get_longitude();
          ords[1] = kmlvec3.get_latitude();
          ords[2] = kmlvec3.get_altitude();

          FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
          fdogeom = gf->CreatePoint(FdoDimensionality_XY|FdoDimensionality_Z,&ords[0]);
        }

      }
    }
    break;
  case kmldom::Type_LineString:
    {
      const kmldom::LineStringPtr kmlline = kmldom::AsLineString(kmlgeom);
      if(kmlline && kmlline->has_coordinates())
      {
        const kmldom::CoordinatesPtr coords = kmlline->get_coordinates();
        int pointnum = coords->get_coordinates_array_size();
        if( pointnum >= 1 )
        {                
          double *ords = new double[3*pointnum];
          double *ordptr=ords;
          for(int ind=0;ind<pointnum;ind++)
          {
            kmldom::Vec3 kmlvec3 = coords->get_coordinates_array_at(ind);
            *ordptr++ = kmlvec3.get_longitude();
            *ordptr++ = kmlvec3.get_latitude();
            *ordptr++ = kmlvec3.get_altitude();            
          }
          FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
          fdogeom = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z,3*pointnum,ords);
          delete ords;
        }
      }
    }
    break;
    case kmldom::Type_MultiGeometry:
    {
      const kmldom::MultiGeometryPtr kmlmulti = kmldom::AsMultiGeometry(kmlgeom);
      if(kmlmulti )
      {
      
      // KML has one mutli geometry type
      // because map and mapguide can't display multigeomtry type
      // I need to try to create multi-point,line,polygon types when possiblle.
        FdoGeometryType alltype = FdoGeometryType_None;
        int counttypes = 0;
      
        FdoPtr<FdoGeometryCollection> fdogmcoll = FdoGeometryCollection::Create();
        
        int count = kmlmulti->get_geometry_array_size();
        for(int ind=0;ind<count;ind++)
        {
          const kmldom::GeometryPtr gm = kmlmulti->get_geometry_array_at(ind);
          FdoPtr<FdoIGeometry> fdogm = CreateFdoGeometry(gm);
          if( fdogm.p )
          {
            if( counttypes==0 )
            {
              alltype = fdogm->GetDerivedType();
              counttypes++;
            }
            else
            {
              if( alltype != fdogm->GetDerivedType()) counttypes++;
            }
            fdogmcoll->Add( fdogm );
          }
          
          
        }
        
        FdoPtr<FdoFgfGeometryFactory> gf=FdoFgfGeometryFactory::GetInstance();
        bool isspecificmulti = false;
        if( counttypes==1 )
        {
          switch(alltype)
          {
            case FdoGeometryType_Point:
            {
              FdoPtr<FdoPointCollection> fdopointcoll = FdoPointCollection::Create();

              int count = fdogmcoll->GetCount();
              for(int ind=0;ind<count;ind++ )
              {
                FdoPtr<FdoIGeometry> fdogm = fdogmcoll->GetItem(ind);

                FdoIPoint * fdopoint = (FdoIPoint*)fdogm.p;
                fdopointcoll->Add(fdopoint);
              }

              fdogeom = gf->CreateMultiPoint(fdopointcoll);

              isspecificmulti=true;
            }
            break;
            case FdoGeometryType_LineString:
            {
              FdoPtr<FdoLineStringCollection> fdolinecoll = FdoLineStringCollection::Create();
              
              int count = fdogmcoll->GetCount();
              for(int ind=0;ind<count;ind++ )
              {
                FdoPtr<FdoIGeometry> fdogm = fdogmcoll->GetItem(ind);
                
                FdoILineString * fdoline = (FdoILineString*)fdogm.p;
                fdolinecoll->Add(fdoline);
              }
              
              fdogeom = gf->CreateMultiLineString(fdolinecoll);
              
              isspecificmulti=true;
            }
            break;
            case FdoGeometryType_Polygon:
              {
                FdoPtr<FdoPolygonCollection> fdopolycoll = FdoPolygonCollection::Create();

                int count = fdogmcoll->GetCount();
                for(int ind=0;ind<count;ind++ )
                {
                  FdoPtr<FdoIGeometry> fdogm = fdogmcoll->GetItem(ind);

                  FdoIPolygon * fdopoly = (FdoIPolygon*)fdogm.p;
                  fdopolycoll->Add(fdopoly);
                }

                fdogeom = gf->CreateMultiPolygon(fdopolycoll);

                isspecificmulti=true;
              }
            break;
          }
        }
        
        // no specific mutli (multipoint multiline or multi polygon) create multigeomtry type
        if( !isspecificmulti )
        {
        
          gf = FdoFgfGeometryFactory::GetInstance();
          fdogeom = gf->CreateMultiGeometry(fdogmcoll);
        }
        
        
        
      }
    }
    break;
  case kmldom::Type_Polygon:
    {
      // For now polygon will read only outer ring
      const kmldom::PolygonPtr kmlpolygon = kmldom::AsPolygon(kmlgeom);
      if(kmlpolygon )
      {
        const kmldom::OuterBoundaryIsPtr outring = kmlpolygon->get_outerboundaryis();
        if( outring && outring->has_linearring() )
        {
          const kmldom::LinearRingPtr ring = outring->get_linearring();
          if( ring && ring->has_coordinates())
          {
            const kmldom::CoordinatesPtr coords = ring->get_coordinates();            
            int pointnum = coords->get_coordinates_array_size();
            if( pointnum >= 1 )
            {                
              double *ords = new double[3*pointnum];
              double *ordptr=ords;
              for(int ind=0;ind<pointnum;ind++)
              {
                kmldom::Vec3 kmlvec3 = coords->get_coordinates_array_at(ind);
                *ordptr++ = kmlvec3.get_longitude();
                *ordptr++ = kmlvec3.get_latitude();
                *ordptr++ = kmlvec3.get_altitude();            
              }
              FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
              //m_FdoGeometry = gf->CreateLineString(FdoDimensionality_XY|FdoDimensionality_Z,3*pointnum,ords);

              FdoPtr<FdoILinearRing> fdoring = gf->CreateLinearRing(FdoDimensionality_XY|FdoDimensionality_Z,3*pointnum,ords);
              fdogeom = gf->CreatePolygon(fdoring,NULL);
              delete ords;
            }
          }
        }
      }
    }
    break;
  }

  return fdogeom;
}//end of c_KgFdoPlacemark::CreateFdoGeometry

void c_KgFdoPlacemark::ParsePlacemarkData()
{
  
  m_FdoPropertyValues = FdoPropertyValueCollection::Create();
  // Add name 
  if( m_Placemark->has_name() )
  {
    std:string placename = m_Placemark->get_name(); 
    FdoStringP fdostr;
    fdostr = placename.c_str();
    FdoPtr<FdoDataValue> dval = FdoDataValue::Create(fdostr);
    FdoPtr<FdoPropertyValue> kml_fdo_name = FdoPropertyValue::Create(L"KML_FDO_NAME", dval);
    m_FdoPropertyValues->Add(kml_fdo_name);
  }

  // Add property description
  if( m_Placemark->has_description() )
  {
    std::string desc = m_Placemark->get_description(); 
    FdoStringP fdostr;
    fdostr = desc.c_str();
    FdoPtr<FdoDataValue> dval = FdoDataValue::Create(fdostr); //fdostr);
    FdoPtr<FdoPropertyValue> kml_fdo_desc = FdoPropertyValue::Create(L"KML_FDO_DESCRIPTION", dval);
    m_FdoPropertyValues->Add(kml_fdo_desc);
  }
  // Add FDO identifier  
  {  
    FdoInt32 newval = m_FdoId;
    FdoPtr<FdoDataValue> dval = FdoDataValue::Create(newval);
    FdoPtr<FdoPropertyValue> kml_fdo_id = FdoPropertyValue::Create(L"KML_FDO_ID", dval);
    m_FdoPropertyValues->Add(kml_fdo_id);
  }
  
  

  // Now from Placemark extended data create other properties
  if( m_Placemark->has_extendeddata() )
  {  
    kmldom::ExtendedDataPtr extdata = m_Placemark->get_extendeddata();
    for(int i=0;i<extdata->get_extendeddatamember_array_size();i++)
    {
      const kmldom::ExtendedDataMemberPtr extmemb = extdata->get_extendeddatamember_array_at(i);
      const kmldom::DataPtr data = kmldom::AsData(extmemb);
      if( data && data->has_name() )
      {
        std::string dname = data->get_name();
        FdoStringP s1 = dname.c_str();
        
        std::string sval = data->get_value();
        FdoStringP fdo_sval = sval.c_str();
        
        FdoPtr<FdoDataValue> dval = FdoDataValue::Create(fdo_sval);
        FdoPtr<FdoPropertyValue> kml_fdo_data = FdoPropertyValue::Create(s1, dval);
        if( dval->IsNull() )
        {
          assert(false);
        }
        
        m_FdoPropertyValues->Add(kml_fdo_data);

      }
    }    

  }



  

}//end of c_KgFdoPlacemark::ParsePlacemarkData

FdoByteArray* c_KgFdoPlacemark::GetFdoGeometry()
{
  FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
  return gf->GetFgf(m_FdoGeometry);
  
}

FdoPropertyValueCollection* c_KgFdoPlacemark::GetPropertyValues()
{
  
  return FDO_SAFE_ADDREF(m_FdoPropertyValues.p);
}

void c_KgFdoPlacemark::SetGeometry( FdoByteArray* FdoGeom )
{
  FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
  FdoIGeometry* igeom = gf->CreateGeometryFromFgf(FdoGeom);
  
  if( igeom )
  {  
    switch( igeom->GetDerivedType() )
    {
      case FdoGeometryType_Point:
      {
        FdoIPoint * ipoint = (FdoIPoint*)igeom;
        int orddim = ipoint->GetDimensionality();
        const double * ords = ipoint->GetOrdinates();
        
        double x,y,z;
        
        x= *ords++;
        y= *ords++;
        if( orddim & FdoDimensionality_Z )    z= *ords++;
        else z=0;
        
        kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();  // Create <coordinates>.  
        kmldom::CoordinatesPtr coordinates = factory->CreateCoordinates();  // Create <coordinates>-122.0816695,37.42052549<coordinates>  
        coordinates->add_latlngalt(y,x,z);  // Create <Point> and give it <coordinates>.  
        kmldom::PointPtr point = factory->CreatePoint();  
        point->set_coordinates(coordinates);  // point takes ownership
        
        
        m_Placemark->set_geometry(point);
      }
      break; 
      case FdoGeometryType_LineString:
      {
        FdoILineString * iline = (FdoILineString*)igeom;
        int count = iline->GetCount();
        int orddim = iline->GetDimensionality();
        const double * ords = iline->GetOrdinates();
        
        
        kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();  // Create <coordinates>.  
        kmldom::CoordinatesPtr coordinates = factory->CreateCoordinates();  // Create <coordinates>-122.0816695,37.42052549<coordinates>  
        
        double x,y,z;
        while(count--)
        {
          x= *ords++;
          y= *ords++;
          if( orddim & FdoDimensionality_Z )    z= *ords++;
          else z=0;
          if( orddim & FdoDimensionality_M )    ords++;
          
          coordinates->add_latlngalt(y,x,z);  // Create <Point> and give it <coordinates>.  
        }
        
        
        kmldom::LineStringPtr kmlline = factory->CreateLineString();
        kmlline->set_coordinates(coordinates);  // point takes ownership


        m_Placemark->set_geometry(kmlline);
      }
      break;
      case FdoGeometryType_Polygon:
      {
        FdoIPolygon * ipolygon = (FdoIPolygon*)igeom;
        FdoPtr<FdoILinearRing> ring = ipolygon->GetExteriorRing();
        int count = ring->GetCount();
        int orddim = ring->GetDimensionality();
        const double * ords = ring->GetOrdinates();
        
        kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();  // Create <coordinates>.  
        kmldom::CoordinatesPtr coordinates = factory->CreateCoordinates();  // Create <coordinates>-122.0816695,37.42052549<coordinates>  

        double x,y,z;
        while(count--)
        {
          x= *ords++;
          y= *ords++;
          if( orddim & FdoDimensionality_Z )    z= *ords++;
          else z=0;
          if( orddim & FdoDimensionality_M )    ords++;

          coordinates->add_latlngalt(y,x,z);  // Create <Point> and give it <coordinates>.  
        }


        kmldom::LinearRingPtr kmlring = factory->CreateLinearRing();
        kmlring->set_coordinates(coordinates);  // point takes ownership

        kmldom::OuterBoundaryIsPtr kmloutboundary = factory->CreateOuterBoundaryIs();
        kmloutboundary->set_linearring(kmlring);
        
        kmldom::PolygonPtr kmlpolygon = factory->CreatePolygon();
        kmlpolygon->set_outerboundaryis(kmloutboundary);

        m_Placemark->set_geometry(kmlpolygon);
      }
      break;
    }
  }
}

void c_KgFdoPlacemark::SetPlacemarkName( FdoString* Value )
{
  FdoStringP fstr = Value;
  m_Placemark->set_name((const char*)fstr);
}//end of c_KgFdoPlacemark::SetPlacemarkName

void c_KgFdoPlacemark::SetPlacemarkDescription( FdoString* Value )
{
  FdoStringP fstr = Value;
  m_Placemark->set_description((const char*)fstr);
}//end of c_KgFdoPlacemark::SetPlacemarkDescription

void c_KgFdoPlacemark::SetAttribute( const std::string&  Name,const std::string& Value )
{
   kmldom::ExtendedDataPtr extdata = m_Placemark->get_extendeddata();
  if( !extdata.get() )
  {
    kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
    kmldom::ExtendedDataPtr newext;
    newext = factory->CreateExtendedData();
    m_Placemark->set_extendeddata(newext);
    extdata = m_Placemark->get_extendeddata();
   
  }
  int count = extdata->get_extendeddatamember_array_size();
  bool found=false;
  int ind=0;
  while(!found && ind<count )
  {
    const kmldom::ExtendedDataMemberPtr extmemb = extdata->get_extendeddatamember_array_at(ind);
    const kmldom::DataPtr data = kmldom::AsData(extmemb);
    if( data && data->has_name() )
    {
      std::string dname = data->get_name();
      if( dname.compare(Name) == 0)
      {
        data->set_value(Value);
        found=true;
      }

    }
    
    ind++;
  }
  
  if( !found )
  {
    kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();  // Create <coordinates>.  


    const kmldom::DataPtr newdata = factory->CreateData();
    newdata->set_name(Name);
    newdata->set_value(Value);
    extdata->add_extendeddatamember(newdata);
    
  }
}