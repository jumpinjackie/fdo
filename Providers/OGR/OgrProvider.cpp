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

//=====================================================================
// This file implements a simple FDO provider for OGR
// data sources. 
//=====================================================================

#include "OgrProvider.h"

#include <ogrsf_frmts.h>

#define PROP_NAME_DATASOURCE L"DataSource"
#define PROP_NAME_READONLY   L"ReadOnly"
#define RDONLY_FALSE         L"FALSE"
#define RDONLY_TRUE          L"TRUE"

#ifndef _WIN32
#define _wcsnicmp wcsncasecmp
#endif

//TODO: usage of wcstombs/mbstowcs is incorrect in most cases
#define A2W(x) \
    size_t len##x = strlen(x)*4+4; \
    wchar_t* w##x = (wchar_t*)alloca(len##x); \
    mbstowcs(w##x, x, len##x);
    
#define W2A(x) \
    size_t len##x = wcslen(x)*4+1; \
    char* mb##x = (char*)alloca(len##x); \
    wcstombs(mb##x, x, len##x);

//FDO entry point
extern "C"
OGR_API FdoIConnection* CreateConnection()
{
    return new OgrConnection();
}

//OGR driver registration -- do it only once
class StaticInit
{
    public:
        StaticInit()
        {
            OGRRegisterAll(); 
        }
};
StaticInit si;


OgrConnection::OgrConnection()
{
    m_poDS = NULL;
    m_pSchema = NULL;
    
    m_mProps = new std::map<std::wstring, std::wstring>();
    m_connState = FdoConnectionState_Closed;
}

OgrConnection::~OgrConnection()
{
    FDO_SAFE_RELEASE(m_pSchema);
    Close();
    delete m_mProps;
}

void OgrConnection::Dispose()
{
    //only delete ourselves when we are being disposed of
    //as a connection, rather than any of the other classes
    //we inherit in the OgrConnection object
    if (((FdoIConnection*)this)->GetRefCount() == 0)
    {
        printf("destroying connection\n");
        delete this;
    }
    else
        printf("Dispose() called by multiply inherited object\n");
}

//----------------------------------------------------------------
//
//                          FdoIConnection
//
//----------------------------------------------------------------

FdoString* OgrConnection::GetConnectionString()
{
    m_connStr = L"";

    std::map<std::wstring, std::wstring>::iterator iter;
    
    for (iter = m_mProps->begin(); iter != m_mProps->end(); iter++)
    {
        m_connStr += iter->first;
        m_connStr += L"=";
        m_connStr += iter->second;
        m_connStr += L";";
    }

    return m_connStr.c_str();
}

void OgrConnection::SetConnectionString(FdoString* value)
{
    m_mProps->clear();
    
    //parse the connection properties from the connection string
    size_t len = wcslen(value);
    wchar_t* valcpy = new wchar_t[len + 1];
    wcscpy(valcpy, value);

    wchar_t* ptr = NULL; //for Linux wcstok

#ifdef _WIN32
    wchar_t* token = wcstok(valcpy, L";");
#else
    wchar_t* token = wcstok(valcpy, L";", &ptr);
#endif

    //tokenize input string into separate connection properties
    while (token) 
    {
        //token is in the form "<prop_name>=<prop_value>"
        //look for the = sign
        wchar_t* eq = wcschr(token, L'=');

        if (eq)
        {
            *eq = L'\0';
    
                //pass empty string instead of null. This means the null prop value
                //exception is delayed up to until the user attempts to open the 
                //connection. This gives the opportunity to fix the connection
                //string before opening the connection.
            if (*(eq+1) == L'\0')
                SetProperty(token, L"");
            else
                SetProperty(token, eq+1);
        }
    
    #ifdef _WIN32
            token = wcstok(NULL, L";");
    #else
            token = wcstok(NULL, L";", &ptr);
    #endif
    }

    delete [] valcpy;
}

FdoConnectionState OgrConnection::Open()
{
    const wchar_t* dsw = GetProperty(PROP_NAME_DATASOURCE);
    bool readonly = _wcsnicmp(GetProperty(PROP_NAME_READONLY), L"TRUE", 4) == 0;
    
    W2A(dsw);
    
    printf ("Attempt OGR connect to %s \n", mbdsw);
    printf ("ReadOnly %d\n", (int)readonly);
    
    m_poDS = OGRSFDriverRegistrar::Open(mbdsw, !readonly);
    if( m_poDS == NULL )
    {
        throw FdoConnectionException::Create(L"Connect failed");
    }
    
    m_connState = FdoConnectionState_Open;
    
    return m_connState;
}

void OgrConnection::Close()
{
    printf ("Close OGR connection\n");
    
    if (m_poDS)
    {
        OGRDataSource::DestroyDataSource(m_poDS);
        m_poDS = NULL;
    }
    
    m_connState = FdoConnectionState_Closed;
}

FdoICommand* OgrConnection::CreateCommand(FdoInt32 commandType)
{
    switch(commandType)
    {
        case FdoCommandType_DescribeSchema :        return new OgrDescribeSchema(this);
        case FdoCommandType_GetSpatialContexts :    return new OgrGetSpatialContexts(this);
        case FdoCommandType_Select :                return new OgrSelect(this);
        case FdoCommandType_SelectAggregates :      return new OgrSelectAggregates(this);
        default: break;
    }
    
    return NULL;
}

//-------------------------------------------------------
// FdoIConnectionPropertyDictionary implementation
//-------------------------------------------------------

FdoString** OgrConnection::GetPropertyNames(FdoInt32& count)
{
    static const wchar_t* PROP_NAMES[] = {PROP_NAME_DATASOURCE, PROP_NAME_READONLY};

    count = 2;
    return (const wchar_t**)PROP_NAMES;
}

FdoString* OgrConnection::GetProperty(FdoString* name)
{
    return (*m_mProps)[name].c_str();
}

void OgrConnection::SetProperty(FdoString* name, FdoString* value)
{
    // check the connection
    if (GetConnectionState() != FdoConnectionState_Closed)
        throw FdoConnectionException::Create(L"Attempt to set property on open connection");

    // validate input
    if (value == NULL && (wcscmp(name, PROP_NAME_DATASOURCE) == 0))
        throw FdoConnectionException::Create(L"DataSource cannot be null");

    if (wcscmp(name, PROP_NAME_DATASOURCE) == 0)
    {
        (*m_mProps)[name] = value;
    }
    else if (wcscmp(name, PROP_NAME_READONLY) == 0)
    {
        if (_wcsnicmp(RDONLY_FALSE, value, wcslen(RDONLY_FALSE)) != 0
            && _wcsnicmp(RDONLY_TRUE, value, wcslen(RDONLY_TRUE)) != 0)
            throw FdoConnectionException::Create(L"Invalid value for ReadOnly -- must be TRUE or FALSE");

        (*m_mProps)[name] = value;
    }
    else
    {
        //some other custom property the user added...
        if (value)
            (*m_mProps)[name] = value;
        else
            (*m_mProps)[name] = L"";
    }
}

FdoString* OgrConnection::GetPropertyDefault(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_DATASOURCE) == 0)
        return L"";
    else if (wcscmp(name, PROP_NAME_READONLY) == 0)
        return RDONLY_TRUE;

    return L"";
}

bool OgrConnection::IsPropertyRequired(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_DATASOURCE) == 0)
        return true;
    else if (wcscmp(name, PROP_NAME_READONLY) == 0)
        return false;

    return false;
}

bool OgrConnection::IsPropertyProtected(FdoString* name)
{
    return false;
}

bool OgrConnection::IsPropertyFileName(FdoString* name)
{
    return false;
}

bool OgrConnection::IsPropertyFilePath(FdoString* name)
{
    return false;
}

bool OgrConnection::IsPropertyDatastoreName(FdoString* name)
{
    return false;
}

bool OgrConnection::IsPropertyEnumerable(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_READONLY) == 0)
        return true;

    return false;
}

FdoString** OgrConnection::EnumeratePropertyValues(FdoString* name, FdoInt32& count)
{
    static const wchar_t* RDONLY_VALUES[] = {RDONLY_FALSE, RDONLY_TRUE};

    if (wcscmp(name, PROP_NAME_READONLY) == 0)
    {
        count = 2;
        return (const wchar_t**)RDONLY_VALUES;
    }

    count = 0;
    return NULL;
}

FdoString* OgrConnection::GetLocalizedName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_DATASOURCE) == 0)
        return L"DataSource";
    else if (wcscmp(name, PROP_NAME_READONLY) == 0)
        return L"ReadOnly";

    return NULL;
}


//--------------------------------------------------------------------------
//
//        Command helpers -- implementation of command functionality
//
//--------------------------------------------------------------------------

//reads feature schema from OGR data source and creates a 
//corresponding FDO schema
FdoFeatureSchemaCollection* OgrConnection::DescribeSchema()
{
    if (!m_pSchema)
    {
        if (m_poDS)
        {
            m_pSchema = FdoFeatureSchemaCollection::Create(NULL);
            FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"OGRSchema", L"");
            
            m_pSchema->Add(schema.p);
            
            FdoPtr<FdoClassCollection> classes = schema->GetClasses();
            
            int count = m_poDS->GetLayerCount();
            
            for (int i=0; i<count; i++)
            {
                OGRLayer* layer = m_poDS->GetLayer(i);
                FdoPtr<FdoClassDefinition> fc = ConvertClass(layer);
                classes->Add(fc);
            }
        }
    }
    
    return FDO_SAFE_ADDREF(m_pSchema);
}


FdoISpatialContextReader* OgrConnection::GetSpatialContexts()
{
    return new OgrSpatialContextReader(this);
}

FdoIFeatureReader* OgrConnection::Select(FdoIdentifier* fcname, FdoFilter* filter, FdoIdentifierCollection* props)
{
    FdoString* fc = fcname->GetName();
    W2A(fc);

    //TODO HACK remove . from feature class names -- this is a workaround for Oracle
    for (int i=strlen(mbfc)-1; i>=0; i--)
    {
        if (mbfc[i] == '~')
            mbfc[i] = '.';
    }
    
    OGRLayer* layer = m_poDS->GetLayerByName(mbfc);

    ApplyFilter(layer, filter);

    return new OgrFeatureReader(this, layer, props);
}

FdoIDataReader* OgrConnection::SelectAggregates(FdoIdentifier* fcname, 
                                 FdoIdentifierCollection* properties,
                                 bool bDistinct,
                                 FdoOrderingOption eOrderingOption,
                                 FdoIdentifierCollection* ordering,
                                 FdoFilter* filter,
                                 FdoIdentifierCollection* grouping)
{
    FdoString* fc = fcname->GetName();
    W2A(fc);
    
    if (bDistinct)
    {
        //make SQL for distict values -- OGR only supports distinct
        //for a single property
        char sql[512];
        
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        FdoString* pname = id->GetName();
        W2A(pname);
        
        sprintf(sql, "SELECT DISTINCT %s FROM %s", mbpname, mbfc);
        printf (" select distinct: %s\n", sql);
        
        OGRLayer* lr = m_poDS->ExecuteSQL(sql, NULL, NULL);
        
        return new OgrDataReader(this, lr, NULL); 
    }
    else
    {
        //select aggregate -- only one computed identifier expected!
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        FdoPtr<FdoComputedIdentifier> ci = dynamic_cast<FdoComputedIdentifier*>(id.p);
        FdoPtr<FdoExpression> expr = ci->GetExpression();
        FdoString* exprs = expr->ToString();
        W2A(exprs);
        
        char sql[512];
        
        sprintf(sql, "SELECT %s FROM %s", mbexprs, mbfc);
        printf (" select distinct: %s\n", sql);
 
        OGRLayer* lr = m_poDS->ExecuteSQL(sql, NULL, NULL);

        return new OgrDataReader(this, lr, properties); 
    }
    
    return NULL;
}


FdoClassDefinition* OgrConnection::ConvertClass(OGRLayer* layer, FdoIdentifierCollection* requestedProps)
{
    OGRFeatureDefn* fdefn = layer->GetLayerDefn();
                
    const char* name = fdefn->GetName();
    A2W(name);

    //TODO HACK remove . from feature class names - this is a workaround for Oracle.
    for (int i=wcslen(wname)-1; i>=0; i--)
    {
        if (wname[i] == L'.')
            wname[i] = L'~';
    }
                
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


//attempts to map an FDO filter to an attribute and spatial filters
//used to set up a feature query for Select commands
//This function assumes MapGuide style queries -- either 
//an attribute filter or a simple spatial filter or a binary
//combination of the two
void OgrConnection::ApplyFilter(OGRLayer* layer, FdoFilter* filter)
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


int OgrConnection::Fgf2Wkb(const unsigned char* fgf, unsigned char* wkb)
{
    return 0;
}

int OgrConnection::Wkb2Fgf(const unsigned char* wkb, unsigned char* fgf)
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


//---------------------------------------------------------------------
//
//    SpatialContextReader
//
//---------------------------------------------------------------------

OgrSpatialContextReader::OgrSpatialContextReader(OgrConnection* conn)
{
    m_nIndex = -1; //first call to ReadNext will increment it to 0
    m_connection = conn;
    ((FdoIConnection*)m_connection)->AddRef();
}

OgrSpatialContextReader::~OgrSpatialContextReader()
{
    ((FdoIConnection*)m_connection)->Release();
}

void OgrSpatialContextReader::Dispose()
{
    delete this;
}

FdoString* OgrSpatialContextReader::GetName()
{
    return L"";
}

FdoString* OgrSpatialContextReader::GetDescription()
{
    return L"";
}

FdoString* OgrSpatialContextReader::GetCoordinateSystem()
{
    return L"";
}

FdoString* OgrSpatialContextReader::GetCoordinateSystemWkt()
{
    char* wkt = NULL;
    m_connection->GetOGRDataSource()->GetLayer(m_nIndex)->GetSpatialRef()->exportToWkt(&wkt);
    
    if (wkt == NULL)
        return NULL;
    
    A2W(wkt);
    m_wkt = wwkt;
    
    OGRFree (wkt);
    return m_wkt.c_str();
}

FdoSpatialContextExtentType OgrSpatialContextReader::GetExtentType()
{
    return FdoSpatialContextExtentType_Dynamic;
}

FdoByteArray* OgrSpatialContextReader::GetExtent()
{
    OGREnvelope e;
    /*OGRErr err =*/ m_connection->GetOGRDataSource()->GetLayer(m_nIndex)->GetExtent(&e, TRUE);
                     
    //generate FGF polygon and return as refcounted byte array
    double coords[10];
    coords[0] = e.MinX;
    coords[1] = e.MinY;
    coords[2] = e.MaxX;
    coords[3] = e.MinY;
    coords[4] = e.MaxX;
    coords[5] = e.MaxY;
    coords[6] = e.MinX;
    coords[7] = e.MaxY;
    coords[8] = e.MinX;
    coords[9] = e.MinY;

    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
    FdoPtr<FdoILinearRing> lr = gf->CreateLinearRing(FdoDimensionality_XY, 10, coords);
    FdoPtr<FdoIPolygon> fgfgeom = gf->CreatePolygon(lr, NULL);

    return gf->GetFgf(fgfgeom);
}

const double OgrSpatialContextReader::GetXYTolerance()
{
    return 0.0;
}

const double OgrSpatialContextReader::GetZTolerance()
{
    //zero tolerance!!! Yeah!!!!!!
    return 0.0;
}

const bool OgrSpatialContextReader::IsActive()
{
    //TODO HACK
    if (m_nIndex == 0)
        return true;
    
    return false;
}

bool OgrSpatialContextReader::ReadNext()
{
    do
    {
        m_nIndex++;
    }    
    while (m_nIndex < m_connection->GetOGRDataSource()->GetLayerCount()
           && !m_connection->GetOGRDataSource()->GetLayer(m_nIndex)->GetSpatialRef());
    
    if (m_nIndex == m_connection->GetOGRDataSource()->GetLayerCount())
        return false;

    return true;
}

//---------------------------------------------------------------------
//
//    OgrFeatureReader
//
//---------------------------------------------------------------------

OgrFeatureReader::OgrFeatureReader(OgrConnection* connection, OGRLayer* layer, FdoIdentifierCollection* props)
{
    m_connection = connection;
    ((FdoIConnection*)m_connection)->AddRef();

    m_props = props;
    if (m_props) m_props->AddRef();

    m_poLayer = layer;
    m_poLayer->ResetReading();
    m_poFeature = NULL;
    
    m_fgflen = 64;
    m_fgf = new unsigned char[m_fgflen*2];
    m_wkb = new unsigned char[m_fgflen];
}

OgrFeatureReader::~OgrFeatureReader()
{
    Close();
    FDO_SAFE_RELEASE(m_props);
    ((FdoIConnection*)m_connection)->Release();
    delete [] m_fgf;
    delete [] m_wkb;
}

void OgrFeatureReader::Dispose()
{
    delete this;
}

FdoClassDefinition* OgrFeatureReader::GetClassDefinition()
{
    //TODO: cache the result of this
    //also this always returns all properties regardless
    //of what was given in the select command
    return m_connection->ConvertClass(m_poLayer, m_props);
}

FdoInt32 OgrFeatureReader::GetDepth()
{
    return 0;
}

bool OgrFeatureReader::GetBoolean(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Boolean");
}

FdoByte OgrFeatureReader::GetByte(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Byte");
}

FdoDateTime OgrFeatureReader::GetDateTime(FdoString* propertyName)
{
    W2A(propertyName);
    int yr = -1;
    int mt = -1;
    int dy = -1;
    int hr = -1;
    int mn = -1;
    int sc = -1;
    int tz = -1;
    
    int index = m_poFeature->GetFieldIndex(mbpropertyName);
    m_poFeature->GetFieldAsDateTime(index, &yr, &mt, &dy, &hr, &mn, &sc, &tz);
    
    return FdoDateTime(yr, mt, dy, hr, mn, (sc==-1) ? 0.0f: (float)sc);
}

double OgrFeatureReader::GetDouble(FdoString* propertyName)
{
    W2A(propertyName);
    return m_poFeature->GetFieldAsDouble(mbpropertyName);
}

FdoInt16 OgrFeatureReader::GetInt16(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Int16");
}

FdoInt32 OgrFeatureReader::GetInt32(FdoString* propertyName)
{
    W2A(propertyName);
    
    //check if we are asked for ID property
    const char* id = m_poLayer->GetFIDColumn();
    if ((*id == 0 && strcmp("FID", mbpropertyName) == 0)
         || strcmp(id, mbpropertyName) == 0)
        return m_poFeature->GetFID();
     
    return m_poFeature->GetFieldAsInteger(mbpropertyName);
}

FdoInt64 OgrFeatureReader::GetInt64(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Int64");
}

float OgrFeatureReader::GetSingle(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Float32");
}

FdoString* OgrFeatureReader::GetString(FdoString* propertyName)
{
    W2A(propertyName);
    const char* val = m_poFeature->GetFieldAsString(mbpropertyName);
    
    A2W(val);
    m_sprops[(long)val] = wval;
    return m_sprops[(long)val].c_str();
}

FdoLOBValue* OgrFeatureReader::GetLOB(FdoString* propertyName)
{
    return NULL;
}

FdoIStreamReader* OgrFeatureReader::GetLOBStreamReader(FdoString* propertyName )
{
    return NULL;
}

bool OgrFeatureReader::IsNull(FdoString* propertyName)
{
    //TODO: how do you check this in OGR
    return false;
}

FdoIFeatureReader* OgrFeatureReader::GetFeatureObject(FdoString* propertyName)
{
    return NULL;
}

FdoByteArray* OgrFeatureReader::GetGeometry(FdoString* propertyName)
{
    int len = 0;
    const void* ptr = GetGeometry(propertyName, &len);
    return FdoByteArray::Create((unsigned char*)ptr, len);
}

const FdoByte* OgrFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* len)
{
    OGRGeometry* geom = m_poFeature->GetGeometryRef();
    
    size_t wkblen = geom->WkbSize();
    
    //allocate enough to hold the geom array
    if (m_fgflen < wkblen)
    {
        delete [] m_fgf;
        delete [] m_wkb;
        m_fgflen = wkblen;
        m_fgf = new unsigned char[m_fgflen*2];
        m_wkb = new unsigned char[m_fgflen];
    }
    
    geom->exportToWkb(wkbNDR, (unsigned char*)m_wkb);
    
    *len = m_connection->Wkb2Fgf(m_wkb, m_fgf);
    return (const unsigned char*)m_fgf;
}

FdoIRaster* OgrFeatureReader::GetRaster(FdoString* propertyName)
{
    return NULL;
}

bool OgrFeatureReader::ReadNext()
{    
    m_sprops.clear();
    
    if (m_poFeature)
        OGRFeature::DestroyFeature(m_poFeature);
    
    m_poFeature = m_poLayer->GetNextFeature();
    
    return (m_poFeature != NULL);
}

void OgrFeatureReader::Close()
{
    if (m_poFeature)
    {
        OGRFeature::DestroyFeature(m_poFeature);
        m_poFeature = NULL;
    }
}

FdoDataType OgrFeatureReader::GetDataType( FdoString* propertyName )
{
    W2A(propertyName);
    
    OGRFeatureDefn* fdefn = m_poLayer->GetLayerDefn();
    
    OGRFieldDefn* field = fdefn->GetFieldDefn(fdefn->GetFieldIndex(mbpropertyName));
    
    FdoDataType dt = (FdoDataType)-1;
    OGRFieldType etype = field->GetType();
 
    switch (etype)
    {
        case OFTInteger: dt = FdoDataType_Int32;break;
        case OFTString: dt = FdoDataType_String;break;
        case OFTWideString: dt = FdoDataType_String;break;
        case OFTReal: dt = FdoDataType_Double;break;
        case OFTDate:
        case OFTTime:
        case OFTDateTime: dt = FdoDataType_DateTime; break;
        default: break; //unknown property type
    }
    
    return dt;
}


    //------------------------------------------------------------
    //
    // OgrDataReader implementation
    //
    //------------------------------------------------------------

OgrDataReader::OgrDataReader(OgrConnection* conn, OGRLayer* layer, FdoIdentifierCollection* ids)
{
    m_connection = conn;
    ((FdoIConnection*)m_connection)->AddRef();
    m_poLayer = layer;
    m_poLayer->ResetReading();
    m_poFeature = NULL;
    m_bUseNameMap = false;
    
    //if an identifier list is passes in, we are doing a computed identifier
    //like min() or max() -- we need to create a map from computed identifier name
    //to OGR aggregate property name
    if (ids)
    {
        m_bUseNameMap = true;
        
        for (int i=0; i<ids->GetCount(); i++)
        {
            FdoPtr<FdoIdentifier> id = ids->GetItem(i);
            FdoComputedIdentifier* cid = dynamic_cast<FdoComputedIdentifier*>(id.p);
            
            if (cid)
            {
                FdoString* cidname = cid->GetName(); 
                
                
                FdoPtr<FdoExpression> expr = cid->GetExpression();
                FdoFunction* func = dynamic_cast<FdoFunction*>(expr.p);
                
                if (func)
                {
                    FdoString* name = func->GetName();
                    W2A(name);
                    
                    FdoPtr<FdoExpressionCollection> args = func->GetArguments();
                    FdoPtr<FdoExpression> fexpr = args->GetItem(0);
                    
                    FdoIdentifier* prop = dynamic_cast<FdoIdentifier*>(fexpr.p);
                    
                    if (prop)
                    {
                        FdoString* propname = prop->GetName();
                        W2A(propname);
                        
                        char ogrname[512];
                        
                        sprintf(ogrname, "%s_%s", mbname, mbpropname);
                        
                        m_namemap[cidname] = ogrname;
                    }
                }
            }
        }
    }
}

void OgrDataReader::Dispose()
{
    delete this;
}

OgrDataReader::~OgrDataReader()
{
    Close();
    ((FdoIConnection*)m_connection)->Release();
}

FdoInt32 OgrDataReader::GetPropertyCount()
{
    return m_poLayer->GetLayerDefn()->GetFieldCount();
}

FdoString* OgrDataReader::GetPropertyName(FdoInt32 index)
{
    if (m_propnames[index].empty())
    {
        const char* name = m_poLayer->GetLayerDefn()->GetFieldDefn(index)->GetNameRef();
        A2W(name);
        m_propnames[index] = wname;
    }
    
    return m_propnames[index].c_str();
}

FdoDataType OgrDataReader::GetDataType(FdoString* propertyName)
{
    W2A(propertyName);
    if (m_bUseNameMap) mbpropertyName = (char*)m_namemap[propertyName].c_str();
    
    OGRFeatureDefn* fdefn = m_poLayer->GetLayerDefn();
    
    OGRFieldDefn* field = fdefn->GetFieldDefn(fdefn->GetFieldIndex(mbpropertyName));
    
    FdoDataType dt = (FdoDataType)-1;
    OGRFieldType etype = field->GetType();
 
    switch (etype)
    {
        case OFTInteger: dt = FdoDataType_Int32;break;
        case OFTString: dt = FdoDataType_String;break;
        case OFTWideString: dt = FdoDataType_String;break;
        case OFTReal: dt = FdoDataType_Double;break;
        default: break; //unknown property type
    }
    
    return dt;
}

FdoPropertyType OgrDataReader::GetPropertyType(FdoString* propertyName)
{
    return FdoPropertyType_DataProperty; //no geom support yet
}

bool OgrDataReader::GetBoolean(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Boolean");
}

FdoByte OgrDataReader::GetByte(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Byte");
}

FdoDateTime OgrDataReader::GetDateTime(FdoString* propertyName)
{
    W2A(propertyName);
    if (m_bUseNameMap) mbpropertyName = (char*)m_namemap[propertyName].c_str();
    
    int yr = -1;
    int mt = -1;
    int dy = -1;
    int hr = -1;
    int mn = -1;
    int sc = -1;
    int tz = -1;
    
    int index = m_poFeature->GetFieldIndex(mbpropertyName);
    m_poFeature->GetFieldAsDateTime(index, &yr, &mt, &dy, &hr, &mn, &sc, &tz);
    
    return FdoDateTime(yr, mt, dy, hr, mn, (sc==-1) ? 0.0f: (float)sc);
}

double OgrDataReader::GetDouble(FdoString* propertyName)
{
    W2A(propertyName);
    if (m_bUseNameMap) mbpropertyName = (char*)m_namemap[propertyName].c_str();

    return m_poFeature->GetFieldAsDouble(mbpropertyName);
}

FdoInt16 OgrDataReader::GetInt16(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Int16");
}

FdoInt32 OgrDataReader::GetInt32(FdoString* propertyName)
{
    W2A(propertyName);
    if (m_bUseNameMap) mbpropertyName = (char*)m_namemap[propertyName].c_str();

    return m_poFeature->GetFieldAsInteger(mbpropertyName);
}

FdoInt64 OgrDataReader::GetInt64(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Int64");
}

float OgrDataReader::GetSingle(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"Float32");
}

FdoString* OgrDataReader::GetString(FdoString* propertyName)
{
    W2A(propertyName);
    if (m_bUseNameMap) mbpropertyName = (char*)m_namemap[propertyName].c_str();

    const char* val = m_poFeature->GetFieldAsString(mbpropertyName);
    
    A2W(val);
    m_sprops[(long)val] = wval;
    return m_sprops[(long)val].c_str();
}

FdoLOBValue* OgrDataReader::GetLOB(FdoString* propertyName)
{
    return NULL;
}

FdoIStreamReader* OgrDataReader::GetLOBStreamReader(FdoString* propertyName )
{
    return NULL;
}

bool OgrDataReader::IsNull(FdoString* propertyName)
{
    //TODO: how do you check this in OGR
    return false;
}

FdoByteArray* OgrDataReader::GetGeometry(FdoString* propertyName)
{
    throw FdoCommandException::Create(L"geom not supported in select aggregates");
}

FdoIRaster* OgrDataReader::GetRaster(FdoString* propertyName)
{
    return NULL;
}

bool OgrDataReader::ReadNext()
{
    m_sprops.clear();
    
    if (m_poFeature)
        OGRFeature::DestroyFeature(m_poFeature);
    
    m_poFeature = m_poLayer->GetNextFeature();
    
    return (m_poFeature != NULL);
}

void OgrDataReader::Close()
{
    if (m_poFeature)
    {
        OGRFeature::DestroyFeature(m_poFeature);
        m_poFeature = NULL;
    }
    
    if (m_poLayer)
    {
        m_connection->GetOGRDataSource()->ReleaseResultSet(m_poLayer);
        m_poLayer = NULL;
    }
}


//---------------------------------------------------------------------
//
//    Test
//
//---------------------------------------------------------------------

////HACK This function has a lot of refcount leaks in order
////to make the code more concise !!!
//// DO NOT use as example code, this is just for testing.
//int main(void)
//{
//   FdoPtr<FdoIConnection> c = CreateConnection();
//   //printf("rc %d\n", c->GetRefCount());
//   //printf("connection %p\n", c.p);
//   //printf("conn tid %s\n", typeid(c.p).name());
//   FdoPtr<FdoIConnectionInfo> info = c->GetConnectionInfo();
//   //printf("rc %d\n", c->GetRefCount());
//   //printf("conninfo %p\n", info.p);
//   FdoPtr<FdoIConnectionPropertyDictionary> pd = info->GetConnectionProperties();
//   //printf("rc %d\n", c->GetRefCount());
//   
//   pd->SetProperty(L"DataSource", L"coast_n83.shp");
//   pd->SetProperty(L"ReadOnly", L"TRUE");
//   
//   c->Open();
//   
//   FdoPtr<FdoIDescribeSchema> ds = (FdoIDescribeSchema*) c->CreateCommand(FdoCommandType_DescribeSchema);
//   
//   ds->Execute();
//   
//   FdoPtr<FdoIGetSpatialContexts> gsc = (FdoIGetSpatialContexts*)c->CreateCommand(FdoCommandType_GetSpatialContexts);
//   
//   FdoPtr<FdoISpatialContextReader> scrdr = gsc->Execute();
//   
//   scrdr->ReadNext();
//   
//   printf ("coord sys %ls\n", scrdr->GetCoordinateSystemWkt());
//   
//   
//   FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
//   
//   
//   FdoPtr<FdoISelect> select = (FdoISelect*)c->CreateCommand(FdoCommandType_Select);
//   select->SetFeatureClassName(L"coast_n83");
//   //select->SetFilter(FdoFilter::Parse(L"(LAST_SALE > 500000) AND (SQFT > 1000)"));
//   FdoPtr<FdoIFeatureReader> rdr = select->Execute();
//   
//   clock_t t0 = clock();
//   
//   int count = 0;
//   while (rdr->ReadNext())
//   {
//       int fgflen = 0;
//       const unsigned char* geom = rdr->GetGeometry(L"GEOMETRY", &fgflen);
//       
//       //printf ("fid %d len %d\n", rdr->GetInt32(L"FID"), fgflen);
//       
//       try
//       {
//       FdoPtr<FdoIGeometry> fdogeom = gf->CreateGeometryFromFgf(geom, fgflen);
//       
//       //FdoString* wkt = fdogeom->GetText();
//       //printf ("%p\n", fdogeom.p);
//       //printf("geom %ls\n", wkt);
//       }
//       catch (FdoException* e)
//       {
//           printf ("***********************************************exception: %ls\n", e->GetExceptionMessage());
//           e->Release();
//           exit(1);
//       }
//       
//       
//       count++; 
//   }
//   
//   clock_t t1 = clock();
//   
//   printf ("read time %lf\n", (double)(t1 - t0)/CLOCKS_PER_SEC);
//   
//   printf ("feat count %d\n", count);
//   
//   rdr->Close();
//   
//   
//   /*
//   FdoPtr<FdoISelectAggregates> sa = (FdoISelectAggregates*) c->CreateCommand(FdoCommandType_SelectAggregates);
//   
//   FdoPtr<FdoIdentifierCollection> props = sa->GetPropertyNames();
//   props->Add(FdoIdentifier::Create(L"RTYPE"));
//    
//   sa->SetDistinct(true);
//   sa->SetFeatureClassName(L"Parcels");
//   
//   FdoPtr<FdoIDataReader> drdr = sa->Execute();
//   
//   while (drdr->ReadNext())
//   {
//       FdoString* val = drdr->GetString(L"RTYPE");
//       printf ("distinct val: %ls\n", val);
//   } 
//   
//   drdr->Close();
//   */
//   /*
//   FdoPtr<FdoISelectAggregates> sa = (FdoISelectAggregates*) c->CreateCommand(FdoCommandType_SelectAggregates);
//   
//   FdoPtr<FdoIdentifierCollection> props = sa->GetPropertyNames();
//   props->Add(FdoComputedIdentifier::Create(L"mymax", FdoExpression::Parse(L"count(\"LAST_SALE\")")));
//    
//   sa->SetFeatureClassName(L"Parcels");
//   
//   FdoPtr<FdoIDataReader> drdr = sa->Execute();
//   
//   while (drdr->ReadNext())
//   {
//       double val = drdr->GetDouble(L"mymax");
//       printf ("maxval val: %lf\n", val);
//   } 
//   
//   drdr->Close();
//   */
//  
//   c->Close();
//}
