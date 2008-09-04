// 
//  
//  Copyright (C) 2008 Autodesk Inc.
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
#include "slt.h"
#include "SltProvider.h"
#include "SltCapabilities.h"
#include "SltCommands.h"
#include "SltSpatialContextReader.h"
#include "SltReader.h"
#include "SltConversionUtils.h"
#include "SltMetadata.h"
#include "SltExprExtensions.h"
#include "SltGeomUtils.h"
#include "SltQueryTranslator.h"

//#include "SpatialIndex.h"
#include "DiskSpatialIndex.h"

using namespace std;

//FDO entry point
extern "C"
SLT_API FdoIConnection* CreateConnection()
{
    return new SltConnection();
}


static bool IsMetadataTable(const char* table)
{
    if (sqlite3StrICmp(table, "sqlite_master") == 0)
        return true;

    if (sqlite3StrICmp(table, "geometry_columns") == 0)
        return true;

    if (sqlite3StrICmp(table, "spatial_ref_sys") == 0)
        return true;

    return false;
}

std::map<int, string> g_fdo2sql_map;


SltConnection::SltConnection() : m_refCount(1)
{
    if (g_fdo2sql_map.empty())
    {
        g_fdo2sql_map[FdoDataType_BLOB] =       "BLOB";
        g_fdo2sql_map[FdoDataType_CLOB] =       "TEXT";
        g_fdo2sql_map[FdoDataType_Boolean] =    "INTEGER";
        g_fdo2sql_map[FdoDataType_Byte] =       "INTEGER";
        g_fdo2sql_map[FdoDataType_DateTime] =   "TEXT";
        g_fdo2sql_map[FdoDataType_Decimal] =    "INTEGER";
        g_fdo2sql_map[FdoDataType_Double] =     "REAL";
        g_fdo2sql_map[FdoDataType_Int16] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Int32] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Int64] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Single] =     "REAL";
        g_fdo2sql_map[FdoDataType_String] =     "TEXT";
    }

    m_db = NULL;
    m_pSchema = NULL;
    m_mProps = new std::map<std::wstring, std::wstring>();
    m_connState = FdoConnectionState_Closed;

    m_caps = new SltCapabilities();
}

SltConnection::~SltConnection()
{
    Close();
    delete m_mProps;
    delete m_caps;
}

//----------------------------------------------------------------
//
//                          FdoIConnection
//
//----------------------------------------------------------------

FdoString* SltConnection::GetConnectionString()
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

void SltConnection::SetConnectionString(FdoString* value)
{
    m_mProps->clear();
    
    //parse the connection properties from the connection string
    size_t len = wcslen(value);
    wchar_t* valcpy = (wchar_t*)alloca(sizeof(wchar_t) * (len + 1));
    wcscpy(valcpy, value);

#ifdef _WIN32
    wchar_t* token = wcstok(valcpy, L";");
#else
    wchar_t* ptr = NULL; //for Linux wcstok
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
}

//creates a new database, based on the filename stored in the
//connection string and connection property dictionary.
void SltConnection::CreateDatabase()
{
    if (m_db || m_connState != FdoConnectionState_Closed)
    {
        throw FdoCommandException::Create(L"Cannot create data store while connection is in open state.");
    }

    const wchar_t* dsw = GetProperty(PROP_NAME_FILENAME);
    
    string file = W2A_SLOW(dsw);
    
    //create the database
    //TODO: this will also work if the database exists -- in which 
    //case it will open it.
    if( sqlite3_open(file.c_str(), &m_db) != SQLITE_OK )
    {
        m_db = NULL;
        std::wstring err = wstring(L"Failed to open or create: ") + dsw;
        throw FdoCommandException::Create(err.c_str());
    }

    //first things first -- set big page size for better
    //performance. Must be done on an empty db to have effect.
    int rc = sqlite3_exec(m_db, "PRAGMA page_size=32768;", NULL, NULL, NULL);
    
    //create the spatial_ref_sys table
    string srs_sql = "CREATE TABLE spatial_ref_sys\
        (srid INTEGER UNIQUE,\
        auth_name TEXT,\
        auth_srid INTEGER,\
        srtext TEXT);";

    char* zerr = NULL;
    rc = sqlite3_exec(m_db, srs_sql.c_str(), NULL, NULL, &zerr);

    if (rc)
    {
        printf ("SQL statement failed to execute.\n");
        printf ("%s\n", zerr);
    }

    //create the geometry_columns table
    string gc_sql = "CREATE TABLE geometry_columns \
                    (f_table_name TEXT,\
                    f_geometry_column TEXT,\
                    geometry_type INTEGER,\
                    coord_dimension INTEGER,\
                    srid INTEGER,\
                    geometry_format TEXT);";  

    int rc2 = sqlite3_exec(m_db, gc_sql.c_str(), NULL, NULL, &zerr);

    if (rc2)
    {
        printf ("SQL statement failed to execute.\n");
        printf ("%s\n", zerr);
    }

    //close the database -- CreateDataStore itself does not 
    //open the FDO connection, subsequent call to Open() does.
    sqlite3_close(m_db);
    m_db = NULL;

    if (rc || rc2)
    {
        throw FdoCommandException::Create(L"Failed to create SQLite database.");
    }
}

FdoConnectionState SltConnection::Open()
{
    const wchar_t* dsw = GetProperty(PROP_NAME_FILENAME);
    
    string file = W2A_SLOW(dsw);
    
    if( sqlite3_open(file.c_str(), &m_db) != SQLITE_OK )
    {
        m_db = NULL;
        std::wstring err = wstring(L"Failed to open ") + dsw;
        throw FdoConnectionException::Create(err.c_str());
    }

    //Register the extra SQL functions we would like to support
    RegisterExtensions(m_db);

    m_connState = FdoConnectionState_Open;
    return m_connState;
}

void SltConnection::Close()
{
    //free the spatial indexes
    for (std::map<std::string, SpatialIndex*>::iterator iter = m_mNameToSpatialIndex.begin();
         iter != m_mNameToSpatialIndex.end(); iter++)
         delete iter->second;

    m_mNameToSpatialIndex.clear();

    //clear the cached schema metadata
    for (std::map<std::string, SltMetadata*>::iterator iter = m_mNameToMetadata.begin();
         iter != m_mNameToMetadata.end(); iter++)
         delete iter->second;

    m_mNameToMetadata.clear();

    ClearQueryCache();

    if (m_db)
    {
        int rc = sqlite3_close(m_db);
        m_db = NULL;
    }
    
    FDO_SAFE_RELEASE(m_pSchema);

    m_connState = FdoConnectionState_Closed;
}

FdoICommand* SltConnection::CreateCommand(FdoInt32 commandType)
{
    switch(commandType)
    {
        case FdoCommandType_DescribeSchema :        return new SltDescribeSchema(this);
        case FdoCommandType_GetSpatialContexts :    return new SltGetSpatialContexts(this);
        case FdoCommandType_Select :                return new SltSelect(this);
        case FdoCommandType_SelectAggregates :      return new SltSelectAggregates(this);
        case FdoCommandType_Update:                 return new SltUpdate(this);
        case FdoCommandType_Delete:                 return new SltDelete(this);
        case FdoCommandType_Insert:                 return new SltInsert(this);
        case FdoCommandType_SQLCommand:             return new SltSql(this);
        case FdoCommandType_CreateDataStore:        return new SltCreateDataStore(this);
        case FdoCommandType_CreateSpatialContext:   return new SltCreateSpatialContext(this);
        case FdoCommandType_ApplySchema:            return new SltApplySchema(this);
        default: break;
    }
    
    return NULL;
}

//-------------------------------------------------------
// FdoIConnectionPropertyDictionary implementation
//-------------------------------------------------------

FdoString** SltConnection::GetPropertyNames(FdoInt32& count)
{
    static const wchar_t* PROP_NAMES[] = {PROP_NAME_FILENAME};
    count = 1;
    return (const wchar_t**)PROP_NAMES;
}

FdoString* SltConnection::GetProperty(FdoString* name)
{
    return (*m_mProps)[name].c_str();
}

void SltConnection::SetProperty(FdoString* name, FdoString* value)
{
    // check the connection
    if (GetConnectionState() != FdoConnectionState_Closed)
        throw FdoConnectionException::Create(L"Attempt to set property on open connection");

    if (value)
        (*m_mProps)[name] = value;
    else
        (*m_mProps)[name] = L"";
}

FdoString* SltConnection::GetPropertyDefault(FdoString* name)
{
    return L"";
}

bool SltConnection::IsPropertyRequired(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return true;

    return false;
}

bool SltConnection::IsPropertyProtected(FdoString* name)
{
    return false;
}

bool SltConnection::IsPropertyFileName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return true;

    return false;
}

bool SltConnection::IsPropertyFilePath(FdoString* name)
{
    return false;
}

bool SltConnection::IsPropertyDatastoreName(FdoString* name)
{
    return false;
}

bool SltConnection::IsPropertyEnumerable(FdoString* name)
{
    return false;
}

FdoString** SltConnection::EnumeratePropertyValues(FdoString* name, FdoInt32& count)
{
    return NULL;
}

FdoString* SltConnection::GetLocalizedName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return L"File";

    return NULL;
}


//--------------------------------------------------------------------------
//
//        Command helpers -- implementation of command functionality
//
//--------------------------------------------------------------------------



//Maps sqlite table descriptions to an FDO schema
FdoFeatureSchemaCollection* SltConnection::DescribeSchema()
{
    if (m_pSchema)
        return FDO_SAFE_ADDREF(m_pSchema);

    if (!m_db)
        return NULL;

    m_pSchema = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"Default", L"");
    
    m_pSchema->Add(schema.p);
    
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    
    //first, make a list of all tables that can be FDO feature classes
    vector<string> tables;

    string tables_sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    if (sqlite3_prepare_v2(m_db, tables_sql.c_str(), -1, &pstmt, &pzTail) == SQLITE_OK)
    {
        while (sqlite3_step(pstmt) == SQLITE_ROW)
            tables.push_back((const char*)sqlite3_column_text(pstmt, 0));
    }
    else
    {
        wstring err = A2W_SLOW(pzTail);
        throw FdoException::Create(err.c_str());
    }

    sqlite3_finalize(pstmt);

    //now for each table, map to an FDO class
    for (size_t i=0; i<tables.size(); i++)
    {
        //first check if table is an FDO metadata table and skip it
        if (IsMetadataTable(tables[i].c_str()))
            continue;

        SltMetadata* md = GetMetadata(tables[i].c_str());
        
        if (md)
        {
            FdoPtr<FdoClassDefinition> fc = md->ToClass();
            classes->Add(fc);
        }
    }

    return FDO_SAFE_ADDREF(m_pSchema);
}


FdoISpatialContextReader* SltConnection::GetSpatialContexts()
{
    return new SltSpatialContextReader(this);
}

FdoIFeatureReader* SltConnection::Select(FdoIdentifier* fcname, FdoFilter* filter, FdoIdentifierCollection* props)
{
    if (m_connState != FdoConnectionState_Open)
        throw FdoCommandException::Create(L"Connection must be open in order to Select.");

    string mbfc = W2A_SLOW(fcname->GetName());
    DBounds bbox;
    string where;
    bool canFastStep = true;

    //Translate the filter from FDO to SQLite where clause.
    //Also detext if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        SltQueryTranslator qt(NULL);
        filter->Process(&qt);

        where = W2A_SLOW(qt.GetFilter());
        qt.GetBBOX(bbox);
        canFastStep = qt.CanUseFastStepping();
    }

    SpatialIterator* siter = NULL;

    //if we have a BBOX filter, we need to get the spatial index
    if (!bbox.IsEmpty())
    {
        SpatialIndex* si = GetSpatialIndex(mbfc.c_str());
        siter = new SpatialIterator(bbox, si);
    }
   
    return new SltReader(this, props, mbfc.c_str(), where.c_str(), siter, canFastStep);
}

FdoIDataReader* SltConnection::SelectAggregates(FdoIdentifier*              fcname, 
                                                FdoIdentifierCollection*    properties,
                                                bool                        bDistinct,
                                                FdoOrderingOption           eOrderingOption,
                                                FdoIdentifierCollection*    ordering,
                                                FdoFilter*                  filter,
                                                FdoIdentifierCollection*    grouping)
{
    FdoString* fc = fcname->GetName();
    string mbfc = W2A_SLOW(fc);

    std::string sql;

    if (bDistinct)
    {
        //make SQL for distict values 
        
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        string mbname = W2A_SLOW(id->GetName());
        
        sql = "SELECT DISTINCT " + mbname + " FROM " + mbfc;
    }
    else
    {
        SltMetadata* md = GetMetadata(mbfc.c_str());

        FdoPtr<FdoClassDefinition> fc = md->ToClass();

        if (!filter && fc->GetClassType() == FdoClassType_FeatureClass)
        {
            SltReader* rdr = CheckForSpatialExtents(properties, (FdoFeatureClass*)fc.p);

            if (rdr)
                return rdr;
        }

        //select aggregate -- only one computed identifier expected!
        FdoPtr<FdoIdentifier> id = properties->GetItem(0);
        FdoPtr<FdoComputedIdentifier> ci = dynamic_cast<FdoComputedIdentifier*>(id.p);

        FdoString* exprs = ci->ToString();
        string mbexprs = W2A_SLOW(exprs);

        sql = "SELECT " + mbexprs + " FROM " + mbfc;
    }

    return new SltReader(this, sql.c_str());
}


FdoInt32 SltConnection::Update(FdoIdentifier* fcname, FdoFilter* filter, FdoPropertyValueCollection* propvals)
{
    string mbfc = W2A_SLOW(fcname->GetName());

    string sql="UPDATE " + mbfc + " SET ";

    //TODO: currently ignores spatial filter in delete

    for (int i=0; i<propvals->GetCount(); i++)
    {
        if (!i) sql += ",";

        FdoPtr<FdoPropertyValue> pv = propvals->GetItem(i);
        FdoPtr<FdoIdentifier> id = pv->GetName();

        sql += W2A_SLOW(id->GetName()) + "=?";
    }

    sql += " WHERE " + W2A_SLOW(filter->ToString()) + ";";

    const char* tail = NULL;
    sqlite3_stmt* stmt = NULL;
    
    if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, &tail) != SQLITE_OK)
    {
        BindPropVals(propvals, stmt);
        
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw FdoCommandException::Create(L"Update failed.");
        }
    }
    else
    {
        throw FdoCommandException::Create(L"Failed to parse update statement.");
    }

    sqlite3_finalize(stmt);

    return sqlite3_changes(m_db);
}

FdoInt32 SltConnection::Delete(FdoIdentifier* fcname, FdoFilter* filter)
{
    string mbfc = W2A_SLOW(fcname->GetName());
    
    //TODO: currently ignores spatial filter in delete
    string sql = "DELETE FROM " + mbfc;

    if (filter)
    {
        sql += " WHERE ";
        string where = W2A_SLOW(filter->ToString());
        sql += where;
    }

    sql += ";";

    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;

    if (sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, &tail) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) != SQLITE_DONE)
        {
            sqlite3_finalize(stmt);
            throw FdoCommandException::Create(L"Failed to execute delete statement.");
        }
    }
    else
    {
        throw FdoCommandException::Create(L"Failed to parse delete statement.");
    }

    sqlite3_finalize(stmt);
    return sqlite3_changes(m_db);
}


FdoInt32 SltConnection::ExecuteNonQuery(FdoString* sql)
{
    string mbsql = W2A_SLOW(sql);

    sqlite3_stmt* pStmt = GetCachedParsedStatement(mbsql);

    int count = 0;
    int rc;

    while ((rc = sqlite3_step(pStmt)) == SQLITE_ROW) count++;

    ReleaseParsedStatement(mbsql, pStmt);

    if (rc == SQLITE_DONE)
        return count;
    else 
        throw FdoCommandException::Create(L"Failed to execute sql command.");
}

FdoISQLDataReader* SltConnection::ExecuteReader(FdoString* sql)
{
    string mbsql = W2A_SLOW(sql);
    return new SltReader(this, mbsql.c_str());
}


SltMetadata* SltConnection::GetMetadata(const char* table)
{
    SltMetadata* const REALLY_BAD_POINTER = (SltMetadata*)1;

    std::string stable(table);

    SltMetadata* ret = m_mNameToMetadata[stable];

    //check if we already know that no such table exists
    if (ret == REALLY_BAD_POINTER)
        return NULL;

    if (!ret)
    {
        if (IsMetadataTable(table))
            ret = REALLY_BAD_POINTER;
        else
        {
            ret = new SltMetadata(m_db, table);

            if (ret->Failed())
            {
                delete ret;
                ret = REALLY_BAD_POINTER;
            }
            else //if we got a table, also create the cached FDO feature class corresponding to it
                ret->ToClass()->Release();
        }

        m_mNameToMetadata[stable] = ret;
    }

    return ret;
}

SpatialIndex* SltConnection::GetSpatialIndex(const char* table)
{
    SpatialIndex* si = m_mNameToSpatialIndex[table];

    //build the spatial index, if this is the first time it
    //is needed
    if (!si)
    {
#if 0
    clock_t t0 = clock();
#endif
    si = new SpatialIndex(GetProperty(PROP_NAME_FILENAME));

    SltReader* rdr = new SltReader(this, NULL, table, "", NULL, true);
    m_mNameToSpatialIndex[table] = si;
    DBounds ext;

    while (rdr->ReadNext())
    {
        int len;

        //With a default SltReader we know that rowID will
        //be returned in column 0 and geometry in column 1
        //Accessing by index is significantly faster than
        //accessing by name
        FdoInt32 id = rdr->GetInt32(0);
        const FdoByte* geom = rdr->GetGeometry(1, &len);

        //TODO: assumes DBounds is 2D since GetFgfExtents is 2D
        GetFgfExtents((unsigned char*)geom, len, (double*)&ext);

        si->Insert(id, ext);
    }

    rdr->Close();
    delete rdr;
    si->ReOpenForRead();
#if 0
    clock_t t1 = clock();
    printf("Spatial index build time: %d\n", t1 - t0);
#endif
    }

    return si;
}


void SltConnection::AddGeomCol(FdoGeometricPropertyDefinition* gpd, const wchar_t* fcname)
{
    //add this geometry property to the geometry_columns table
    string gpname = W2A_SLOW(gpd->GetName());
    string gci_sql = "INSERT INTO geometry_columns\
        (f_table_name,\
        f_geometry_column,\
        geometry_format,\
        geometry_type,\
        coord_dimension,\
        srid)\
        VALUES(";
    
    gci_sql += "'" + W2A_SLOW(fcname) + "'"; //f_table_name
    gci_sql += ",";
    gci_sql += "'" + gpname + "'"; //f_geometry_column
    gci_sql += ",'FGF',"; //f_geometry_format

    char sdim[16];

    int len = 0;
    int gtype = 0;
    FdoGeometryType* gtypes = gpd->GetSpecificGeometryTypes(len);
    if (len == 1)
    {
        gtype = *gtypes;
    }
    else
    {
        //Try to get less specific geometric type indicator.
        //Here, the returned value is a mask of FdoGeometricTypes, 
        //which also do not distinguish between regular and multi- geometries
        //i.e. FdoGeometricType_Point is for both Points and MultiPoints.
        FdoGeometricType gictype = (FdoGeometricType)gpd->GetGeometryTypes();

        //Attempt some reasonable mapping to actual geometry type
        //-- simply picks the non-multi geometry type that corresponds
        //to the geometry type. Confusing, huh?
        switch (gictype)
        {
        case FdoGeometricType_Point: gtype = FdoGeometryType_Point; break;
        case FdoGeometricType_Curve: gtype = FdoGeometryType_LineString; break;
        case FdoGeometricType_Surface: gtype = FdoGeometryType_Polygon; break;
        }
    }

    //if gtype remains 0 at this points, it will be treated as "All" types
    //of geometry
    sprintf(sdim, "%d", gtype);

    gci_sql += sdim + string(",");

    int dim = 2;
    if (gpd->GetHasElevation()) dim++;
    if (gpd->GetHasMeasure()) dim++;
    sprintf(sdim, "%d", dim);
    gci_sql += sdim; //coord_dimension
    gci_sql += ",";
    
    //TODO: we assume the spatial context association is
    //a string representing the SRID (integer id) of a coordinate
    //system that already exists in our spatial_ref_sys table.
    FdoString* sca = gpd->GetSpatialContextAssociation();
    gci_sql += sca ? W2A_SLOW(sca) : "0"; //srid

    gci_sql += ");";

    int rc = sqlite3_exec(m_db, gci_sql.c_str(), NULL, NULL, NULL);
}


void SltConnection::ApplySchema(FdoFeatureSchema* schema)
{
    //TODO:
    //for now, only implement addition of feature classes
    FdoPtr<FdoFeatureSchemaCollection> myschemac = DescribeSchema();
    FdoPtr<FdoFeatureSchema> myschema = myschemac->GetItem(0);
    FdoPtr<FdoClassCollection> myclasses = myschema->GetClasses();

    FdoPtr<FdoClassCollection> inclasses = schema->GetClasses();

    int rc = sqlite3_exec(m_db, "BEGIN;", NULL, NULL, NULL);

    for (int i=0; i<inclasses->GetCount(); i++)
    {
        FdoPtr<FdoClassDefinition> fc = inclasses->GetItem(i);
        
        if (myclasses->Contains(fc->GetName()))
            continue; //Warning! -- class already exists... Skip it.

        //create the sql to create a SQLite table corresponding to the feature class
        string sql = "CREATE TABLE " + W2A_SLOW(fc->GetName()) + "(";

        FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
        FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
        FdoPtr<FdoGeometricPropertyDefinition> gpd;
        
        if (fc->GetClassType() == FdoClassType_FeatureClass)
            gpd = ((FdoFeatureClass*)fc.p)->GetGeometryProperty();

        FdoPtr<FdoDataPropertyDefinition> idp = idpdc->GetItem(0);

        if (idpdc->GetCount() > 1)
        {
            printf ("Source class has more than one identity property -- the converter does not support that.");
            continue;
        }
        else
        {
            string name = W2A_SLOW(idp->GetName());
            
            FdoDataType dt = idp->GetDataType();
            if (idp->GetIsAutoGenerated() 
                || dt == FdoDataType_Int32 
                || dt == FdoDataType_Int64)
            {
                //autogenerated ID -- we will have SQLite generate a new one for us
                sql += name + " INTEGER PRIMARY KEY";
            }
            else
            {
                sql += name;
                sql += " ";
                sql += g_fdo2sql_map[idp->GetDataType()];
                sql += " UNIQUE";
            }
        }

        //now do the geometry property -- we want that to be near the 
        //beginning of the list for better performance when reading
        if (gpd.p)
        {
            string gpname = W2A_SLOW(gpd->GetName());
            sql += ", ";
            sql +=  gpname + " BLOB";

            //also add to the geometry_columns table
            AddGeomCol(gpd.p, fc->GetName());
        }


        //add the remaining properties
        for (int i=0; i<pdc->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);

            FdoPropertyType ptype = pd->GetPropertyType();

            if (ptype == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)pd.p;

                //make sire it is not the id property, we did that already
                if (idpdc->Contains(dpd->GetName()))
                    continue;

                sql += ", ";
                sql += W2A_SLOW(dpd->GetName());
                sql += " ";
                sql += g_fdo2sql_map[dpd->GetDataType()];
            }
            else if (ptype == FdoPropertyType_GeometricProperty)
            {
                //make sure it is not THE geometry property which we already did
                if (wcscmp(gpd->GetName(), pd->GetName()) == 0)
                    continue;

                sql += ", ";
                sql += W2A_SLOW(pd->GetName()) + " BLOB";

                AddGeomCol((FdoGeometricPropertyDefinition*)pd.p, fc->GetName());
            }
        }

        sql += ");"; //close the create table command

        //printf ("SQLite Feature class: %s\n", sql.c_str());

        //create the database table for this feature class
        int rc = sqlite3_exec(m_db, sql.c_str(), NULL, NULL, NULL);
    }

    rc = sqlite3_exec(m_db, "COMMIT;", NULL, NULL, NULL);

    //the cached FDO schema will need to be refreshed
    FDO_SAFE_RELEASE(m_pSchema);
}


SltReader* SltConnection::CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc)
{
    //Checks for a SpatialExtents or Count request
    std::string fcname = W2A_SLOW(fc->GetName());
    std::string gname;
    std::string extname = ""; //if this remains empty, we know extents is not needed
    std::string countname = "TheCount";

    int propsCount = props->GetCount();
    if (propsCount == 1 || propsCount == 2)
    {
        for (int i=0; i<propsCount; i++)
        {
            FdoPtr<FdoIdentifier> identifier = props->GetItem(i);
            FdoComputedIdentifier* computedIdentifier = dynamic_cast<FdoComputedIdentifier*>(identifier.p);
        
            if (computedIdentifier) 
            {
                FdoPtr<FdoExpression> expr = computedIdentifier->GetExpression();
                FdoFunction* func = dynamic_cast<FdoFunction*>(expr.p);
                if (func && (_wcsicmp(func->GetName(), FDO_FUNCTION_SPATIALEXTENTS) == 0))
                {
                    FdoPtr<FdoExpressionCollection> args = func->GetArguments();
                    FdoPtr<FdoExpression> arg = args->GetItem(0);
                    FdoIdentifier* argId = dynamic_cast<FdoIdentifier*>(arg.p);
                    if (fc)
                    {
                        FdoPtr<FdoGeometricPropertyDefinition> geomProp = fc->GetGeometryProperty();
                        if (geomProp && argId && 0==wcscmp(argId->GetName(), geomProp->GetName()))
                            extname = W2A_SLOW(computedIdentifier->GetName());
                    }
                }
                else if (func && (_wcsicmp(func->GetName(), FDO_FUNCTION_COUNT) == 0))
                {
                    countname = W2A_SLOW(computedIdentifier->GetName());
                }
                else
                {
                    return NULL;
                }
            }
            else
            {
                return NULL;
            }
        }
    }
    else
    {
        return NULL;
    }

    //ok this is a spatial extents or count computed property.
    //Look up the extents and count and return a reader with them.
    //We will always return count, but not extents, if they are not needed.

    //create a temporary table to hold return result
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);

    //now get the count -- this is approximate since
    //a real count would take a long time (table scan)
    //we always use the count, whether we compute extents or not
    sqlite3_stmt* stmt;
    const char* tail = NULL;

    std::string sql = "SELECT MAX(ROWID) FROM " + fcname + ";";
    rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, &tail);
    rc = sqlite3_step(stmt);
    int count = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);
    stmt = NULL;
    tail = NULL;

    //case where we only need count
    if (extname.empty())
    {
        sql = "CREATE TABLE SpatialExtentsResult (" + countname + " INTEGER);";

        char* err;
        rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &err);

        if (rc)
            sqlite3_free(err);

        //insert into the temporary table
        sql = "INSERT INTO SpatialExtentsResult VALUES(?);";

        //TODO: some day we should check the error codes...
        //but really, failure is not an option here.
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);
        rc = sqlite3_bind_int(stmt, 1, count);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
    }
    else
    {
        //get the extents and convert to blob
        SpatialIndex* si = GetSpatialIndex(fcname.c_str());
        DBounds ext;
        si->GetTotalExtent(ext);

        //convert to FGF byte array
        FgfPolygon poly;
        poly.np = 5;

        poly.p[0] = ext.min[0];   poly.p[1] = ext.min[1];
        poly.p[2] = ext.max[0];   poly.p[3] = ext.min[1];
        poly.p[4] = ext.max[0];   poly.p[5] = ext.max[1];
        poly.p[6] = ext.min[0];   poly.p[7] = ext.max[1];
        poly.p[8] = ext.min[0];   poly.p[9] = ext.min[1];


        std::string sql = "CREATE TABLE SpatialExtentsResult (" + extname + " BLOB," + countname + " INTEGER);";

        char* err;
        rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &err);

        if (rc)
            sqlite3_free(err);

        //insert into the temporary table
        sql = "INSERT INTO SpatialExtentsResult VALUES(?,?);";

        //TODO: some day we should check the error codes...
        //but really, failure is not an option here.
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);
        rc = sqlite3_bind_blob(stmt, 1, &poly, sizeof(FgfPolygon), SQLITE_TRANSIENT);
        rc = sqlite3_bind_int(stmt, 2, count);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
    }

    sql = "SELECT * FROM SpatialExtentsResult;";

    stmt = NULL;
    tail = NULL;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, &tail);

    if (rc == SQLITE_OK)
        return new SltReader(this, stmt);        
    else
        throw FdoException::Create(L"Failed to generate Count() or SpatialExtents() reader.");
}


sqlite3_stmt* SltConnection::GetCachedParsedStatement(const std::string& sql)
{
    //Don't let too many queries get cached.
    //There are legitimate cases where lots of different
    //queries can be issued on the same connection.
    if (m_mCachedQueries.size() > 1000)
    {
        ClearQueryCache();
    }

    sqlite3_stmt* ret = m_mCachedQueries[sql];

    if (!ret)
    {
        const char* tail = NULL;
        int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &ret, &tail);

        if (rc != SQLITE_OK)
        {
            throw FdoException::Create(L"Failed to parse SQL statement");
        }
    }
    else
    {
        m_mCachedQueries[sql] = NULL;
    }

    return ret;
}


void SltConnection::ReleaseParsedStatement(const std::string& sql, sqlite3_stmt* stmt)
{
    sqlite3_stmt* exists = m_mCachedQueries[sql];

    if (!exists)
    {
        sqlite3_reset(stmt);
        m_mCachedQueries[sql] = stmt;
    }
    else
    {
        sqlite3_finalize(stmt);
    }
}


void SltConnection::ClearQueryCache()
{
    for (std::map<std::string, sqlite3_stmt*>::iterator iter = m_mCachedQueries.begin(); 
        iter != m_mCachedQueries.end(); iter++)
        sqlite3_finalize(iter->second);

    m_mCachedQueries.clear();
}


void SltConnection::GetExtents(const wchar_t* fcname, double ext[4])
{
    std::string table = W2A_SLOW(fcname);

    SpatialIndex* si = GetSpatialIndex(table.c_str());

    if (si)
    {
        DBounds dext;
        si->GetTotalExtent(dext);

        ext[0] = dext.min[0];
        ext[1] = dext.min[1];
        ext[2] = dext.max[0];
        ext[3] = dext.max[1];
    }
}
