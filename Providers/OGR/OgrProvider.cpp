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

#include "stdafx.h"
#include "OgrProvider.h"
#include "OgrFdoUtil.h"
#include "ProjConverter.h"
#include "FdoSpatial.h"


#define PROP_NAME_DATASOURCE L"DataSource"
#define PROP_NAME_READONLY   L"ReadOnly"
#define RDONLY_FALSE         L"FALSE"
#define RDONLY_TRUE          L"TRUE"


extern void tilde2dot(char* mbfc);
extern void dot2tilde(wchar_t* wname);


static void avoidwarnings()
{
    //those two are here just to suppress warnings from showing up
    //for unused variables.
    (void)FDO_ACTIVELONGTRANSACTION;
    (void)FDO_ROOTLONGTRANSACTION;
    avoidwarnings();
}


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
            ProjConverter::ProjectionConverter = new ProjConverter();
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
    
    size_t slen = wcslen(dsw);
    if (dsw[slen - 1] == '\\')
        slen--;

    wchar_t* tmp = new wchar_t[slen + 1];
    wcsncpy(tmp, dsw, slen);
    tmp[slen] = 0;

    W2A(tmp);

    delete tmp;
    
#if DEBUG
    printf ("Attempt OGR connect to %s \n", mbtmp);
    printf ("ReadOnly %d\n", (int)readonly);
#endif
    
    m_poDS = OGRSFDriverRegistrar::Open(mbtmp, !readonly);
    if( m_poDS == NULL )
    {
        std::string str = "Connect failed: "; 
        str.append(CPLGetLastErrorMsg());
        const char* x = str.c_str();
        A2W(x);
        throw FdoConnectionException::Create(wx);
    }
    m_connState = FdoConnectionState_Open;
    
    return m_connState;
}

void OgrConnection::Close()
{
#if DEBUG
    printf ("Close OGR connection\n");
#endif

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
        case FdoCommandType_Update:                 return new OgrUpdate(this);
        case FdoCommandType_Delete:                 return new OgrDelete(this);
        case FdoCommandType_Insert:                 return new OgrInsert(this);
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
    if (wcscmp(name, PROP_NAME_DATASOURCE) == 0)
        return true;

    return false;
}

bool OgrConnection::IsPropertyFilePath(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_DATASOURCE) == 0)
        return true;

    return false;
}

bool OgrConnection::IsPropertyDatastoreName(FdoString* name)
{
    return false;
}

bool OgrConnection::IsPropertyEnumerable(FdoString* name)
{
    //if (wcscmp(name, PROP_NAME_READONLY) == 0)
    //    return true;

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
                FdoPtr<FdoClassDefinition> fc = OgrFdoUtil::ConvertClass(layer);
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
    bool bbox = false;
    FdoString* fc = fcname->GetName();
    W2A(fc);

    tilde2dot(mbfc);
    
    OGRLayer* layer = m_poDS->GetLayerByName(mbfc);

    OgrFdoUtil::ApplyFilter(layer, filter);

    return new OgrFeatureReader(this, layer, props, filter);
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
#if DEBUG
        printf (" select distinct: %s\n", sql);
#endif
        OGRLayer* lr = m_poDS->ExecuteSQL(sql, NULL, NULL);
        
        return new OgrDataReader(this, lr, NULL); 
    }
    else
    {
        //TODO: We need to add support for SpatialExtents() and Count()
        //in order to speed up initial connection to data in Map3D.
        //For now, this check will reject such requests and throw Map
        //into the slow way of getting the information -- a full table scan
        if (properties->GetCount() > 1)
            throw FdoCommandException::Create(L"Unsupported aggregate operation.");

        //select aggregate -- only one computed identifier expected!
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        FdoComputedIdentifier* ci = dynamic_cast<FdoComputedIdentifier*>(id.p);
        FdoPtr<FdoExpression> expr = ci->GetExpression();

        //TODO: We need to add support for SpatialExtents() and Count()
        //in order to speed up initial connection to data in Map3D.
        //For now, this check will reject such requests and throw Map
        //into the slow way of getting the information -- a full table scan
        FdoFunction* func = dynamic_cast<FdoFunction*>(expr.p);
        if (func && (_wcsicmp(func->GetName(),FDO_FUNCTION_SPATIALEXTENTS) == 0))
            throw FdoCommandException::Create(L"Unsupported aggregate operation.");

        FdoString* exprs = expr->ToString();
        W2A(exprs);
        
        char sql[512];
        
        sprintf(sql, "SELECT %s FROM %s", mbexprs, mbfc);
#if DEBUG
        printf (" select distinct: %s\n", sql);
#endif
        OGRLayer* lr = m_poDS->ExecuteSQL(sql, NULL, NULL);

        return new OgrDataReader(this, lr, properties); 
    }
    
    return NULL;
}


FdoInt32 OgrConnection::Update(FdoIdentifier* fcname, FdoFilter* filter, FdoPropertyValueCollection* propvals)
{
    FdoString* fc = fcname->GetName();
    W2A(fc);

    tilde2dot(mbfc);
    
    OGRLayer* layer = m_poDS->GetLayerByName(mbfc);
    
        //check if we can delete
    int canDo = layer->TestCapability(OLCRandomWrite);
    
    if (!canDo)
        throw FdoCommandException::Create(L"Current OGR connection does not support update of existing features.");
    
    OgrFdoUtil::ApplyFilter(layer, filter);

    OGRFeature* feature = NULL;
    
    while (feature = layer->GetNextFeature())
    {
        //update the feature properties
        //this call is not fast, so if we need
        //fast update for multiple features it should be optimized
        OgrFdoUtil::ConvertFeature(propvals, feature, layer);
        
        layer->SetFeature(feature);
        
        OGRFeature::DestroyFeature(feature);
    }

    return 0;
}

FdoInt32 OgrConnection::Delete(FdoIdentifier* fcname, FdoFilter* filter)
{
    FdoString* fc = fcname->GetName();
    W2A(fc);

    tilde2dot(mbfc);
    
    OGRLayer* layer = m_poDS->GetLayerByName(mbfc);
    
        //check if we can delete
    int canDo = layer->TestCapability(OLCDeleteFeature);
    
    if (!canDo)
        throw FdoCommandException::Create(L"Current OGR connection does not support delete.");

    OgrFdoUtil::ApplyFilter(layer, filter);

    std::vector<long> ids; //list of FIDs of features to delete
    
    OGRFeature* feature = NULL;
    
    while (feature = layer->GetNextFeature())
    {
        ids.push_back(feature->GetFID());
        OGRFeature::DestroyFeature(feature);
    }
    
    int count = 0;
    
    for (std::vector<long>::iterator iter = ids.begin(); iter != ids.end(); iter++)
    {
        if (layer->DeleteFeature(*iter) == OGRERR_NONE)
            count++;
    }

    return count;
}

FdoIFeatureReader* OgrConnection::Insert(FdoIdentifier* fcname, FdoPropertyValueCollection* propvals)
{
    FdoString* fc = fcname->GetName();
    W2A(fc);

    tilde2dot(mbfc);
    
    OGRLayer* layer = m_poDS->GetLayerByName(mbfc);
    
    //check if we can insert
    int canDo = layer->TestCapability(OLCSequentialWrite);
    
    if (!canDo)
        throw FdoCommandException::Create(L"Current OGR connection does not support insert.");

    //create the new feature
    OGRFeature* feature = new OGRFeature(layer->GetLayerDefn());
    long fid = OGRNullFID;
    feature->SetFID(fid);

    //set all the properties
    OgrFdoUtil::ConvertFeature(propvals, feature, layer);

    if (layer->CreateFeature(feature) == OGRERR_NONE)
    {
        //new FID should be now correctly set
        fid = feature->GetFID();        
    }

    OGRFeature::DestroyFeature(feature);

    if (fid != OGRNullFID)
    {
        char filter[32];
        snprintf(filter, 32, "FID=%d", fid);
        layer->SetAttributeFilter(filter);
        return new OgrFeatureReader(this, layer, NULL, false);
    }

    throw FdoCommandException::Create(L"Insert of feature failed.");
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
    const char* name = m_connection->GetOGRDataSource()->GetLayer(m_nIndex)->GetLayerDefn()->GetName();
    A2W(name);

    m_name = wname;
    return m_name.c_str();
}

FdoString* OgrSpatialContextReader::GetDescription()
{
    return L"";
}

FdoString* OgrSpatialContextReader::GetCoordinateSystem()
{
    return GetCoordinateSystemWkt();
}

FdoString* OgrSpatialContextReader::GetCoordinateSystemWkt()
{
    static const wchar_t* ArbitraryWkt_Meter = L"LOCAL_CS [ \"Non-Earth (Meter)\", LOCAL_DATUM [\"Local Datum\", 0], UNIT [\"Meter\", 1.0], AXIS [\"X\", EAST], AXIS[\"Y\", NORTH]]";

    char* wkt = NULL;
    
    m_connection->GetOGRDataSource()->GetLayer(m_nIndex)->GetSpatialRef()->exportToWkt(&wkt);
    
    if (wkt == NULL)
        return ArbitraryWkt_Meter;
    
    A2W(wkt);
    m_wkt = wwkt;
    
    OGRFree (wkt);
    return ProjConverter::ProjectionConverter->TranslateProjection(m_wkt.c_str());
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

OgrFeatureReader::OgrFeatureReader(OgrConnection* connection, OGRLayer* layer, FdoIdentifierCollection* props, FdoFilter* filter)
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
    
    FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();

    m_geomFilter = NULL;
    if (dynamic_cast<FdoSpatialCondition*>(filter))
    {
        FdoSpatialCondition* sc = (FdoSpatialCondition*)filter;
        m_spatialOperation = sc->GetOperation();
        if (m_spatialOperation != FdoSpatialOperations_EnvelopeIntersects)
        {
             FdoPtr<FdoExpression> geomExpr = sc->GetGeometry();
             m_geomFilter = gf->CreateGeometryFromFgf(((FdoGeometryValue*)(geomExpr.p))->GetGeometry());
        }
    }
}

OgrFeatureReader::~OgrFeatureReader()
{
    Close();
    FDO_SAFE_RELEASE(m_props);
    ((FdoIConnection*)m_connection)->Release();
    delete [] m_fgf;
    delete [] m_wkb;
    
    FDO_SAFE_RELEASE(m_geomFilter);
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
    return OgrFdoUtil::ConvertClass(m_poLayer, m_props);
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

const FdoByte* OgrFeatureReader::GetGeometry(OGRGeometry* geom, FdoInt32* len)
{
    if (geom)
    {    
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
        
        *len = OgrFdoUtil::Wkb2Fgf(m_wkb, m_fgf);
        return (const unsigned char*)m_fgf;
    }

    throw FdoException::Create(L"Geometry is null.");
}

const FdoByte* OgrFeatureReader::GetGeometry(FdoString* propertyName, FdoInt32* len)
{
    return this->GetGeometry(m_poFeature->GetGeometryRef(), len);
}

FdoIRaster* OgrFeatureReader::GetRaster(FdoString* propertyName)
{
    return NULL;
}

bool OgrFeatureReader::ReadNext()
{    
    try
    {
        m_sprops.clear();
        
        if (m_poFeature)
            OGRFeature::DestroyFeature(m_poFeature);
        
        m_poFeature = m_poLayer->GetNextFeature();

        //OGR uses envelope intersection testing only, this breaks tooltips and selection
        //If the actual selection was not for envelope intersection, the geometry filtering is done here instead
        if (m_geomFilter != NULL)
            while (m_poFeature != NULL && m_poFeature->GetGeometryRef() != NULL)
            {
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoInt32 fgfLen;
                const FdoByte* fgf = this->GetGeometry(m_poFeature->GetGeometryRef(), &fgfLen);
                FdoPtr<FdoIGeometry> featureGeom = gf->CreateGeometryFromFgf(fgf, fgfLen);

                //call on the geometry utility to evaluate the spatial operation
                if (FdoSpatialUtility::Evaluate(m_geomFilter, m_spatialOperation, featureGeom))
                    break;
                else
                {
                    //Goto next
                    OGRFeature::DestroyFeature(m_poFeature);
                    m_poFeature = m_poLayer->GetNextFeature();
                }
            }

        return (m_poFeature != NULL);
    }
    catch(...)
    {
        return false;
    }
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
    
    //if an identifier list is passed in, we are doing a computed identifier
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

int main(void)
{
    OGRDataSource* ds = OGRSFDriverRegistrar::Open("C:\\Documents and Settings\\Kenneth\\Skrivebord\\bo", TRUE);
    /*FdoIConnection* con = CreateConnection();
    con->SetConnectionString(new FdoString(_L"
    con->Open(*/
    return 0;
}

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
