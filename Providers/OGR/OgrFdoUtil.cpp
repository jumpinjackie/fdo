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


void tilde2dot(char* mbfc)
{
    //TODO HACK remove . from feature class names -- this is a workaround for Oracle
    for (int i=(int)strlen(mbfc)-1; i>=0; i--)
    {
        if (mbfc[i] == '~')
            mbfc[i] = '.';
    }
}

void dot2tilde(wchar_t* wname)
{
    //TODO HACK remove . from feature class names -- this is a workaround for Oracle
    for (int i=(int)wcslen(wname)-1; i>=0; i--)
    {
        if (wname[i] == '.')
            wname[i] = '~';
    }
}

FdoClassDefinition* OgrFdoUtil::ConvertClass(OGRLayer* layer, FdoIdentifierCollection* requestedProps)
{
    OGRFeatureDefn* fdefn = layer->GetLayerDefn();
                
    const char* name = fdefn->GetName();
    A2W(name);

    dot2tilde(wname);
                
    printf ("Feature class name: %s\n", name);
                
    FdoPtr<FdoFeatureClass> fc = FdoFeatureClass::Create(wname, L"");
                
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
                                
                //data properties (attributes)
    int propcount = fdefn->GetFieldCount();
                
    for (int j=0; j<propcount; j++)
    {
        OGRFieldDefn* field = fdefn->GetFieldDefn(j);
        const char* name = field->GetNameRef();
        A2W(name);
                    
        printf("Attribute : %s\n", name);
                            
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
            
            default: add=false; break; //unknown property type
        }
                    
        if (add)
        {
            //check if property is on the optional requested property list
            FdoPtr<FdoIdentifier> found = (requestedProps) ? requestedProps->FindItem(wname) : NULL;

            //if it's on the list or there was no list at all, then add the property
            if (!requestedProps || requestedProps->GetCount() == 0 || (requestedProps && found.p))
            {
                FdoPtr<FdoDataPropertyDefinition> dpd = FdoDataPropertyDefinition::Create(wname, L"");
                            
                dpd->SetDataType(dt);
                dpd->SetLength(field->GetWidth());
                dpd->SetPrecision(field->GetPrecision());
                //TODO: default value
                pdc->Add(dpd);
            }
        }
    }

    //add geometry property -- this code assumes there is one
    const char* geomname = layer->GetGeometryColumn();
    if (*geomname == 0) geomname = "GEOMETRY";
    A2W(geomname);

    //check if property is on the optional requested property list
    FdoPtr<FdoIdentifier> found = (requestedProps) ? requestedProps->FindItem(wgeomname) : NULL;

    //if it's on the list or there was no list at all, then add the property
    if (!requestedProps || requestedProps->GetCount() == 0 || (requestedProps && found.p))
    {
        printf ("Geometry column : %s\n", geomname);
    
        FdoPtr<FdoGeometricPropertyDefinition> gpd = FdoGeometricPropertyDefinition::Create(wgeomname, L"");
                
        OGRwkbGeometryType gt = fdefn->GetGeomType();
                
        switch (gt)
        {
            case wkbNone: gpd = NULL;
            case wkbPolygon:
                case wkbMultiPolygon: gpd->SetGeometryTypes(FdoGeometricType_Surface); break;
            case wkbPoint:
                case wkbMultiPoint: gpd->SetGeometryTypes(FdoGeometricType_Point); break;
            case wkbLineString:
                case wkbMultiLineString: gpd->SetGeometryTypes(FdoGeometricType_Curve); break;
                default: gpd->SetGeometryTypes(7); break;
        }
                
        if (gpd.p)
        {
            pdc->Add(gpd);
            fc->SetGeometryProperty(gpd);
        }
                
    }
                
    //identity property
    const char* idname = layer->GetFIDColumn();
    if (*idname == 0) idname = "FID";
    A2W(idname);
        
    printf ("Identity column : %s\n", idname);
    
    //check if property is on the optional requested property list
    found = (requestedProps) ? requestedProps->FindItem(widname) : NULL;

    //if it's on the list or there was no list at all, then add the property
    if (!requestedProps || requestedProps->GetCount() == 0 || (requestedProps && found.p))
    {
        //see if FID column was in the attributes we processed earlier
        FdoPtr<FdoDataPropertyDefinition> fid = (FdoDataPropertyDefinition*)pdc->FindItem(widname);
        
        //if not, create one. 
        //TODO: we may not need to do this at all
        if (!fid.p)
        {
            fid = FdoDataPropertyDefinition::Create(widname, L"");
            fid->SetDataType(FdoDataType_Int32); //TODO should we use Int64?
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
void OgrFdoUtil::ConvertFeature(FdoPropertyValueCollection* src, OGRFeature* dst, OGRLayer* layer)
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
        W2A(propName);
            
        const char* geomname = layer->GetGeometryColumn();
        if (*geomname == 0) geomname = "GEOMETRY";
        
        FdoPtr<FdoValueExpression> value = pv->GetValue();
            
        if (strcmp(geomname, mbpropName) == 0)
        {
                //is it the geometry property?

                //TODO
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
                        if (iv) dst->SetField(mbpropName, (int)iv->GetInt32());
                    }
                    break;
                case OFTString:
                    {
                        FdoStringValue* sv = dynamic_cast<FdoStringValue*>(value.p);
                        if (sv)
                        {
                            FdoString* str = sv->GetString();
                            W2A(str);
                            dst->SetField(mbpropName, (const char*)mbstr);
                        }
                    }
                    break;
                case OFTWideString: 
                    {
                        FdoStringValue* sv = dynamic_cast<FdoStringValue*>(value.p);
                        if (sv)
                        {
                            FdoString* str = sv->GetString();
                            //dst->SetField(mbpropName, str); //unsupported so far
                        }
                    }
                    break;
                case OFTReal:
                    {
                        FdoDoubleValue* iv = dynamic_cast<FdoDoubleValue*>(value.p);
                        if (iv) dst->SetField(mbpropName, (double)iv->GetDouble());
                    }
                    break;
                case OFTDate:
                case OFTTime:
                case OFTDateTime:
                    {
                        FdoDateTimeValue* dv = dynamic_cast<FdoDateTimeValue*>(value.p);
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
        W2A(attrsql);
        layer->SetAttributeFilter(mbattrsql);
    }

    //set spatial query -- assumes EnvelopeIntersects
    if (spatial)
    {
        FdoSpatialCondition* sc = (FdoSpatialCondition*)spatial;
        
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
            //TODO: must implement this for selection to work in the AJAX viewer.
        }
    }
}


int OgrFdoUtil::Fgf2Wkb(const unsigned char* fgf, unsigned char* wkb)
{
    return 0;
}

int OgrFdoUtil::Wkb2Fgf(const unsigned char* wkb, unsigned char* fgf)
{
    //TODO: This routine assumes LITTLE ENDIAN (INTEL) byte order
    
    int fgflen = 0;
    unsigned char * src = (unsigned char*)wkb;
    int * dst = (int*)fgf;
    
    src++; //skip byte order -- FGF is always NDR
    int* ireader = (int*)src;
    
    // the geometry type
    int geom_type = (OGRwkbGeometryType)*ireader++;

    // the coordinate type
    int skip = (geom_type < 0) ? 1 : 0; //0=XY, 1=XYZ or XYM, 2 = XYZM
    geom_type = geom_type & 0x7FFFFFFF; //now convert to FGF geom type

    *dst++ = geom_type; //strip out WKB 2.5D indicator (FDO doesn't use that)
    fgflen += 4;

    bool is_multi = (geom_type == wkbMultiLineString)
            || (geom_type == wkbMultiPolygon
            || (geom_type == wkbMultiPoint));
    
    int num_geoms = 1;

    //in case of multipolygon or multilinestring or multipoint,
    //read poly or linestring count
    if (is_multi) 
    {
        num_geoms = *ireader++;
        *dst++ = num_geoms;
        fgflen += 4;
    }
    
    for (int q=0; q<num_geoms; q++)
    {
        if (is_multi)
        {
            //skip byte order
            ireader = (int*)(((unsigned char*)ireader) + 1);
            
            //geom type
            geom_type = *ireader++;
            skip = geom_type < 0 ? 1 : 0; //is it 2.5D
            geom_type &= 0x7FFFFFFF;
            *dst++ = geom_type;
            fgflen += 4;
        }

        //read cordinate type
        int dim = skip ? FdoDimensionality_XY | FdoDimensionality_Z : FdoDimensionality_XY;
        *dst++ = dim;
        fgflen += 4;

        // the number of contours in current polygon/linestring
        int contour_count = 1; //for linestrings

        if ((geom_type == wkbPolygon)
             || (geom_type == wkbMultiPolygon))
        {
            contour_count = *ireader++;
            *dst++ = contour_count;
            fgflen += 4;
        }

        for (int i=0; i<contour_count; i++)
        {
            int point_count = 1;

            //point geoms do not have a point count, since
            //each piece is just one point each
            if ((geom_type != wkbMultiPoint)
                 && (geom_type != wkbPoint))
            {
                point_count = *ireader++;
                *dst++ = point_count;
                fgflen += 4;
            }
            
            //*** ireader not valid from here down
            double* dreader = (double*) ireader;

            memcpy(dst, dreader, point_count * (skip + 2) * sizeof(double));
            int numd = point_count * (skip + 2);
            dreader += numd;
            dst += 2 * numd;
            fgflen += 8 * numd;
                        
            ireader = (int*)dreader;
            //*** ireader valid again
        }
    }
    
    return fgflen;
}
