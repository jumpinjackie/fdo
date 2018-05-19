// 
//  
//  Copyright (C) 2006 Autodesk Inc.
//  
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of version 2.1 of the GNU Lesser
//  General Public License as published by the Free Software Foundation.
//  
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//  
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//  

#include "stdafx.h"
#include "OgrFdoUtil.h"
#include "OgrProvider.h"

void tilde2dot(std::string& mbfc)
{
    //TODO HACK remove . from feature class names -- this is a workaround for Oracle
    for (int i=(int)mbfc.length()-1; i>=0; i--)
    {
        if (mbfc[i] == '~')
            mbfc[i] = '.';
    }
}

void dot2tilde(std::wstring& wname)
{
    //TODO HACK remove . from feature class names -- this is a workaround for Oracle
    for (int i=(int)wname.length()-1; i>=0; i--)
    {
        if (wname[i] == '.')
            wname[i] = '~';
    }
}

FdoClassDefinition* OgrFdoUtil::ConvertClass(OGRLayer* layer, FdoIdentifierCollection* requestedProps)
{
    return ConvertClass(NULL, layer, requestedProps);
}

FdoClassDefinition* OgrFdoUtil::ConvertClass(OgrConnection* connection, OGRLayer* layer, FdoIdentifierCollection* requestedProps)
{
    OGRFeatureDefn* fdefn = layer->GetLayerDefn();
    CHECK_CPL_ERROR(FdoSchemaException);

    const char* name = fdefn->GetName();
    std::wstring wname = A2W_SLOW(name);

    dot2tilde(wname);

#if DEBUG
    printf ("Feature class name: %s\n", name);
#endif

    FdoPtr<FdoFeatureClass> fc = FdoFeatureClass::Create(wname.c_str(), L"");
    FdoPtr<FdoClassCapabilities> classCapabilities;
    FdoPolygonVertexOrderRule vertexOrderRule = FdoPolygonVertexOrderRule_CCW;
    bool bStrictness = false;
    if (connection)
    {
        classCapabilities = FdoClassCapabilities::Create(*fc.p);
        classCapabilities->SetSupportsLocking(false);
        classCapabilities->SetSupportsLongTransactions(false);
        classCapabilities->SetSupportsWrite(true);
        fc->SetCapabilities(classCapabilities);

#if GDAL_VERSION_MAJOR < 2
        OGRDataSource* dataStore = connection->GetOGRDataSource();
#else
        GDALDataset* dataStore = connection->GetOGRDataSource();
#endif
        if (NULL != dataStore)
        {
#if GDAL_VERSION_MAJOR < 2
            OGRSFDriver* driver = dataStore->GetDriver();
#else
            GDALDriver* driver = dataStore->GetDriver();
#endif
            if (NULL != driver)
            {
#if GDAL_VERSION_MAJOR < 2
                const char* name = driver->GetName();
#else
                const char* name = dataStore->GetDriverName();
#endif

                // As far as I know, ESRI Shapefile is the only feature source that uses
                // clockwise vertex order. I assume other feature sources use counterclockwise
                // vertex order.
                if (strcmp(name, "ESRI Shapefile") == 0)
                {
                    vertexOrderRule = FdoPolygonVertexOrderRule_CW;
                    bStrictness = true;
                }
            }
        }
    }

    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();

    //data properties (attributes)
    int propcount = fdefn->GetFieldCount();

    for (int j=0; j<propcount; j++)
    {
        OGRFieldDefn* field = fdefn->GetFieldDefn(j);
        const char* pname = field->GetNameRef();
        std::wstring wpname = A2W_SLOW(pname);
        dot2tilde(wpname);
#ifdef DEBUG
        printf("Attribute : %s\n", name);
#endif
        FdoDataType dt;
        OGRFieldType etype = field->GetType();
        bool add = true;
        switch (etype)
        {
        case OFTInteger: dt = FdoDataType_Int32; break;
        case OFTString: dt = FdoDataType_String; break;
        case OFTWideString: dt = FdoDataType_String; break;
        case OFTReal: dt = FdoDataType_Double; break;
        case OFTDate:
        case OFTTime:
        case OFTDateTime: dt = FdoDataType_DateTime; break;
#if GDAL_VERSION_MAJOR >= 2
        case OFTInteger64: dt = FdoDataType_Int64; break;
#endif
        default: add=false; break; //unknown property type
        }

        if (add)
        {
            //check if property is on the optional requested property list
            FdoPtr<FdoIdentifier> found = (requestedProps) ? requestedProps->FindItem(wpname.c_str()) : NULL;

            //if it's on the list or there was no list at all, then add the property
            if (!requestedProps || requestedProps->GetCount() == 0 || (requestedProps && found.p))
            {
                FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(wpname.c_str(), L"");

                dpd->SetDataType(dt);
                dpd->SetLength(field->GetWidth());
                dpd->SetPrecision(field->GetPrecision());
#if GDAL_VERSION_MAJOR >= 2
                //Since such concepts are now available in GDAL 2.x, we might as well map them
                dpd->SetNullable((field->IsNullable() == TRUE));
                const char* defaultVal = field->GetDefault();
                if (defaultVal)
                {
                    std::wstring wDefaultVal = A2W_SLOW(defaultVal);
                    dpd->SetDefaultValue(wDefaultVal.c_str());
                }
#endif
                pdc->Add(dpd);
            }
        }
#ifdef DEBUG
        else
        {
            printf("Unknown data type for attributed: %s\n", name);
        }
#endif
    }

    //add geometry property -- this code assumes there is one
    const char* geomname = layer->GetGeometryColumn();
    if (*geomname == 0) geomname = PROP_GEOMETRY;
    std::wstring wgeomname = A2W_SLOW(geomname);
    dot2tilde(wgeomname);

    //check if property is on the optional requested property list
    FdoPtr<FdoIdentifier> found = (requestedProps) ? requestedProps->FindItem(wgeomname.c_str()) : NULL;

    //if it's on the list or there was no list at all, then add the property
    if (!requestedProps || requestedProps->GetCount() == 0 || (requestedProps && found.p))
    {
#if DEBUG
        printf ("Geometry column : %s\n", geomname);
#endif

        FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(wgeomname.c_str(), L"");

        OGRwkbGeometryType gt = fdefn->GetGeomType();

        switch (gt)
        {
        case wkbNone: gpd = NULL; break;
        case wkbPolygon:
        case wkbMultiPolygon: gpd->SetGeometryTypes(FdoGeometricType_Surface); break;
        case wkbPoint:
        case wkbMultiPoint: gpd->SetGeometryTypes(FdoGeometricType_Point); break;
        case wkbLineString:
        case wkbMultiLineString: gpd->SetGeometryTypes(FdoGeometricType_Curve); break;
        default: gpd->SetGeometryTypes(7); break;
        }

        if (gpd)
        {
            gpd->SetSpatialContextAssociation(wname.c_str());
            pdc->Add(gpd);
            fc->SetGeometryProperty(gpd);

            if (classCapabilities != NULL)
            {
                // Set vertex order and strictness rule for geometry property
                classCapabilities->SetPolygonVertexOrderRule(gpd->GetName(), vertexOrderRule);
                classCapabilities->SetPolygonVertexOrderStrictness(gpd->GetName(), bStrictness);
            }
        }
    }

    //identity property
    const char* idname = layer->GetFIDColumn();
    if (*idname == 0) idname = PROP_FID;
    std::wstring widname = A2W_SLOW(idname);
    dot2tilde(widname);
#if DEBUG
    printf ("Identity column : %s\n", idname);
#endif
    //check if property is on the optional requested property list
    found = (requestedProps) ? requestedProps->FindItem(widname.c_str()) : NULL;

    //if it's on the list or there was no list at all, then add the property
    if (!requestedProps || requestedProps->GetCount() == 0 || (requestedProps && found.p))
    {
        //see if FID column was in the attributes we processed earlier
        FdoPtr<FdoDataPropertyDefinition> fid = (FdoDataPropertyDefinition*)pdc->FindItem(widname.c_str());

        //if not, create one. 
        //TODO: we may not need to do this at all
        if (!fid.p)
        {
            fid = FdoDataPropertyDefinition::Create(widname.c_str(), L"");
#if GDAL_VERSION_MAJOR < 2
            fid->SetDataType(FdoDataType_Int32);
#else
            fid->SetDataType(FdoDataType_Int64); //FID is GIntBig in 2.x
#endif
            pdc->Add(fid);
        }

        fid->SetIsAutoGenerated(true);
        //set the ID property of the feature class
        FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
        idpdc->Add(fid);
    }

    return FDO_SAFE_ADDREF(fc.p);
}



//copies feature properties from an FDO propety value collection
//into an OGR feature -- used for inserting new feature and updating
//existing ones
void OgrFdoUtil::ConvertFeature(FdoPropertyValueCollection* src, OGRFeature* dst, OGRLayer* layer, const std::string& encoding)
{
    //update the feature properties
    //this code is not fast at all
    //since we could in theory pre-convert all the property values
    //and then apply them to all the featues, but in the most
    //common case we are only updating one feature so it's a wash.
    for (int i=src->GetCount()-1; i>=0; i--)
    {
        //convert propvals
        FdoPtr<FdoPropertyValue> pv = src->GetItem(i);

        FdoString* propName = FdoPtr<FdoIdentifier>(pv->GetName())->GetName();
        W2A_PROPNAME(propName);

        const char* geomname = layer->GetGeometryColumn();
        if (*geomname == 0) geomname = PROP_GEOMETRY;

        FdoPtr<FdoValueExpression> value = pv->GetValue();

        if (strcmp(geomname, mbpropName) == 0)
        {
            //is it the geometry property?

            FdoGeometryValue* gv = dynamic_cast<FdoGeometryValue*>(value.p);

            if (gv)
            {
                FdoPtr<FdoByteArray> fgf = gv->GetGeometry();

                unsigned char* wkb = new unsigned char[fgf->GetCount()]; //WKB is smaller than FGF due to lack of dimensionality flag
                
                int len = Fgf2Wkb(fgf->GetData(), wkb);

                OGRGeometry* geom = NULL;
                OGRGeometryFactory::createFromWkb(wkb, NULL, &geom, len);

                dst->SetGeometryDirectly(geom); //the feature now owns this pointer

                delete [] wkb;
            }
        }
        else
        {
            //or is it a data property?
            OGRFeatureDefn* fdefn = layer->GetLayerDefn();
            OGRFieldDefn* field = fdefn->GetFieldDefn(fdefn->GetFieldIndex(mbpropName));

            FdoDataType dt = (FdoDataType)-1;
            OGRFieldType etype = field->GetType();

            switch (etype)
            {
            case OFTInteger:
                {
                    FdoInt32Value* iv = dynamic_cast<FdoInt32Value*>(value.p);
                    if (iv && !iv->IsNull()) 
                        dst->SetField(mbpropName, (int)iv->GetInt32());
                }
                break;
#if GDAL_VERSION_MAJOR >= 2
            case OFTInteger64:
                {
                    FdoInt64Value* i64v = dynamic_cast<FdoInt64Value*>(value.p);
                    FdoInt32Value* iv = dynamic_cast<FdoInt32Value*>(value.p);
                    if (i64v && !i64v->IsNull())
                        dst->SetField(mbpropName, (GIntBig)i64v->GetInt64());
                    if (iv && !iv->IsNull()) //Allow int32
                        dst->SetField(mbpropName, (int)iv->GetInt32());
                }
                break;
#endif
            case OFTString:
                {
                    FdoStringValue* sv = dynamic_cast<FdoStringValue*>(value.p);
                    if (sv && !sv->IsNull())
                    {
                        FdoString* str = sv->GetString();
                        std::string mbstr = W2A_SLOW(str, encoding);
                        dst->SetField(mbpropName, mbstr.c_str());
                    }
                }
                break;
            case OFTWideString: 
                {
                    FdoStringValue* sv = dynamic_cast<FdoStringValue*>(value.p);
                    if (sv && !sv->IsNull())
                    {
                        FdoString* str = sv->GetString();
                        //dst->SetField(mbpropName, str); //unsupported so far
                    }
                }
                break;
            case OFTReal:
                {
                    FdoDoubleValue* iv = dynamic_cast<FdoDoubleValue*>(value.p);
                    if (iv && !iv->IsNull()) 
                        dst->SetField(mbpropName, (double)iv->GetDouble());
                }
                break;
            case OFTDate:
            case OFTTime:
            case OFTDateTime:
                {
                    FdoDateTimeValue* dv = dynamic_cast<FdoDateTimeValue*>(value.p);

                    if (dv && !dv->IsNull())
                    {
                        FdoDateTime dt = dv->GetDateTime();

                        if (dt.IsDate())
                        {
                            dst->SetField(mbpropName, dt.year, dt.month, dt.day);
                        }
                        else if (dt.IsDateTime())
                        {
                            dst->SetField(mbpropName, dt.year, dt.month, dt.day, dt.hour, dt.minute, (int)dt.seconds);
                        }
                        else if (dt.IsTime())
                        {
                            dst->SetField(mbpropName, 0, 0, 0, dt.hour, dt.minute, (int)dt.seconds);
                        }
                    }
                }
                break;
            default: break; //unknown property type
            }
        }
    }
}


//attempts to map an FDO filter to an attribute and spatial filters
//used to set up a feature query for Select commands
//This function assumes MapGuide style queries -- either 
//an attribute filter or a simple spatial filter or a binary
//combination of the two
void OgrFdoUtil::ApplyFilter(OGRLayer* layer, FdoFilter* filter)
{
    FdoFilter* spatial = NULL;
    FdoFilter* attr = NULL;

    //zero out the filters
    layer->SetAttributeFilter(NULL);
    layer->SetSpatialFilter(NULL);
    
    //this is lame, but much shorter than implementing
    //all of FdoIFilterProcessor and FdoIExpressionProcessor
    if (dynamic_cast<FdoSpatialCondition*>(filter))
        spatial = (FdoSpatialCondition*)filter;
    else if (dynamic_cast<FdoBinaryLogicalOperator*>(filter))
    {
        FdoPtr<FdoFilter> left = ((FdoBinaryLogicalOperator*)filter)->GetLeftOperand();
        FdoPtr<FdoFilter> right = ((FdoBinaryLogicalOperator*)filter)->GetRightOperand();

        if (dynamic_cast<FdoSpatialCondition*>(left.p))
        {
            spatial = left;
            attr = right;
        }
        else if (dynamic_cast<FdoSpatialCondition*>(right.p))
        {
            spatial = right;
            attr = left;
        }
        else
            attr = filter;
    }
    else
        attr = filter;

    //set attribute query on OGR layer
    if (attr)
    {
        FdoString* attrsql = attr->ToString();
        std::string mbattrsql = W2A_SLOW(attrsql);
        layer->SetAttributeFilter(mbattrsql.c_str());
    }

    //set spatial query
    if (spatial)
    {
        FdoSpatialCondition* sc = (FdoSpatialCondition*)spatial;

        //Simple envelope can be handled faster, ad is used by map renders
        if (sc->GetOperation() == FdoSpatialOperations_EnvelopeIntersects)
        {
            FdoPtr<FdoExpression> expr = sc->GetGeometry();
            FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

            if (geomval)
            {
                FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoIGeometry> fgfgeom = gf->CreateGeometryFromFgf(fgf);
                FdoPtr<FdoIEnvelope> envelope = fgfgeom->GetEnvelope();

                layer->SetSpatialFilterRect(envelope->GetMinX(),
                    envelope->GetMinY(),
                    envelope->GetMaxX(),
                    envelope->GetMaxY());
            }
        }
        else if (sc->GetOperation() == FdoSpatialOperations_Intersects)
        {
            //Intersects is used for selection of features by the MapGuide AJAX viewer

            FdoPtr<FdoExpression> expr = sc->GetGeometry();
            FdoGeometryValue* geomval = dynamic_cast<FdoGeometryValue*>(expr.p);

            if (geomval)
            {
                FdoPtr<FdoByteArray> fgf = geomval->GetGeometry();

                unsigned char* wkb = (unsigned char*) alloca(fgf->GetCount());

                int len = Fgf2Wkb(fgf->GetData(), wkb);

                OGRGeometry* geom = NULL;
                OGRGeometryFactory::createFromWkb(wkb, NULL, &geom, len);

                if (geom)
                    layer->SetSpatialFilter(geom);
#if DEBUG
                else
                    printf ("failed to convert intersects spatial filter geometry value");
#endif

#if GDAL_VERSION_MAJOR < 2
                OGRFree(geom);
#else
                OGRGeometryFactory::destroyGeometry(geom);
#endif
            }
        }
    }
}

//For reference, duplicated from ogr_geometry.h
///**
// * List of well known binary geometry types.  These are used within the BLOBs
// * but are also returned from OGRGeometry::getGeometryType() to identify the
// * type of a geometry object.
// */
//
//typedef enum 
//{
//    wkbUnknown = 0,             /* non-standard */
//    wkbPoint = 1,               /* rest are standard WKB type codes */
//    wkbLineString = 2,
//    wkbPolygon = 3,
//    wkbMultiPoint = 4,
//    wkbMultiLineString = 5,
//    wkbMultiPolygon = 6,
//    wkbGeometryCollection = 7,
//    wkbNone = 100,              /* non-standard, for pure attribute records */
//    wkbLinearRing = 101,        /* non-standard, just for createGeometry() */
//    wkbPoint25D = 0x80000001,   /* 2.5D extensions as per 99-402 */
//    wkbLineString25D = 0x80000002,
//    wkbPolygon25D = 0x80000003,
//    wkbMultiPoint25D = 0x80000004,
//    wkbMultiLineString25D = 0x80000005,
//    wkbMultiPolygon25D = 0x80000006,
//    wkbGeometryCollection25D = 0x80000007
//} OGRwkbGeometryType;
//
//#define wkb25DBit 0x80000000
//#define wkbFlatten(x)  ((OGRwkbGeometryType) ((x) & (~wkb25DBit)))
//
//#define ogrZMarker 0x21125711
//
//const char CPL_DLL * OGRGeometryTypeToName( OGRwkbGeometryType eType );
//
//typedef enum 
//{
//    wkbXDR = 0,         /* MSB/Sun/Motoroloa: Most Significant Byte First   */
//    wkbNDR = 1          /* LSB/Intel/Vax: Least Significant Byte First      */
//} OGRwkbByteOrder;


int OgrFdoUtil::Wkb2Fgf(const unsigned char* wkb, unsigned char* fgf)
{
    OgrBinaryReader src(wkb);
    OgrBinaryWriter dst(fgf);

    src.ReadByte(); //skip byte order -- FGF is always NDR

    // the geometry type
    int geom_type = src.ReadInt();

    // the coordinate type
    int skip = (geom_type < 0) ? 1 : 0; //0=XY, 1=XYZ or XYM, 2 = XYZM
    geom_type = geom_type & 0x7FFFFFFF; //now convert to FGF geom type

    dst.WriteInt(geom_type); //strip out WKB 2.5D indicator (FDO doesn't use that)

    bool is_multi = (geom_type == FdoGeometryType_MultiLineString)
        || (geom_type == FdoGeometryType_MultiPolygon
        || (geom_type == FdoGeometryType_MultiPoint));

    int num_geoms = 1;

    //in case of multipolygon or multilinestring or multipoint,
    //read poly or linestring count
    if (is_multi) 
    {
        num_geoms = src.ReadInt();
        dst.WriteInt(num_geoms);
    }

    for (int q=0; q<num_geoms; q++)
    {
        if (is_multi)
        {
            //skip byte order
            src.ReadByte();

            //geom type
            geom_type = src.ReadInt();
            skip = geom_type < 0 ? 1 : 0; //is it 2.5D
            geom_type &= 0x7FFFFFFF;
            dst.WriteInt(geom_type);
        }

        //read cordinate type
        int dim = skip ? FdoDimensionality_XY | FdoDimensionality_Z : FdoDimensionality_XY;
        dst.WriteInt(dim);

        // the number of contours in current polygon/linestring
        int contour_count = 1; //for linestrings

        if ((geom_type == FdoGeometryType_Polygon)
            || (geom_type == FdoGeometryType_MultiPolygon))
        {
            contour_count = src.ReadInt();
            dst.WriteInt(contour_count);
        }

        for (int i=0; i<contour_count; i++)
        {
            int point_count = 1;

            //point geoms do not have a point count, since
            //each piece is just one point each
            if ((geom_type != FdoGeometryType_MultiPoint)
                && (geom_type != FdoGeometryType_Point))
            {
                point_count = src.ReadInt();
                dst.WriteInt(point_count);
            }

            int numd = point_count * (skip + 2);
            for (int j=0; j<numd; j++) dst.WriteDouble(src.ReadDouble());
        }
    }

    return dst.GetLength();
}


int OgrFdoUtil::Fgf2Wkb(const unsigned char* fgf, unsigned char* wkb)
{
    OgrBinaryReader src(fgf);
    OgrBinaryWriter dst((unsigned char*)wkb);

    dst.WriteByte(1); //NDR

    // the geometry type
    int geom_type = src.ReadInt();

    //we will need to do a second pass to set the 3D flag on this once we read the FDO dimensionality flag
    dst.WriteInt(geom_type); 

    //The FDO constants match WKB, so we don't really need to use the OGR wkb defines
    bool is_multi = (geom_type == FdoGeometryType_MultiLineString)
        || (geom_type == FdoGeometryType_MultiPolygon
        || (geom_type == FdoGeometryType_MultiPoint));

    int num_geoms = 1;

    //in case of multipolygon or multilinestring or multipoint,
    //read poly or linestring count
    if (is_multi) 
    {
        num_geoms = src.ReadInt();
        dst.WriteInt(num_geoms);
    }

    for (int q=0; q<num_geoms; q++)
    {
        if (is_multi)
        {
            //set byte order
            dst.WriteByte(1);

            //geom type
            geom_type = src.ReadInt();
            dst.WriteInt(geom_type);
        }

        //read cordinate type
        int dim = src.ReadInt();
        int skip = ((dim & FdoDimensionality_Z) != 0) ? 1 : 0;

        if (skip)
        {
            //roll back and set the geom_type to wkb 2.5D
            //TODO: do we need to set both the geom type for the parent multi geom
            //and the child geom? For example in the case of a multipolygon, do we need to 
            //set it for both the current polygon and the parent multipolygon?
            *(int*)(dst.m_begin+1) = geom_type | 0x80000000; 
        }
        
        // the number of contours in current polygon/linestring
        int contour_count = 1; //for linestrings

        if ((geom_type == FdoGeometryType_Polygon)
            || (geom_type == FdoGeometryType_MultiPolygon))
        {
            contour_count = src.ReadInt();
            dst.WriteInt(contour_count);
        }

        for (int i=0; i<contour_count; i++)
        {
            int point_count = 1;

            //point geoms do not have a point count, since
            //each piece is just one point each
            if ((geom_type != FdoGeometryType_MultiPoint)
                && (geom_type != FdoGeometryType_Point))
            {
                point_count = src.ReadInt();
                dst.WriteInt(point_count);
            }

            int numd = point_count * (skip + 2);
            for (int j=0; j<numd; j++) dst.WriteDouble(src.ReadDouble());
        }
    }

    return dst.GetLength();
}


