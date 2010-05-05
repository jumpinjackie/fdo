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
#include "RowidIterator.h"
#include "StringUtil.h"
#include "SltQueryTranslator.h"
#include "FdoCommonSchemaUtil.h"
#include "SQLiteSchemaMergeContext.h"
#include "SltTransaction.h"
#include <sys/stat.h>

#ifndef _MSC_VER
#include "SpatialIndex.h"
#else
#include "DiskSpatialIndex.h"
#endif

#include "SpatialIndexDescriptor.h"

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

    if (sqlite3StrICmp(table, "fdo_columns") == 0)
        return true;

    if (sqlite3StrICmp(table, "sqlite_sequence") == 0)
        return true;
    
    return false;
}

std::map<int, std::string> g_fdo2sql_map;

SltConnection::SltConnection() : m_refCount(1)
{
    if (g_fdo2sql_map.empty())
    {
        g_fdo2sql_map[FdoDataType_BLOB] =       "BLOB";
        g_fdo2sql_map[FdoDataType_CLOB] =       "TEXT";
        g_fdo2sql_map[FdoDataType_Boolean] =    "INTEGER";
        g_fdo2sql_map[FdoDataType_Byte] =       "INTEGER";
        g_fdo2sql_map[FdoDataType_DateTime] =   "TEXT";
        g_fdo2sql_map[FdoDataType_Decimal] =    "REAL";
        g_fdo2sql_map[FdoDataType_Double] =     "REAL";
        g_fdo2sql_map[FdoDataType_Int16] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Int32] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Int64] =      "INTEGER";
        g_fdo2sql_map[FdoDataType_Single] =     "REAL";
        g_fdo2sql_map[FdoDataType_String] =     "TEXT";
    }

    m_dbWrite = NULL;
    m_pSchema = NULL;
    m_mProps = new std::map<std::wstring, std::wstring>();
    m_connState = FdoConnectionState_Closed;

    m_caps = new SltCapabilities();

    m_bUseFdoMetadata = false;
    m_bHasFdoMetadata = false;
    m_transactionState = SQLiteActiveTransactionType_None;
    m_cSupportsDetGeomType = -1;
    m_wkbBuffer = NULL;
    m_wkbBufferLen = 0;
    m_updateHookEnabled = false;
    m_changesAvailable = false;
    m_connDet = NULL;
    m_defSpatialContextId = 0;
    m_isReadOnlyConnection = true;
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
    
    if(!value)
        return;

    std::wstring token;
    bool isError = false;
    short state = 0;
    int pos = 0, possn = 0, possv = 0, posend = 0;
    do
    {
        switch(state)
        {
            case 0: // looking for start property name skipping spaces, e.g. <  Name=Val>
                if (*(value+pos) == L'=')
                    isError = true;
                else if (*(value+pos) != L' ' && *(value+pos) != L';')
                {
                    possn = pos;
                    posend = pos+1;
                    state = 1;
                }
                token.clear();
            break;
            case 1: //get property name skipping spaces after name, e.g. <Name  =Val>
                if (*(value+pos) == L'=')
                {
                    token.append(value+possn, posend-possn);
                    if (*(value+pos+1) == L'\"')
                    {
                        pos++;
                        state = 3;
                    }
                    else if (*(value+pos+1) == L' ')
                    {
                        pos++;
                        state = 4;
                    }
                    else
                    {
                        posend = pos+1;
                        state = 2;
                    }
                    possv = pos+1;
                }
                else if(*(value+pos) == L';' || *(value+pos) == L'\0')
                    isError = true;
                else if(*(value+pos) != L' ')
                    posend = pos+1;
            break;
            case 2:  // get property value in case value is not surrounded by "
                if(*(value+pos) == L'\"')
                    isError = true;
                else if(*(value+pos) == L';' || *(value+pos) == L'\0')
                {
                    if (posend != possv)
                    {
                        std::wstring val(value+possv, posend-possv);
                        SetProperty(token.c_str(), val.c_str());
                    }
                    else
                        SetProperty(token.c_str(), L"");
                    state = 0;
                }
                else if(*(value+pos) != L' ')
                    posend = pos+1;
            break;
            case 3:  //get property value in case value is surrounded by "
                if(*(value+pos) == L'\"')
                {
                    if (pos != possv)
                    {
                        std::wstring val(value+possv, pos-possv);
                        SetProperty(token.c_str(), val.c_str());
                    }
                    else
                        SetProperty(token.c_str(), L"");
                    state = 0;
                }
                else if(*(value+pos+1) == L'\0')
                    isError = true;
            break;
            case 4:  // handle space before ", it skipsd all spaces before ", e.g. <Name=  Val;>
                if (*(value+pos) == L'\"')
                {
                    pos++;
                    state = 3;
                }
                else if(*(value+pos) == L';')
                {
                    // handle empty values like: <File=;> or <File="";> or <File= ;>
                    // and cases like <File=val; ;; Test=;>
                    if (!token.empty())
                        SetProperty(token.c_str(), L"");
                    state = 0;
                }
                else if(*(value+pos) != L' ')
                {
                    posend = pos;
                    state = 2;
                }
                possv = pos;
            break;
        }
        pos++;
    }while(*(value+pos-1) != L'\0' && !isError);

    if (isError)
        throw FdoConnectionException::Create(L"Invalid connection string!");
}

//creates a new database, based on the filename stored in the
//connection string and connection property dictionary.
void SltConnection::CreateDatabase()
{
    if (m_dbWrite || m_connState != FdoConnectionState_Closed)
    {
        throw FdoCommandException::Create(L"Cannot create data store while connection is in open state.");
    }

    const wchar_t* dsw = GetProperty(PROP_NAME_FILENAME);
    
    std::string file = W2A_SLOW(dsw);

    sqlite3* tmpdb = NULL;

    //create the database
    //TODO: this will also work if the database exists -- in which 
    //case it will open it.
    int rc;
    if((rc = sqlite3_open(file.c_str(), &tmpdb)) != SQLITE_OK )
    {
        std::wstring err = std::wstring(L"Failed to open or create: ") + dsw;
        throw FdoCommandException::Create(err.c_str(), rc);
    }

    //first things first -- set big page size for better
    //performance. Must be done on an empty db to have effect.
    rc = sqlite3_exec(tmpdb, "PRAGMA page_size=32768;", NULL, NULL, NULL);
    rc = sqlite3_exec(tmpdb, "PRAGMA journal_mode=MEMORY;", NULL, NULL, NULL);
    
    //create the spatial_ref_sys table
    //Note the sr_name field is not in the spec, we are adding it in order to 
    //match fdo feature class geometry properties to rows in the spatial_ref_sys table.
    //This is because geometry properties use a string spatial context association.
    const char* srs_sql = "CREATE TABLE spatial_ref_sys"
                          "(srid INTEGER PRIMARY KEY,"
                          "sr_name TEXT, "
                          "auth_name TEXT,"
                          "auth_srid INTEGER,"
                          "srtext TEXT "
                          ");";

    char* zerr = NULL;
    rc = sqlite3_exec(tmpdb, srs_sql, NULL, NULL, &zerr);

    //create the geometry_columns table
    const char* gc_sql = "CREATE TABLE geometry_columns "
                         "(f_table_name TEXT,"
                         "f_geometry_column TEXT,"
                         "geometry_type INTEGER,"
                         "geometry_dettype INTEGER,"
                         "coord_dimension INTEGER,"
                         "srid INTEGER,"
                         "geometry_format TEXT);";  

    int rc2 = sqlite3_exec(tmpdb, gc_sql, NULL, NULL, &zerr);

    //Check if we should create the FDO metadata table as well
    dsw = GetProperty(PROP_NAME_FDOMETADATA);
    int rc3 = 0;

    if (dsw && _wcsicmp(dsw, L"true") == 0)
    {
        const char* fc_sql = "CREATE TABLE fdo_columns "
                             "(f_table_name TEXT,"
                             "f_column_name TEXT,"
                             "f_column_desc TEXT,"
                             "fdo_data_type INTEGER,"
                             "fdo_data_details INTEGER," // flag usage readonly = 0x02, system = 0x01
                             "fdo_data_length INTEGER,"
                             "fdo_data_precision INTEGER,"
                             "fdo_data_scale INTEGER);";  

        rc3 = sqlite3_exec(tmpdb, fc_sql, NULL, NULL, &zerr);
    }

    //close the database -- CreateDataStore itself does not 
    //open the FDO connection, subsequent call to Open() does.
    sqlite3_close(tmpdb);

    if (rc || rc2 || rc3)
    {
        throw FdoCommandException::Create(L"Failed to create SQLite database.", rc ? rc : rc2 ? rc2 : rc3);
    }
}

FdoConnectionState SltConnection::Open()
{
    if (m_connState == FdoConnectionState_Open)
        return m_connState;

    const wchar_t* dsw = GetProperty(PROP_NAME_FILENAME);
    
    std::string file = W2A_SLOW(dsw);

#ifdef _WIN32
    struct _stat statInfo;
    if (0 != _wstat (dsw, &statInfo) || (statInfo.st_mode&_S_IFREG) == 0)
        throw FdoConnectionException::Create(L"File does not exist!");
    if ((statInfo.st_mode&_S_IREAD) == 0)
        throw FdoConnectionException::Create(L"File cannot be accessed!");
#else 
    struct stat statInfo;
    if (0 != stat (file.c_str(), &statInfo) || (statInfo.st_mode&S_IFREG) == 0)
        throw FdoConnectionException::Create(L"File does not exist!");
    if ((statInfo.st_mode&S_IREAD) == 0)
        throw FdoConnectionException::Create(L"File cannot be accessed!");
#endif

    const wchar_t* sUseMeta = GetProperty(PROP_NAME_FDOMETADATA);

    if (sUseMeta != NULL && _wcsicmp(sUseMeta, L"true") == 0)
        m_bUseFdoMetadata = true;

    //We will use two connections to the database -- one for reading and one for writing.
    //This will help us with concurrent reads and writes (to different tables).
    //If we use the same SQLite connection, we will get problems with transaction nesting
    //due to interleaved reads and writes.

    //Allow sharing of memory caches between the reading and writing connections
    int rc = sqlite3_enable_shared_cache(1);

    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to enable shared cache.\n");
    //Open the Read connection
    if( (rc = sqlite3_open(file.c_str(), &m_dbWrite)) != SQLITE_OK )
    {
        m_dbWrite = NULL;
        std::wstring err = std::wstring(L"Failed to open ") + dsw;
        throw FdoConnectionException::Create(err.c_str(), rc);
    }

    rc = sqlite3_exec(m_dbWrite, "PRAGMA read_uncommitted=1;", NULL, NULL, NULL);

    //Register the extra SQL functions we would like to support
    RegisterExtensions(m_dbWrite);
    rc = sqlite3_exec(m_dbWrite, "PRAGMA journal_mode=MEMORY;", NULL, NULL, NULL);

    //in case we have a FDO metadata use it since we can make things out of sync by mixing things.
    const char* tables_sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='fdo_columns';";
    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    m_bHasFdoMetadata = false;
    rc = sqlite3_prepare_v2(m_dbWrite, tables_sql, -1, &pstmt, &pzTail); 
    if (rc == SQLITE_OK)
    {
        while (sqlite3_step(pstmt) == SQLITE_ROW)
        {
            m_bHasFdoMetadata = true;
            break;
        }
    }
    else if (rc == SQLITE_NOTADB)
        throw FdoException::Create(L"File opened that is not a database file.", rc);
    else
        m_bHasFdoMetadata = false;

    sqlite3_finalize(pstmt);

    m_connState = FdoConnectionState_Open;
    
    m_connDet = new ConnInfoDetails(this);
    m_dbWrite->pUserArg = m_connDet;

#ifdef _WIN32
    m_isReadOnlyConnection = ((statInfo.st_mode&_S_IWRITE)==0);
#else // _WIN32
    m_isReadOnlyConnection = ((statInfo.st_mode&S_IWRITE)==0);
#endif // _WIN32
    return m_connState;
}

void SltConnection::Close()
{
    //free the spatial indexes
    for (SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.begin();
         iter != m_mNameToSpatialIndex.end(); iter++)
    {
        delete iter->second;
        free(iter->first); //was allocated using strdup
    }

    for (MaxRecordInfoCache::iterator iter = m_mTableRecInfo.begin();
         iter != m_mTableRecInfo.end(); iter++)
    {
        if (iter->second != NULL)
        {
            iter->second->SetReleased(true);
            iter->second->Release();
        }
        free(iter->first); //was allocated using strdup
    }
    

    m_mTableRecInfo.clear();
    m_mNameToSpatialIndex.clear();

    //clear the cached schema metadata
    for (MetadataCache::iterator iter = m_mNameToMetadata.begin();
         iter != m_mNameToMetadata.end(); iter++)
    {
         delete iter->second;
         free(iter->first); //it was created via strdup, must use free()
    }

    m_mNameToMetadata.clear();
    
    switch(m_transactionState)
    {
    case SQLiteActiveTransactionType_Internal:
        CommitTransaction();
        break;
    case SQLiteActiveTransactionType_User:
        RollbackTransaction(true); // enforce a rollback
        break;
    }

    ClearQueryCache();
    
    delete[] m_wkbBuffer;
    m_wkbBuffer = NULL;
    m_wkbBufferLen = 0;

    if (m_dbWrite)
    {
        m_dbWrite->pUserArg = NULL;
        if(sqlite3_close(m_dbWrite) != SQLITE_BUSY)
            m_dbWrite = NULL;
    }
    
    FDO_SAFE_RELEASE(m_pSchema);

    m_connState = FdoConnectionState_Closed;

    m_updateHookEnabled = false;
    m_changesAvailable = false;
    m_isReadOnlyConnection = true;

    delete m_connDet;
    m_connDet = NULL;
}

FdoICommand* SltConnection::CreateCommand(FdoInt32 commandType)
{
    switch(commandType)
    {
        case FdoCommandType_DescribeSchema :        return new SltDescribeSchema(this);
        case FdoCommandType_GetSpatialContexts :    return new SltGetSpatialContexts(this);
        case FdoCommandType_Select :                return new SltExtendedSelect(this);
        case FdoCommandType_SelectAggregates :      return new SltSelectAggregates(this);
        case FdoCommandType_Update:                 return new SltUpdate(this);
        case FdoCommandType_Delete:                 return new SltDelete(this);
        case FdoCommandType_Insert:                 return new SltInsert(this);
        case FdoCommandType_SQLCommand:             return new SltSql(this);
        case FdoCommandType_CreateDataStore:        return new SltCreateDataStore(this);
        case FdoCommandType_CreateSpatialContext:   return new SltCreateSpatialContext(this);
        case FdoCommandType_ApplySchema:            return new SltApplySchema(this);
        case FdoCommandType_ExtendedSelect:         return new SltExtendedSelect(this);
        default: break;
    }
    
    return NULL;
}

//-------------------------------------------------------
// FdoIConnectionPropertyDictionary implementation
//-------------------------------------------------------

FdoString** SltConnection::GetPropertyNames(FdoInt32& count)
{
    //NOTE: We advertise (at least for testing purposes) the FDO metadata flag
    static const wchar_t* PROP_NAMES[] = {PROP_NAME_FILENAME, PROP_NAME_FDOMETADATA};
    count = 2;
    return (const wchar_t**)PROP_NAMES;
}

FdoString* SltConnection::GetProperty(FdoString* name)
{
    std::map<std::wstring, std::wstring>::iterator val = m_mProps->find(name);

    if (val == m_mProps->end())
        return NULL;

    return val->second.c_str();
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
    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
        return VAL_FALSE;
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
    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
        return true;
    return false;
}

FdoString** SltConnection::EnumeratePropertyValues(FdoString* name, FdoInt32& count)
{
    static const wchar_t* PROP_VALUES[] = {VAL_TRUE, VAL_FALSE};
    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
    {
        count = 2;
        return PROP_VALUES;
    }
    return NULL;
}

FdoString* SltConnection::GetLocalizedName(FdoString* name)
{
    if (wcscmp(name, PROP_NAME_FILENAME) == 0)
        return PROP_NAME_FILENAME;

    if (wcscmp(name, PROP_NAME_FDOMETADATA) == 0)
        return L"Fdo Enabled";

    return NULL;
}


//--------------------------------------------------------------------------
//
//        Command helpers -- implementation of command functionality
//
//--------------------------------------------------------------------------



//Maps sqlite table descriptions to an FDO schema
FdoFeatureSchemaCollection* SltConnection::DescribeSchema(FdoStringCollection* classNames, bool makeACopy)
{
    if (classNames && classNames->GetCount() != 0)
    {
        FdoPtr<FdoFeatureSchemaCollection> pSchema = FdoFeatureSchemaCollection::Create(NULL);
        FdoPtr<FdoFeatureSchema> sch = FdoFeatureSchema::Create(L"Default", L"");
        pSchema->Add(sch);
        FdoPtr<FdoClassCollection> clss = sch->GetClasses();
        for(int i = 0; i < classNames->GetCount(); i++)
        {
            FdoPtr<FdoIdentifier> idClass = FdoIdentifier::Create(classNames->GetString(i));
            std::string clsName = W2A_SLOW(idClass->GetName());
            SltMetadata* md = GetMetadata(clsName.c_str());
            FdoPtr<FdoClassDefinition> fc = (md) ? md->ToClass() : NULL;
            if (fc)
            {
                FdoPtr<FdoClassDefinition> fc_copy = FdoCommonSchemaUtil::DeepCopyFdoClassDefinition(fc);
                clss->Add(fc_copy);
            }
            else
                throw FdoException::Create(L"Failed to get all tables that can be FDO feature classes.");
        }
        return FDO_SAFE_ADDREF(pSchema.p);
    }

    if (m_pSchema)
    {
        //We need to clone the schema, because the caller may modify it and mess us up.
        return makeACopy ? FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(m_pSchema, NULL) : FDO_SAFE_ADDREF(m_pSchema);
    }

    if (!m_dbWrite)
        return NULL;

    m_pSchema = FdoFeatureSchemaCollection::Create(NULL);
    FdoPtr<FdoFeatureSchema> schema = FdoFeatureSchema::Create(L"Default", L"");
    m_pSchema->Add(schema.p);
    
    FdoPtr<FdoClassCollection> classes = schema->GetClasses();
    
    //first, make a list of all tables that can be FDO feature classes
    std::vector<std::string> tables;

    const char* tables_sql = "SELECT name FROM sqlite_master WHERE type=? ORDER BY name;";
    sqlite3_stmt* pstmt = NULL;
    const char* pzTail = NULL;
    int rc;
    if ((rc = sqlite3_prepare_v2(m_dbWrite, tables_sql, -1, &pstmt, &pzTail)) == SQLITE_OK)
    {
        sqlite3_bind_text(pstmt, 1, "table", 5, SQLITE_STATIC);
        while (sqlite3_step(pstmt) == SQLITE_ROW)
            tables.push_back((const char*)sqlite3_column_text(pstmt, 0));
    }
    else
    {
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);
        else
            throw FdoException::Create(L"Failed to get all tables that can be FDO feature classes.", rc);
    }

    sqlite3_reset(pstmt);
    sqlite3_bind_text(pstmt, 1, "view", 4, SQLITE_STATIC);
    while (sqlite3_step(pstmt) == SQLITE_ROW)
        tables.push_back((const char*)sqlite3_column_text(pstmt, 0));

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

    //We need to clone the schema, because the caller may modify it and mess us up.
    return makeACopy ? FdoCommonSchemaUtil::DeepCopyFdoFeatureSchemas(m_pSchema, NULL) : FDO_SAFE_ADDREF(m_pSchema);
}

FdoISpatialContextReader* SltConnection::GetSpatialContexts()
{
    return new SltSpatialContextReader(this);
}

SltReader* SltConnection::Select(FdoIdentifier* fcname, 
                                 FdoFilter* filter, 
                                 FdoIdentifierCollection* props, 
                                 bool scrollable, 
                                 const std::vector<NameOrderingPair>& ordering,
                                 FdoParameterValueCollection*  parmValues)
{
    if (m_connState != FdoConnectionState_Open)
        throw FdoCommandException::Create(L"Connection must be open in order to Select.");

    const wchar_t* wfc = fcname->GetName();
    size_t wlen = wcslen(wfc);
    size_t clen = 4 * wlen+1;
    char* mbfc = (char*)alloca(4*wlen+1);
    W2A_FAST(mbfc, clen, wfc, wlen);
    StringBuffer sbfcn;
    char* mbfcname = mbfc;

    DBounds bbox;
    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    bool canFastStep = true;
    bool mustKeepFilterAlive = false;

    const wchar_t* idClassProp = L"rowid";
    SltMetadata* md = GetMetadata(mbfc);
    if (md == NULL)
    {
        std::wstring errVal(L"Class '");
        errVal.append(wfc);
        errVal.append(L"' is not found");
        throw FdoException::Create(errVal.c_str(), 1);
    }
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    if (md->IsView())
    {
        if (md->GetIdName() == NULL)
        {
            CacheViewContent(mbfc);
            sbfcn.Reset();
            sbfcn.Append("$view", 5);
            sbfcn.Append(wfc);
            mbfc = sbfcn.Data();
        }
        else
            idClassProp = md->GetIdName();
    }

    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
        canFastStep = qt.CanUseFastStepping();
        mustKeepFilterAlive = qt.MustKeepFilterAlive();
    }

    SpatialIterator* siter = NULL;
    RowidIterator* ri = NULL;


    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids && ordering.empty())
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfcname);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            siter = new SpatialIterator(bbox, si);
        }
        else
        {
            // enforce an empty result since result will be empty
            rowids = new std::vector<__int64>();
            ri = new RowidIterator(-1, rowids);
        }
    }

    //Now process any ordering options .
    //Our strategy here is to perform any ordering without evaluating the where
    //clause. Then we will pass the ordered list of IDs to the feature reader
    //which will process the where clause (filter) in the same way regardless of ordering,
    //or at least we hope it does.
    //This approach may or may not be slower, it needs some experimentation to see if
    //it's better to also perform the where clause here.

    if (!ordering.empty())
    {
        StringBuffer sb;
        sb.Append("SELECT ", 7);
        sb.AppendDQuoted(idClassProp);
        sb.Append(" FROM ", 6);
        sb.AppendDQuoted(mbfc);
        sb.Append(" ORDER BY ", 10);

        SltExtractExpressionTranslator exTrans(props);
        for (size_t i=0; i<ordering.size(); i++)
        {
            if (i)
                sb.Append(",", 1);

            // identifiers for order are provided as identifiers even are calculations.
            // so in order to get the "expression" we need to look for them in the select list
            FdoIdentifier* idfto = ordering[i].name;
            FdoPtr<FdoIdentifier> idfadd;
            if (props != NULL)
            {
                idfadd = props->FindItem(idfto->GetName());
                if (idfadd != NULL)
                {
                    idfadd->Process(&exTrans);
                    StringBuffer* exp = exTrans.GetExpression();
                    sb.Append(exp->Data(), exp->Length());
                    exTrans.Reset();
                }
            }
            // in case it's a property not present in selection list just add it
            if (idfadd == NULL)
                sb.Append(idfto->ToString());

            if (ordering[i].option == FdoOrderingOption_Ascending)
                sb.Append(" ASC", 4);
            else
                sb.Append(" DESC", 5);
        }

        sb.Append(";", 1);

        sqlite3_stmt* stmt = NULL;
        const char* tail = NULL;
        VectorMF* rows = new VectorMF(); //accumulate ordered row ids here

        int rc = sqlite3_prepare_v2(m_dbWrite, sb.Data(), -1, &stmt, &tail);

        //If there is also a spatial filter, we need to compute the intersection
        //of the ordering with the spatial filter -- the SltReader cannot handle both
        //a row id iterator and a spatial iterator, so we have to precompute this
        //and pass it the intersected row id iterator.
        
        //First, read the spatial iterator fully -- it will return results in sorted order
        if (siter)
        {
            std::vector<FdoInt64> srows;
            int start = -1;
            int end = -1;

            while (siter->NextRange(start, end))
            {
                for (int i=start; i<end; i++)
                    srows.push_back((*siter)[i]);
            }

            //Second, we will check for each result of the ordering query, if
            //it exists in the spatial query results, and add it to a new list 
            //if it does

            // we need to get all rows
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
            {
                __int64 id = sqlite3_column_int64(stmt, 0);
                //TODO: resolve rowid type issue (__int64 vs. int)
                if (std::binary_search(srows.begin(), srows.end(), (int)id))
                    rows->push_back(id);
            }

            //get rid of the spatial iterator -- we are done with it here
            delete siter;
            siter = NULL;
        }
        else
        {
            // we need to get all rows
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
                rows->push_back(sqlite3_column_int64(stmt, 0));
        }
        rc = sqlite3_finalize(stmt);

        ri = new RowidIterator(-1, rows);

        if (strWhere.Length()>0 && scrollable)
        {
            FdoPtr<FdoIdentifierCollection> collidf = FdoIdentifierCollection::Create();
            FdoPtr<FdoIdentifier> rowIdIdf = FdoIdentifier::Create(idClassProp);
            collidf->Add(rowIdIdf);
            SltReader* rdrSc = new SltReader(this, collidf, mbfcname, strWhere.Data(), siter, canFastStep, ri, parmValues);
            ri = GetScrollableIterator(rdrSc);
            delete rdrSc;
            siter = NULL;
            strWhere.Reset();
            canFastStep = true;
        }
    }
    else if (scrollable) //if we want the reader to be scrollable without ordering, we also need a rowid iterator
    {
        if (strWhere.Length()>0 && scrollable)
        {
            FdoPtr<FdoIdentifierCollection> collidf = FdoIdentifierCollection::Create();
            FdoPtr<FdoIdentifier> rowIdIdf = FdoIdentifier::Create(idClassProp);
            collidf->Add(rowIdIdf);
            SltReader* rdrSc = new SltReader(this, collidf, mbfcname, strWhere.Data(), siter, canFastStep, ri, parmValues);
            ri = GetScrollableIterator(rdrSc);
            delete rdrSc;
            siter = NULL;
            strWhere.Reset();
            canFastStep = true;
        }
        else
        {
            //again, if we also have a spatial iterator, we need
            //to eliminate it from the picture by intersecting the 
            //row list with the spatial result list
            if (siter)
            {
                VectorMF* rows = new VectorMF();
                int start = -1;
                int end = -1;

                while (siter->NextRange(start, end))
                {
                    for (int i=start; i<end; i++)
                        rows->push_back((*siter)[i]);
                }

                delete siter;
                siter = NULL;

                ri = new RowidIterator(-1, rows);
            }
            else if (!ri)
            {
                // TODO: can we find a different way to get all items in scrollable reader?
                // Issues to solve without this change: data store with 3 rows and max count=100
                // since the other 97 rows were deleted. The returned count is 100, so Move to will not work.
                // in case we run Count(rowid) and set the count will not work either since we could have:
                // Feat1Id=40, Feat2Id=50 and Feat3Id=100, how can we implement move to, e.g MoveTo(2)!?
                FdoPtr<FdoIdentifierCollection> collidf = FdoIdentifierCollection::Create();
                FdoPtr<FdoIdentifier> rowIdIdf = FdoIdentifier::Create(idClassProp);
                collidf->Add(rowIdIdf);
                SltReader* rdrSc = new SltReader(this, collidf, mbfcname, "", NULL, canFastStep, NULL, NULL);
                ri = GetScrollableIterator(rdrSc);
                delete rdrSc;
                canFastStep = true;
            }
        }
    }
    SltReader* rdr = new SltReader(this, props, mbfcname, strWhere.Data(), siter, canFastStep, ri, parmValues);
    if (mustKeepFilterAlive)
        rdr->SetInternalFilter(filter);

    return rdr;
}

RowidIterator* SltConnection::GetScrollableIterator(SltReader* rdr)
{
    VectorMF* list = new VectorMF();
    while (rdr->ReadNext())
    {
        //With a default SltReader we know that rowID will
        //be returned in column 0 and geometry in column 1
        //Accessing by index is significantly faster than
        //accessing by name
        FdoInt64 id = rdr->GetInt64((int)0);
        list->push_back(id);
    }
    rdr->Close();
    return new RowidIterator(-1, list);
}


FdoIDataReader* SltConnection::SelectAggregates(FdoIdentifier*              fcname, 
                                                FdoIdentifierCollection*    properties,
                                                bool                        bDistinct,
                                                FdoFilter*                  filter,
                                                FdoOrderingOption           eOrderingOption,
                                                FdoIdentifierCollection*    ordering,
                                                FdoFilter*                  grFilter,
                                                FdoIdentifierCollection*    grouping,
                                                FdoParameterValueCollection*  parmValues)
{
    const wchar_t* wfc = fcname->GetName();
    StringBuffer sbfcn;
    sbfcn.Append(wfc);
    char* mbfc = sbfcn.Data();
    SltMetadata* md = GetMetadata(mbfc);
    if (md == NULL)
    {
        std::wstring errVal(L"Class '");
        errVal.append(wfc);
        errVal.append(L"' is not found");
        throw FdoException::Create(errVal.c_str(), 1);
    }
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    if (md->IsView() && md->GetIdName() == NULL)
    {
        CacheViewContent(mbfc);
        sbfcn.Reset();
        sbfcn.Append("$view", 5);
        sbfcn.Append(wfc);
        mbfc = sbfcn.Data();
    }
    
    StringBuffer sb;
    SltExpressionTranslator exTrans(properties);
    int propsCount = properties->GetCount();
    
    if (!bDistinct && fc->GetClassType() == FdoClassType_FeatureClass && (propsCount == 1 || propsCount == 2))
    {
        SltReader* rdr = CheckForSpatialExtents(properties, (FdoFeatureClass*)fc.p, filter);

        if (rdr)
            return rdr;
    }

    sb.Append("SELECT ", 7);
    if (bDistinct)
        sb.Append("DISTINCT ", 9);
        
    for (int i=0; i<propsCount; i++)
    {
        if (i)
            sb.Append(",", 1);

        FdoPtr<FdoIdentifier> identifier = properties->GetItem(i);
        exTrans.Reset();
        identifier->Process(&exTrans);
        StringBuffer* exp = exTrans.GetExpression();
        sb.Append(exp->Data(), exp->Length());
    }
    if (propsCount == 0)
        sb.Append("* FROM ", 7);
    else
        sb.Append(" FROM ", 6);
    sb.AppendDQuoted(mbfc);

    bool mustKeepFilterAlive = false;
    if(filter)
    {
        SltQueryTranslator qt(fc);
        filter->Process(&qt);
        mustKeepFilterAlive = qt.MustKeepFilterAlive();
        qt.Reset(); // avoid optimize the filter

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter)
        {
            sb.Append(" WHERE ", 7);
            sb.Append(txtFilter);
        }
    }
    propsCount = grouping->GetCount();
    if (propsCount)
    {
        sb.Append(" GROUP BY ", 10);
        for (int i=0; i<propsCount; i++)
        {
            if (i)
                sb.Append(",", 1);

            FdoPtr<FdoIdentifier> identifier = grouping->GetItem(i);
            exTrans.Reset();
            identifier->Process(&exTrans);
            StringBuffer* exp = exTrans.GetExpression();
            sb.Append(exp->Data(), exp->Length());
        }
        if (grFilter)
        {
            SltQueryTranslator qt(fc);
            grFilter->Process(&qt);
            qt.Reset(); // avoid optimize the filter

            const char* txtFilter = qt.GetFilter();
            if (*txtFilter)
            {
                sb.Append(" HAVING ", 8);
                sb.Append(txtFilter);
            }
        }
    }
    propsCount = ordering->GetCount();
    if (propsCount)
    {
        SltExtractExpressionTranslator exTransEx(properties);
        sb.Append(" ORDER BY ", 10);
        for (int i=0; i<propsCount; i++)
        {
            if (i)
                sb.Append(",", 1);

            FdoPtr<FdoIdentifier> identifier = ordering->GetItem(i);
            identifier->Process(&exTransEx);
            StringBuffer* exp = exTransEx.GetExpression();
            sb.Append(exp->Data(), exp->Length());
            exTransEx.Reset();
        }
        if (eOrderingOption == FdoOrderingOption_Ascending)
            sb.Append(" ASC", 4);
        else
            sb.Append(" DESC", 5);
    }
    sb.Append(";", 1);

    SltReader* rdr = new SltReader(this, sb.Data(), parmValues);
    if (mustKeepFilterAlive)
        rdr->SetInternalFilter(filter);

    return rdr;
}


FdoInt32 SltConnection::Update(FdoIdentifier* fcname, FdoFilter* filter, 
                               FdoPropertyValueCollection* propvals,
                               FdoParameterValueCollection*  parmValues )
{
    StringBuffer sb;
    bool geomPropIsUpdated = false;
    int geomFormat = eFGF;

    const wchar_t* wfc = fcname->GetName();
    size_t wlen = wcslen(wfc);
    size_t clen = 4 * wlen+1;
    char* mbfc = (char*)alloca(4*wlen+1);
    W2A_FAST(mbfc, clen, wfc, wlen);
    SltMetadata* md = GetMetadata(mbfc);
    if (md == NULL)
    {
        std::wstring errVal(L"Class '");
        errVal.append(wfc);
        errVal.append(L"' is not found");
        throw FdoException::Create(errVal.c_str(), 1);
    }
    geomFormat = md->GetGeomFormat();    
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    sb.Append("UPDATE ", 7);
    // don't update views
    if (md->IsView())
    {
        // views well defined can be modified
        if (md->GetIdName() == NULL)
            throw FdoException::Create(L"Views cannot be updated");
        sb.AppendDQuoted(md->GetMainViewTable());
        md = GetMetadata(md->GetMainViewTable());
        fc = (md) ? md->ToClass() : NULL;
        if (fc == NULL)
        {
            std::wstring errVal(L"Class '");
            errVal.append(wfc);
            errVal.append(L"' is not found");
            throw FdoException::Create(errVal.c_str(), 1);
        }
        // TODO: in case is needed handle filter with secondary properties
        // now we just support filters for update with primary properties
        // we need to select PK from view using the filter create a rowid list
        // and pass RowidIterator* to the update statement.
    }
    else // we have a table
        sb.AppendDQuoted(fcname->GetName());

    sb.Append(" SET ", 5);

    FdoString* geomPropName = NULL;
    if (fc->GetClassType() == FdoClassType_FeatureClass)
    {
        FdoFeatureClass* fcl = static_cast<FdoFeatureClass*>(fc.p);
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = fcl->GetGeometryProperty();
        geomPropName = geomProp->GetName();
    }
    
    for (int i=0; i<propvals->GetCount(); i++)
    {
        FdoPtr<FdoPropertyValue> pv = propvals->GetItem(i);
        FdoPtr<FdoIdentifier> id = pv->GetName();
        FdoString* propName = id->GetName();

        if (i)
            sb.Append(",", 1); 

        if (geomPropName != NULL && wcscmp(geomPropName, propName) == 0)
            geomPropIsUpdated = true;

        sb.AppendDQuoted(propName);
        sb.Append("=?", 2);
    }

    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    DBounds bbox;
    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
    }

    RowidIterator* ri = NULL;
    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids)
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfc);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            SpatialIterator* siter = new SpatialIterator(bbox, si);
            VectorMF* mfrowids = new VectorMF();
            int start = -1;
            int end = -1;

            while (siter->NextRange(start, end))
            {
                for (int i=start; i<end; i++)
                    mfrowids->push_back((*siter)[i]);
            }
            delete siter;
            if (mfrowids->size() == 0)
            {
                delete mfrowids;
                return 0;
            }
            else
                ri = new RowidIterator(-1, mfrowids);
        }
        else
            return 0; // enforce an empty result since result will be empty
    }

    if (!strWhere.Length())
    {
        if (ri)
            sb.Append(" WHERE ROWID=?;", 15);
        else
            sb.Append(";", 1);
    }
    else
    {
        sb.Append(" WHERE ", 7);

        if (ri)
            sb.Append("ROWID=? AND ", 12);

        sb.Append("(", 1);
        sb.Append(strWhere.Data(), strWhere.Length());
        sb.Append(");", 2);
    }

    const char* tail = NULL;
    sqlite3_stmt* stmt = NULL;
    
    FdoInt64 ret = 0;
    
    int rc = sqlite3_prepare_v2(m_dbWrite, sb.Data(), -1, &stmt, &tail);
    if (rc == SQLITE_OK)
    {
        if (geomPropIsUpdated)
            EnableHooks(true);

        if (!ri)
        {
            BindPropVals(propvals, stmt, geomFormat);
            if( parmValues != NULL )
                BindPropVals(parmValues, stmt, true, geomFormat);

            if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
            {
                sqlite3_finalize(stmt);
                if (geomPropIsUpdated)
                    EnableHooks(false, true);

                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                else
                    throw FdoCommandException::Create(L"Failed to execute update statement.", rc);                
            }
            ret += sqlite3_changes(m_dbWrite);
        }
        else
        {
            ri->MoveToIndex(0);
            int cntProps = propvals->GetCount();
            while(ri->Next())
            {
                BindPropVals(propvals, stmt, geomFormat);
                sqlite3_bind_int64(stmt, cntProps+1, ri->CurrentRowid());
                
                if( parmValues != NULL )
                    BindPropVals(parmValues, stmt, true, geomFormat );

                if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
                {
                    sqlite3_finalize(stmt);
                    if (ri)
                        delete ri;
                    if (geomPropIsUpdated)
                        EnableHooks(false, true);

                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                else                    
                    throw FdoCommandException::Create(L"Failed to execute update statement.", rc);
                }
                ret += sqlite3_changes(m_dbWrite);
                sqlite3_reset(stmt);
            }
        }
        if (ri)
            delete ri;
        sqlite3_finalize(stmt);
        if (geomPropIsUpdated)
            EnableHooks(false);
    }
    else
    {
        if (ri)
            delete ri;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
        else
        {
            std::wstring err = L"Failed to parse: " + A2W_SLOW(sb.Data());
            throw FdoCommandException::Create(err.c_str(), rc);
        }
    }
    return (FdoInt32)ret;
}

FdoInt32 SltConnection::Delete(FdoIdentifier* fcname, FdoFilter* filter, FdoParameterValueCollection*  parmValues)
{
    StringBuffer sb;

    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    DBounds bbox;
    char* mbfc = NULL;
    const wchar_t* wfc = fcname->GetName();
    size_t wlen = wcslen(wfc);
    size_t clen = 4 * wlen+1;
    mbfc = (char*)alloca(4*wlen+1);
    W2A_FAST(mbfc, clen, wfc, wlen);
    SltMetadata* md = GetMetadata(mbfc);
    if (md == NULL)
    {
        std::wstring errVal(L"Class '");
        errVal.append(wfc);
        errVal.append(L"' is not found");
        throw FdoException::Create(errVal.c_str(), 1);
    }
    FdoPtr<FdoClassDefinition> fc = md->ToClass();
    sb.Append("DELETE FROM ", 12);
    // don't update views
    if (md->IsView())
    {
        // views well defined can be modified
        if (md->GetIdName() == NULL)
            throw FdoException::Create(L"Views cannot be updated");
        sb.AppendDQuoted(md->GetMainViewTable());
        md = GetMetadata(md->GetMainViewTable());
        if (md == NULL)
        {
            std::wstring errVal(L"Class '");
            errVal.append(wfc);
            errVal.append(L"' is not found");
            throw FdoException::Create(errVal.c_str(), 1);
        }
        fc = md->ToClass();
        // TODO: in case is needed handle filter with secondary properties
        // now we just support filters for delete with primary properties
        // we need to select PK from view using the filter create a rowid list
        // and pass RowidIterator* to the delete statement.
    }
    else // we have a table
        sb.AppendDQuoted(fcname->GetName());

    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {

        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
    }

    RowidIterator* ri = NULL;
    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids)
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfc);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            SpatialIterator* siter = new SpatialIterator(bbox, si);
            VectorMF* mfrowids = new VectorMF();
            int start = -1;
            int end = -1;

            while (siter->NextRange(start, end))
            {
                for (int i=start; i<end; i++)
                    mfrowids->push_back((*siter)[i]);
            }
            delete siter;
            if (mfrowids->size() == 0)
            {
                delete mfrowids;
                return 0;
            }
            else
                ri = new RowidIterator(-1, mfrowids);
        }
        else
            return 0; // enforce an empty result since result will be empty
    }

    if (!strWhere.Length())
    {
        if (ri)
            sb.Append(" WHERE ROWID=?;", 15);
        else
            sb.Append(";", 1);
    }
    else
    {
        sb.Append(" WHERE ", 7);

        if (ri)
            sb.Append("ROWID=? AND ", 12);

        sb.Append("(", 1);
        sb.Append(strWhere.Data(), strWhere.Length());
        sb.Append(");", 2);
    }

    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
    int rc;

    FdoInt64 ret = 0;
    if ((rc = sqlite3_prepare_v2(m_dbWrite, sb.Data(), -1, &stmt, &tail)) == SQLITE_OK)
    {
        EnableHooks(true);
        if (!ri)
        { 
            if( parmValues != NULL )
                BindPropVals(parmValues, stmt,true, eFGF /* not necessary to be correct in case of delete */);

            if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
            {
                const char* err = sqlite3_errmsg(m_dbWrite);
                FdoException* excThr = NULL;
                if (err != NULL)
                    excThr = FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                else
                    excThr = FdoCommandException::Create(L"Failed to execute delete statement.", rc);

                sqlite3_finalize(stmt);
                EnableHooks(false, true);
                throw excThr;
            }
            ret += sqlite3_changes(m_dbWrite);
        }
        else
        {
            ri->MoveToIndex(0);
            while(ri->Next())
            {
                sqlite3_bind_int64(stmt, 1, ri->CurrentRowid());
                
                if( parmValues != NULL )
                    BindPropVals(parmValues, stmt, true, eFGF /* not necessary to be correct in case of delete */ );

                if ((rc = sqlite3_step(stmt)) != SQLITE_DONE)
                {
                    const char* err = sqlite3_errmsg(m_dbWrite);
                    FdoException* excThr = NULL;
                    if (err != NULL)
                        excThr = FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
                    else
                        excThr = FdoCommandException::Create(L"Failed to execute delete statement.", rc);
                    sqlite3_finalize(stmt);
                    if (ri)
                        delete ri;

                    EnableHooks(false, true);
                    throw excThr;
                }
                ret += sqlite3_changes(m_dbWrite);
                sqlite3_reset(stmt);
            }
        }
        if (ri)
            delete ri;
        sqlite3_finalize(stmt);
        EnableHooks(false);
    }
    else
    {
        if (ri)
            delete ri;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            throw FdoCommandException::Create(A2W_SLOW(err).c_str(), rc);
        else
        {
            std::wstring err = L"Failed to parse: " + A2W_SLOW(sb.Data());
            throw FdoCommandException::Create(err.c_str(), rc);
        }
    }
    return (FdoInt32)ret;
}

SltMetadata* SltConnection::GetMetadata(const char* table)
{
    SltMetadata* ret = NULL;

    MetadataCache::iterator iter = m_mNameToMetadata.find((char*)table);
    
    if (iter == m_mNameToMetadata.end())
    {
        if (!IsMetadataTable(table))
        {
            ret = new SltMetadata(this, table, m_bUseFdoMetadata && m_bHasFdoMetadata);
            FdoPtr<FdoClassDefinition> fc = ret->ToClass();

            //Check if it failed to find the corresponding database table
            if (fc == NULL)
            {
                delete ret;
                ret = NULL;
            }
        }

        m_mNameToMetadata[_strdup(table)] = ret; //Note the memory allocation here
    }
    else 
    {
        ret = iter->second;
    }

    return ret;
}

void SltConnection::GetGeometryExtent(const unsigned char* ptr, int len, DBounds* ext)
{
    if (ptr != NULL && len != 0)
    {
        if (*ptr == 1 && *(ptr+1) != 0) // is WKB
        {
            int estFGFLen = len * 2; //should be sufficient, heh
            if (m_wkbBufferLen < estFGFLen)
            {
                delete[] m_wkbBuffer;
                m_wkbBufferLen = estFGFLen;
                m_wkbBuffer = new unsigned char[m_wkbBufferLen];
            }
            len = Wkb2Fgf(ptr, m_wkbBuffer);

            GetFgfExtents(m_wkbBuffer, len, (double*)ext);
        }
        else if (*ptr != 0 && *(ptr+1) == 0) // is FGF
        {
            GetFgfExtents(ptr, len, (double*)ext);
        }
    }
}

SpatialIndex* SltConnection::GetSpatialIndex(const char* table)
{
    SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.find((char*)table);

    SpatialIndex* si = NULL;
    SpatialIndexDescriptor* spDesc = NULL;
    SltReader* rdr = NULL;
    DBounds ext;

    if (iter != m_mNameToSpatialIndex.end())
    {
        spDesc = iter->second;
        si = spDesc->GetSpatialIndex();
        // don't auto-update the SI for views we will need to find a different way to do that
        if (spDesc->IsView())
            return si;
        FdoInt64 id = si->GetLastInsertedIdx();
        FdoInt64 lastCnt = spDesc->GetFeatureCount();
        if (id < lastCnt)
        {
            // in this case is faster to not use a reader SltReader since we need limited
            // functionality to get the id and the geometry
            sqlite3_stmt* stmt = spDesc->GetNewFeatures(id);
            if (stmt != NULL)
            {
                FdoPtr<FdoFgfGeometryFactory> gf = FdoFgfGeometryFactory::GetInstance();
                FdoPtr<FdoIGeometry> fg;
                while(sqlite3_step(stmt) == SQLITE_ROW)
                {
                    FdoInt64 idToAdd = sqlite3_column_int64(stmt, 0);
	                const unsigned char* ptr = (const unsigned char*)sqlite3_column_blob(stmt, 1);
	                int len = sqlite3_column_bytes(stmt, 1);
                    GetGeometryExtent(ptr, len, &ext);
                    if (!ext.IsEmpty())
                        si->Insert((unsigned int)idToAdd, ext);
                }
            }
        }
        // in case we have outstanding changes commit them
        if (spDesc->ChangesAvailable())
            spDesc->CommitChanges();
        return si;
    }

#if 0
    clock_t t0 = clock();
    for (int i=0; i<1000; i++)
    {
    delete si;
#endif
    //build the spatial index, if this is the first time it
    //is needed
    bool autodel = true;
    // lets create the description in case we get an exception we don't leak
    SltMetadata* md = GetMetadata(table);
    if(md == NULL || md->IsView())
    {
        if(md != NULL && md->GetIdName() != NULL)
        {
            si = GetSpatialIndex(md->GetMainViewTable());
            autodel = (si==NULL);
        }
        if (autodel)
            si = new SpatialIndex(NULL);

        spDesc = new SpatialIndexDescriptor(si, autodel);
    }
    else
    {
        si = new SpatialIndex(NULL);
        spDesc = new SpatialIndexDescriptor(this, table, si);
    }
    m_mNameToSpatialIndex[_strdup(table)] = spDesc; //Note the memory allocation

    // SI already built return it
    if (!autodel)
        return si;

    SpatialIndexMaxRecordInfo* imax = NULL;
    MaxRecordInfoCache::iterator itmx = m_mTableRecInfo.find((char*)table);
    if (itmx == m_mTableRecInfo.end())
    {
        imax = new SpatialIndexMaxRecordInfo(spDesc);
        m_mTableRecInfo[_strdup(table)] = imax; //Note the memory allocation
    }
    else
    {
        imax = itmx->second;
        imax->SetSpatialIndexDescriptor(spDesc);// ensure we have the right SI
    }
    spDesc->SetMaxRecordInfo(imax);

    //we will need the ID and the geometry when we build the spatial index, so add them to the query
    FdoPtr<FdoIdentifierCollection> idcol = FdoIdentifierCollection::Create();
    FdoPtr<FdoIdentifier> idid = FdoIdentifier::Create(L"rowid");
    idcol->Add(idid);
    FdoPtr<FdoIdentifier> idgeom = FdoIdentifier::Create(md->GetGeomName());
    idcol->Add(idgeom);

    rdr = new SltReader(this, idcol, table, "", NULL, true, NULL, NULL);
    FdoPtr<FdoIDataReader> rdrAutoDel = rdr; // in case of exception this smart ptr will delete the reader
    while (rdr->ReadNext())
    {
        int len = 0;

        //With a default SltReader we know that rowID will
        //be returned in column 0 and geometry in column 1
        //Accessing by index is significantly faster than
        //accessing by name
        FdoInt32 id = rdr->GetInt32(0);
        const FdoByte* geom = rdr->GetGeometry(1, &len);

        if (len)
        {
            //TODO: assumes DBounds is 2D since GetFgfExtents is 2D
            GetFgfExtents((unsigned char*)geom, len, (double*)&ext);

            si->Insert(id, ext);
        }
    }
    rdr->Close();
    si->ReOpen();
#if 0
    }
    clock_t t1 = clock();
    printf("Spatial index build time: %d\n", t1 - t0);
#endif

    return si;
}

void SltConnection::AddGeomCol(FdoGeometricPropertyDefinition* gpd, const wchar_t* fcname)
{
    StringBuffer sb;

    //add this geometry property to the geometry_columns table
    
    bool supDetGeom = SupportsDetailedGeomType();
    if(supDetGeom)
        sb.Append("INSERT INTO geometry_columns(f_table_name,f_geometry_column,geometry_format,geometry_type,geometry_dettype,coord_dimension,srid)VALUES(", 135);
    else
        sb.Append("INSERT INTO geometry_columns(f_table_name,f_geometry_column,geometry_format,geometry_type,coord_dimension,srid)VALUES(", 118);
    
    sb.AppendSQuoted(fcname);//f_table_name
    sb.Append(",", 1); 
    sb.AppendSQuoted(gpd->GetName());//f_geometry_column
    sb.Append(",'FGF',", 7); 

    int gtype = gpd->GetGeometryTypes();
    //if gdettype remains 0 at this points, it will be treated as "All" types
    //of geometry
    sb.Append(gtype);
    sb.Append(",", 1);

    int len = 0;
    int gdettype = 0;

    if (supDetGeom)
    {
        FdoGeometryType* gtypes = gpd->GetSpecificGeometryTypes(len);
        for (int idx = 0; idx < len; idx++)
        {
            if (*(gtypes + idx) == FdoGeometryType_None)
                continue;
            gdettype |= (0x01 << ((*(gtypes + idx)) - 1));
        }
        sb.Append(gdettype);
        sb.Append(",", 1);
    }

    int dim = 0x00;
    if (gpd->GetHasElevation())
        dim = 0x01;
    if (gpd->GetHasMeasure())
        dim |= 0x02;
    
    sb.Append(dim); //coord_dimension
    sb.Append(",", 1);
    
    //find a record in spatial_ref_sys whose sr_name matches
    //the name of the spatial context association.
    //Then, assign that record's SRID as the SRID for this
    //geometry table. This way we remove the FDO-idiosyncratic
    //spatial context name from the picture.
    sb.Append((int) FindSpatialContext(gpd->GetSpatialContextAssociation()));
    sb.Append(");", 2);

    int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
}


//Adds an entry for the given property definition to the FDO metadata table.
//Only done if the connection was opened with the "Use FDO metadata" flag set.
void SltConnection::AddDataCol(FdoDataPropertyDefinition* dpd, const wchar_t* fcname)
{
    if (!m_bUseFdoMetadata || !m_bHasFdoMetadata)
        return;

    StringBuffer sb;
    sb.Append("INSERT INTO fdo_columns (f_table_name,f_column_name,f_column_desc,fdo_data_type,fdo_data_details,fdo_data_length,fdo_data_precision,fdo_data_scale)VALUES(", 154);
    sb.AppendSQuoted(fcname);
    sb.Append(",", 1);
    sb.AppendSQuoted(dpd->GetName());
    sb.Append(",", 1);
    sb.AppendSQuotedHandleNull(dpd->GetDescription());
    sb.Append(",", 1);
    sb.Append((int)dpd->GetDataType());
    sb.Append(",", 1);
    //readonly = 0x02, system = 0x01
    sb.Append((int)((dpd->GetReadOnly() ? 0x02 : 0x00) |(dpd->GetIsSystem() ? 0x01 : 0x00)));
    sb.Append(",", 1);
    sb.Append((int)dpd->GetLength());
    sb.Append(",", 1);
    sb.Append((int)dpd->GetPrecision());
    sb.Append(",", 1);
    sb.Append((int)dpd->GetScale());
    sb.Append(");", 2);

    int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
}

//Returns the SRID of an entry in the spatial_ref_sys table,
//whose sr_name column matches the input.
int SltConnection::FindSpatialContext(const wchar_t* name, int valIfNotFound)
{
    // After apply schema a class can have a garbage CS name
    // in case we do not find it return the first CS SRID
    int retVal = 0; // invalid CS
    if (name)
    {
        std::string mbname = W2A_SLOW(name);

        //We will attempt two ways to get the SRID -- first
        //in case there is an sr_name column, we will get it by name.
        //Otherwise, we will interpret the given name as an SRID integer
        //and make sure it exists in the spatial_ref_sys table
        std::string sql1 = "SELECT srid FROM spatial_ref_sys WHERE sr_name='" + mbname + "';";
        std::string sql2 = "SELECT srid FROM spatial_ref_sys WHERE srid=" + mbname + ";";

        int rc;
        int ret = 0;
        sqlite3_stmt* stmt = NULL;
        const char* tail = NULL;
       
        if ((rc = sqlite3_prepare_v2(m_dbWrite, sql1.c_str(), -1, &stmt, &tail)) != SQLITE_OK)
            if ((rc = sqlite3_prepare_v2(m_dbWrite, sql2.c_str(), -1, &stmt, &tail)) != SQLITE_OK)
                return (!valIfNotFound) ? 0 : valIfNotFound;

        if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
            ret = sqlite3_column_int(stmt, 0);

        sqlite3_finalize(stmt);
    }
    // most of FDO providers have only one CS per schema
    // in case we have invalid CS name try getting the first one
    if (!retVal)
        retVal = (!valIfNotFound) ? GetDefaultSpatialContext() : valIfNotFound;        
    
    return retVal; //returns 0 if not found
}

int SltConnection::GetDefaultSpatialContext()
{
    int rc;
    sqlite3_stmt* stmt = NULL;
    const char* tail = NULL;
   
    if ((rc = sqlite3_prepare_v2(m_dbWrite, "SELECT srid FROM spatial_ref_sys;", -1, &stmt, &tail)) != SQLITE_OK)
        return m_defSpatialContextId;

    // avoid some lock issues in case there is a opened transaction tables can be locked 
    // till we commit since we make changes on tables, cache the value
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
        m_defSpatialContextId = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return m_defSpatialContextId; //returns 0 if not found
}

void SltConnection::DeleteClassFromSchema(FdoClassDefinition* fc)
{
    DeleteClassFromSchema(fc->GetName());
}

void SltConnection::DeleteClassFromSchema(const wchar_t* fcName)
{
    std::string table = W2A_SLOW(fcName);

    StringBuffer sb;
    sb.Append("DROP TRIGGER IF EXISTS \"OnInsert", 32);
    sb.Append(table.c_str());
    sb.Append("\";", 2);
    int rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);

    sb.Reset();
    sb.Append("DROP TABLE IF EXISTS ", 21);
    sb.AppendDQuoted(table.c_str());
    sb.Append(";", 1);
    rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    if (rc != SQLITE_OK)
    {
        FdoException* baseExc = NULL;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (rc == SQLITE_LOCKED)
        {
            std::wstring errorMsg = std::wstring(L"Class \'") + fcName + L"\' is locked (used) by other request(s): " + 
                ((err != NULL) ? A2W_SLOW(err) : std::wstring(L""));
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }
        else
        {
            std::wstring errorMsg = std::wstring(L"Failed to delete class \'") + fcName + L"\': " + 
                ((err != NULL) ? A2W_SLOW(err) : std::wstring(L""));
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }
    }
    sb.Reset();
    sb.Append("DELETE FROM geometry_columns WHERE f_table_name=", 48);
    sb.AppendSQuoted(table.c_str());
    sb.Append(";", 1);
    rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);

    if (m_bUseFdoMetadata)
    {
        sb.Reset();
        sb.Append("DELETE FROM fdo_columns WHERE f_table_name=", 43);
        sb.AppendSQuoted(table.c_str());
        sb.Append(";", 1);
        rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    }
    SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.find((char*)table.c_str());
    if (iter != m_mNameToSpatialIndex.end())
    {
         delete iter->second;
         free(iter->first); //was allocated using strdup
         m_mNameToSpatialIndex.erase(iter);
    }

    MaxRecordInfoCache::iterator itmx = m_mTableRecInfo.find((char*)table.c_str());
    if (itmx != m_mTableRecInfo.end())
    {
         if (itmx->second != NULL)
         {
             itmx->second->SetReleased(true);
             itmx->second->Release();
         }
         free(itmx->first); //was allocated using strdup
         m_mTableRecInfo.erase(itmx);
    }
}

SpatialIndexMaxRecordInfo* SltConnection::GetSpatialIndexMaxRecordInfo(const char* table)
{
    SpatialIndexMaxRecordInfo* retVal = NULL;
    MaxRecordInfoCache::iterator itmx = m_mTableRecInfo.find((char*)table);
    if (itmx == m_mTableRecInfo.end())
    {
        retVal = new SpatialIndexMaxRecordInfo();
        m_mTableRecInfo[_strdup(table)] = retVal; //Note the memory allocation
    }
    else
        retVal = itmx->second;
    return FDO_SAFE_ADDREF(retVal);
}

void SltConnection::AddClassToSchema(FdoClassCollection* classes, FdoClassDefinition* fc)
{
    std::string fcname = W2A_SLOW(fc->GetName());
    StringBuffer sb;
    sb.Append("CREATE TABLE ", 13);
    sb.AppendDQuoted(fcname.c_str());
    sb.Append(" (", 2);

    FdoPtr<FdoClassDefinition> fctmp = FDO_SAFE_ADDREF(fc);
    UniqueConstraints simpleUqc;
    FdoPtr<FdoUniqueConstraintCollection> complexUqc = FdoUniqueConstraintCollection::Create();
    
    int cntip = 0;
    while (fctmp != NULL)
    {
        FdoPtr<FdoDataPropertyDefinitionCollection> pdi = fctmp->GetIdentityProperties();
        if (pdi != NULL)
            cntip += pdi->GetCount();

        FdoPtr<FdoUniqueConstraintCollection> uqc = fctmp->GetUniqueConstraints();
        int cntConstr = uqc->GetCount();
        if (cntConstr != 0)
        {
            for (int i = 0; i < cntConstr; i++)
            {
                FdoPtr<FdoUniqueConstraint> uc = uqc->GetItem(i);
                FdoPtr<FdoDataPropertyDefinitionCollection> propsConstr = uc->GetProperties();
                int cntProps = propsConstr->GetCount();
                if (cntProps == 1)
                {
                    FdoPtr<FdoDataPropertyDefinition> propCu = propsConstr->GetItem(0);
                    simpleUqc[propCu->GetName()] = uc.p;
                }
                else if (cntProps > 1)
                    complexUqc->Add(uc);
            }
        }
        fctmp = fctmp->GetBaseClass();
    }

	size_t szInit = sb.Length();
    CollectBaseClassProperties(classes, fc, fc, sb, (cntip > 1) ? 0 : 1, simpleUqc);
    CollectBaseClassProperties(classes, fc, fc, sb, 2, simpleUqc);
    CollectBaseClassProperties(classes, fc, fc, sb, 3, simpleUqc);
    if (szInit == sb.Length())
		throw FdoException::Create(L"Classes without properties are not supported", 1);

    if (complexUqc->GetCount() != 0)
        AddComplexUniqueConstraints(complexUqc, fc,sb);

    if (cntip > 1)
        AddClassPrimaryKeys(fc, sb);

    //HACK -- remove a trailing comma and space added by CollectBaseClassProps
    //and replace by a closing of the SQL command
    sb.Data()[sb.Length()-2] = ')';
    sb.Data()[sb.Length()-1] = ';';

    //create the database table for this feature class
    int rc;
    if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
    {
        FdoException* baseExc = NULL;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
        std::wstring errorMsg = std::wstring(L"Failed to create class \'") + fc->GetName() + L"\'"; 
        throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
    }
    if (cntip > 1)
        GenerateAutoGenerateTrigger(fc);
}

void SltConnection::GenerateAutoGenerateTrigger(FdoClassDefinition* fc, bool dropTriggerFirst)
{
    FdoPtr<FdoDataPropertyDefinitionCollection> idtProps = fc->GetIdentityProperties();
    int cnt = idtProps->GetCount();
    if (cnt <= 1)
        return;
    std::string propName;
    for(int i = 0; i < cnt; i++)
    {
        FdoPtr<FdoDataPropertyDefinition> prop = idtProps->GetItem(i);
        FdoDataType dt = prop->GetDataType();
        if (prop->GetIsAutoGenerated() && (dt == FdoDataType_Int16 || dt == FdoDataType_Int32 || dt == FdoDataType_Int64))
        {
            propName = W2A_SLOW(prop->GetName());
            break;
        }
    }
    if (propName.size() == 0)
        return;

    int rc;
    StringBuffer sb;
    std::string fcname = W2A_SLOW(fc->GetName());

    if (dropTriggerFirst)
    {
        sb.Append("DROP TRIGGER IF EXISTS \"OnInsert", 32);
        sb.Append(fcname.c_str());
        sb.Append("\";", 2);
        sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    }

    sb.Reset();
    sb.Append("CREATE TRIGGER \"OnInsert", 24);
    sb.Append(fcname.c_str());
    sb.Append("\" AFTER INSERT ON ", 18);
    sb.AppendDQuoted(fcname.c_str());
    sb.Append(" \n BEGIN \n UPDATE ", 18);
    sb.AppendDQuoted(fcname.c_str());
    sb.Append(" SET ", 5);
    sb.AppendDQuoted(propName.c_str());
    sb.Append("=new.rowid WHERE rowid = new.rowid AND ", 39);
    sb.AppendDQuoted(propName.c_str());
    sb.Append(" IS NULL;\n END;", 15);

    if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
    {
        FdoException* baseExc = NULL;
        const char* err = sqlite3_errmsg(m_dbWrite);
        if (err != NULL)
            baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
        std::wstring errorMsg = std::wstring(L"Failed to create class \'") + fc->GetName() + L"\'"; 
        throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
    }
}

void SltConnection::UpdateClassFromSchema(FdoClassCollection* classes, FdoClassDefinition* fc, FdoClassDefinition* mainfc)
{
    FdoPtr<FdoClassDefinition> baseFc = fc->GetBaseClass();
    if (NULL != baseFc)
        UpdateClassFromSchema(classes, baseFc, mainfc);

    StringBuffer sb;
    // since we allow only to add fields when we have data
    // we can check only for new added properties.
    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    for (int i = 0; i < idpdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
        if (pd->GetElementState() != FdoSchemaElementState_Unchanged)
            throw FdoException::Create(L"PRIMARY KEYs or UNIQUE constraints cannot be added when class contains data");
    }
    // check if this altering is purely additive
    bool pureAdditive = true;
    for (int i = 0; pureAdditive && i < pdc->GetCount(); i++)
    {
        FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
        pureAdditive = (pd->GetElementState() == FdoSchemaElementState_Added || pd->GetElementState() == FdoSchemaElementState_Unchanged);
    }

    if(!pureAdditive) 
    {
        const std::wstring originalClassName = fc->GetName();    
        const std::wstring tempClassName     = originalClassName + L"_temp_copy";
        fc->SetName(tempClassName.c_str());

        // create temporary class (and table)
        AddClassToSchema(classes, fc);
        
        // copy relevant content of old into temporary table
        sb.Reset();
        sb.Append("INSERT INTO ", 12);
        sb.AppendDQuoted(tempClassName.c_str());
        sb.Append(" (", 2);

        bool first = true;
        for (int i = 0, iEnd = pdc->GetCount(); i<iEnd; i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            if( pd->GetElementState() != FdoSchemaElementState_Detached &&
                pd->GetElementState() != FdoSchemaElementState_Deleted
            ) {
                if(!first) {
                    sb.Append(", ", 2);            
                }
                sb.AppendDQuoted(pd->GetName());
                first = false;
            }
        }
        sb.Append(") SELECT ", 9);
        first = true;
        for (int i = 0, iEnd = pdc->GetCount(); i<iEnd; i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            if( pd->GetElementState() == FdoSchemaElementState_Unchanged ||
                pd->GetElementState() == FdoSchemaElementState_Modified // only renaming is allowed so far
            ) {
                if(!first) {
                    sb.Append(", ", 2);            
                }
                sb.AppendDQuoted(pd->GetName());
                first = false;                          
            }         
        }
        sb.Append(" FROM ", 6);
        sb.Append(originalClassName.c_str());
        sb.Append(";", 1);

        int rc;
        if (!first && (rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
        {
            FdoException* baseExc = NULL;
            const char* err = sqlite3_errmsg(m_dbWrite);
            if (err != NULL)
                baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
            std::wstring errorMsg = L"Failed to copy table content for class \'" + originalClassName + L"\'"; 
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }
        // drop old table
        DeleteClassFromSchema(originalClassName.c_str());
        // rename temporary table
        sb.Reset();
        sb.Append("ALTER TABLE ", 12);
        sb.Append(tempClassName.c_str());
        sb.Append(" RENAME TO ", 11);
        sb.Append(originalClassName.c_str());
        sb.Append(";", 1);
        
        if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
        {
            FdoException* baseExc = NULL;
            const char* err = sqlite3_errmsg(m_dbWrite);
            if (err != NULL)
                baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
            std::wstring errorMsg = L"Failed to rename temporary table for class \'" + originalClassName + L"\'"; 
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }
        // update table name column in meta tables 
        sb.Reset();
        sb.Append("UPDATE geometry_columns SET f_table_name=", 41);
        sb.AppendSQuoted(originalClassName.c_str());
        sb.Append(" WHERE f_table_name=", 20);
        sb.AppendSQuoted(tempClassName.c_str());
        sb.Append(";", 1);
        if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
        {
            FdoException* baseExc = NULL;
            const char* err = sqlite3_errmsg(m_dbWrite);
            if (err != NULL)
                baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
            std::wstring errorMsg = L"Failed to update geometry meta data table for class \'" + originalClassName + L"\'"; 
            throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
        }

        if (m_bUseFdoMetadata)
        {
            sb.Reset();
            sb.Append("UPDATE fdo_columns SET f_table_name=", 36);
            sb.AppendSQuoted(originalClassName.c_str());
            sb.Append(" WHERE f_table_name=", 20);
            sb.AppendSQuoted(tempClassName.c_str());
            sb.Append(";", 1);
            if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
            {
                FdoException* baseExc = NULL;
                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
                std::wstring errorMsg = L"Failed to update meta data table for class \'" + originalClassName + L"\'"; 
                throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
            }
        }
        // update spatial index
        std::string tempClassNameA = W2A_SLOW(tempClassName.c_str());
        std::string originalClassNameA = W2A_SLOW(originalClassName.c_str());
        SpatialIndexCache::iterator iter = m_mNameToSpatialIndex.find((char*) tempClassNameA.c_str());
        SpatialIndexDescriptor* desc = NULL;
        if (iter != m_mNameToSpatialIndex.end())
        {
             m_mNameToSpatialIndex.erase(iter);
             desc = new SpatialIndexDescriptor(this, originalClassNameA.c_str(), iter->second->DetachSpatialIndex());
             free(iter->first); //was allocated using strdup
             m_mNameToSpatialIndex[_strdup(originalClassNameA.c_str())] = desc; //Note the memory allocation
        }

        MaxRecordInfoCache::iterator itermx = m_mTableRecInfo.find((char*) tempClassNameA.c_str());
        if (itermx != m_mTableRecInfo.end())
        {
            itermx->second->SetSpatialIndexDescriptor(desc);
            m_mTableRecInfo.erase(itermx);
            free(itermx->first); //was allocated using strdup
            m_mTableRecInfo[_strdup(originalClassNameA.c_str())] = itermx->second; //Note the memory allocation
        }
        // reset class name
        fc->SetName(originalClassName.c_str());

    } 
    else 
    {
        for (int i = 0; i < pdc->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            // only new properties are processed
            if (pd->GetElementState() != FdoSchemaElementState_Added)
                continue;
            
            FdoPropertyType ptype = pd->GetPropertyType();
            sb.Reset();
            sb.Append("ALTER TABLE ", 12);
            sb.AppendDQuoted(fc->GetName());
            sb.Append(" ADD COLUMN ", 12);
            if (ptype == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)pd.p;
                
                if (dpd->GetIsAutoGenerated())
                {
                    FdoPtr<FdoDataPropertyDefinition> pkProp = idpdc->FindItem(dpd->GetName());
                    if (pkProp == NULL)
                        throw FdoException::Create(L"Non-primary key auto generated properties are not supported.", 1);
                }
                
                sb.AppendDQuoted(dpd->GetName());
                sb.Append(" ", 1);
                sb.Append(g_fdo2sql_map[dpd->GetDataType()].c_str());

                AddPropertyConstraintDefaultValue(dpd, sb);
                sb.Append(";", 1);
                AddDataCol(dpd, mainfc->GetName());
            }
            else if (ptype == FdoPropertyType_GeometricProperty)
            {
                sb.AppendDQuoted(pd->GetName());
                sb.Append(" BLOB, ", 7);

                AddGeomCol((FdoGeometricPropertyDefinition*)pd.p, mainfc->GetName());
            }
            else
                throw FdoException::Create(L"Invalid property type");
           
            int rc;
            if ((rc = sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL)) != SQLITE_OK)
            {
                FdoException* baseExc = NULL;
                const char* err = sqlite3_errmsg(m_dbWrite);
                if (err != NULL)
                    baseExc = FdoException::Create(A2W_SLOW(err).c_str(), rc);
                std::wstring errorMsg = std::wstring(L"Failed to apply schema for class \'") + fc->GetName() + L"\'"; 
                throw FdoException::Create(errorMsg.c_str(), baseExc, rc);
            }
        }
    }
}

void SltConnection::ApplySchema(FdoFeatureSchema* schema, bool ignoreStates)
{
    FdoPtr<FdoFeatureSchemaCollection> myschemac = DescribeSchema(NULL);
    FdoPtr<FdoFeatureSchema> myschema = myschemac->GetItem(0);
    FdoPtr<FdoClassCollection> oldclasses = myschema->GetClasses();
	FdoPtr<FdoFeatureSchema> userschema = FDO_SAFE_ADDREF(schema);

    FdoPtr<FdoFeatureSchema> mergedSchema;
    FdoPtr<SQLiteSchemaMergeContext> context;
    if (oldclasses->GetCount() != 0)
    {
		// an error will be thrown when user will take the current schema and change it (change the name)
		// and apply schema uning ignoreStates=true (SQLite does not support multimple schema).
		if (ignoreStates && _wcsicmp(schema->GetName(), L"Default") != 0)
		{
			// in case ignoreStates = true and schema name is different, make a copy, change the name
			// and run merge schema. We need to copy it since we cannot change user data!
			userschema = FdoCommonSchemaUtil::DeepCopyFdoFeatureSchema(schema, NULL);
		}
        context = SQLiteSchemaMergeContext::Create(this, myschemac, schema, ignoreStates);
        context->CommitSchemas();
        FdoFeatureSchemasP mergedSchemas = context->GetSchemas();
        mergedSchema = mergedSchemas->GetItem( myschema->GetName() );
    }
    else
        mergedSchema = userschema;

    FdoPtr<FdoClassCollection> classes = mergedSchema->GetClasses();

    // cache the default SpatialContext Id to avoid some locking issues
    GetDefaultSpatialContext();
    int rc = SQLITE_OK;
    bool needsCommit = false;
    switch(m_transactionState)
    {
    case SQLiteActiveTransactionType_None:
        rc = sqlite3_exec(m_dbWrite, "BEGIN;", NULL, NULL, NULL);
        if (rc == SQLITE_OK)
        {
            m_transactionState = SQLiteActiveTransactionType_User;
            needsCommit = true;
        }
        break;
    case SQLiteActiveTransactionType_Internal:
        CommitTransaction();
        rc = sqlite3_exec(m_dbWrite, "BEGIN;", NULL, NULL, NULL);
        if (rc == SQLITE_OK)
        {
            m_transactionState = SQLiteActiveTransactionType_User;
            needsCommit = true;
        }
        break;
    case SQLiteActiveTransactionType_User:
        break;
    }

    try
    {
        for (int i = 0; i < classes->GetCount(); i++)
        {
            bool changeMade = false;
            FdoPtr<FdoClassDefinition> fc = classes->GetItem(i);
            std::string table;
            if (!ignoreStates)
            {
                switch (fc->GetElementState())
                {
                case FdoSchemaElementState_Added:
                    AddClassToSchema(classes, fc);
                    changeMade = true;
                    break;
                case FdoSchemaElementState_Deleted:
                    DeleteClassFromSchema(fc);
                    changeMade = true;
                    break;
                case FdoSchemaElementState_Modified:
                    if ((NULL == context) || !context->TableHasObjects(fc->GetName()))
                    {
                        DeleteClassFromSchema(fc);
                        AddClassToSchema(classes, fc);
                    }
                    else
                        UpdateClassFromSchema(classes, fc, fc);
                    changeMade = true;
                    break;
                }
            }
            else
            {
                FdoPtr<FdoClassDefinition> oldfc = oldclasses->FindItem(fc->GetName());
                if (oldfc == NULL)
                    AddClassToSchema(classes, fc);
                else
                {
                    if (fc->GetElementState() == FdoSchemaElementState_Unchanged)
                        continue;
                    table = W2A_SLOW(fc->GetName());
                    if (GetFeatureCount(table.c_str()) <= 0)
                    {
                        DeleteClassFromSchema(fc);
                        AddClassToSchema(classes, fc);
                    }
                    else
                        UpdateClassFromSchema(classes, fc, fc);
                }
                changeMade = true;
            }
            if (changeMade)
            {
                // we need to erase the metadata to force a refresh
                if (table.size() == 0)
                    table = W2A_SLOW(fc->GetName());
                MetadataCache::iterator iter = m_mNameToMetadata.find((char*)table.c_str());
                if (iter != m_mNameToMetadata.end())
                {
                    delete iter->second;
                    free(iter->first); //it was created via strdup, must use free()
                    m_mNameToMetadata.erase(iter);
                }
            }
        }
    }
    catch(...)
    {
        RollbackTransaction(needsCommit);
        // The cached FDO schema will need to be refreshed
        FDO_SAFE_RELEASE(m_pSchema);
        m_pSchema = NULL;
        throw;
    }

    rc = CommitTransaction(needsCommit);
    if (rc == SQLITE_OK)
        schema->AcceptChanges();
    else // not sure we need to do that yet
        RollbackTransaction(needsCommit);
    
    // The cached FDO schema will need to be refreshed
    FDO_SAFE_RELEASE(m_pSchema);
    m_pSchema = NULL;
}

std::wstring SltConnection::GenerateValidConstrName(FdoString* name)
{
    std::wstring ret(name);
    int cnt = ret.size();
    for (int idx = 0; idx < cnt; idx++)
    {
        if (iswalnum(ret[idx]) == 0)
            ret[idx] = L'_';
    }
    return ret;
}

void SltConnection::AddPropertyUniqueConstraint(UniqueConstraints& simpleUniqueConstr, FdoDataPropertyDefinition* prop, StringBuffer& sb)
{
    UniqueConstraints::iterator it = simpleUniqueConstr.find(prop->GetName());
    if (it == simpleUniqueConstr.end())
        return;
    FdoUniqueConstraint* uqc = it->second;
    sb.Append(" CONSTRAINT UNQ_", 16);
    sb.Append(GenerateValidConstrName(prop->GetName()).c_str());
    sb.Append(" UNIQUE", 7);
    // remove it to avoid another search
    simpleUniqueConstr.erase(it);
}

void SltConnection::AddPropertyConstraintDefaultValue(FdoDataPropertyDefinition* prop, StringBuffer& sb)
{
    // add the default value
    FdoString* defVal = prop->GetDefaultValue();
    FdoDataType dt = prop->GetDataType();
    if (defVal != NULL && *defVal != '\0' && (dt != FdoDataType_String && _wcsicmp(defVal, L"null") == 0))
        defVal = NULL;

    FdoPtr<FdoPropertyValueConstraint> constr = prop->GetValueConstraint();
    if (!prop->GetNullable() && !prop->GetIsAutoGenerated())
        sb.Append(" NOT NULL ", 10);

    char dateBuff[31];
    *dateBuff = '\0';
    if(defVal != NULL && defVal[0] != '\0')
    {
        switch(dt)
        {
            case FdoDataType_String:
                if (constr == NULL || constr->Contains(FdoPtr<FdoStringValue>(FdoStringValue::Create(defVal))))
                {
                    sb.Append(" DEFAULT(", 9);
                    std::wstring strRez;
                    while(*defVal != L'\0')
                    {
                        if (*defVal == L'\'')
                            strRez.append(L"''");
                        else
                            strRez.append(defVal, 1);
                        defVal++;
                    }
                    sb.AppendSQuoted(strRez.c_str());
                    sb.Append(")", 1);
                }
            break;
            case FdoDataType_Boolean:
                {
                    FdoPtr<FdoDataValue> dv = SltMetadata::GenerateConstraintValue(dt, defVal);
                    if (constr == NULL || constr->Contains(dv))
                    {
                        FdoBooleanValue* pboolVal = static_cast<FdoBooleanValue*>(dv.p);
                        if (!pboolVal->IsNull())
                        {
                            sb.Append(" DEFAULT(", 9);
                            sb.Append(pboolVal->GetBoolean() ? "1" : "0", 1);
                            sb.Append(")", 1);
                        }
                    }
                }
            case FdoDataType_BLOB:
            case FdoDataType_CLOB:
                // nothing
            break;
            case FdoDataType_DateTime:
                {
                    int len = wcslen(defVal);
                    const wchar_t* defValtmp = defVal;
                    std::wstring wsVal;
                    if (len > 2)
                    {
                        if (*defVal == '\'' && *(defVal+len-1) == '\'')
                        {
                            wsVal = std::wstring(defVal+1, len-2);
                        }
                        else if (sqlite3UpperToLower[*defVal] == sqlite3UpperToLower['D'] || 
                            sqlite3UpperToLower[*defVal] == sqlite3UpperToLower['T'] && *(defVal+len-1) == '\'')
                        {
                            // eliminate "DATE '" or "TIME '" or "TIMESTAMP '" from the default value
                            while (*defValtmp != '\'' && *defValtmp != '\0' && (defValtmp-defVal)<15) defValtmp++;
                            if (*defValtmp == '\'')
                                wsVal = std::wstring(defValtmp+1, len-(defValtmp-defVal)-2);
                            else
                                wsVal = defVal;
                        }
                        else
                            wsVal = defVal;

                        FdoPtr<FdoDataValue> dVal = SltMetadata::GenerateConstraintValue(dt, wsVal.c_str());
                        // check if default values are ok in case we have a constraint
                        if((dVal != NULL && !dVal->IsNull()) && (constr == NULL || constr->Contains(dVal)))
                        {
                            sb.Append(" DEFAULT(", 9);
                            FdoDateTimeValue* dataValue = static_cast<FdoDateTimeValue*>(dVal.p);
                            FdoDateTime dtRet = dataValue->GetDateTime();
                            DateToString(&dtRet, dateBuff, 31);
                            sb.AppendSQuoted(dateBuff);
                            sb.Append(")", 1);
                        }
                    }
                }
            break;
            //FdoDataType_Byte FdoDataType_Decimal FdoDataType_Double FdoDataType_Int16 FdoDataType_Int32 FdoDataType_Int64 FdoDataType_Single
            default:
                // check if default values are ok in case we have a constraint
                if(constr == NULL || constr->Contains(FdoPtr<FdoDataValue>(SltMetadata::GenerateConstraintValue(dt, defVal))))
                {
                    sb.Append(" DEFAULT(", 9);
                    sb.Append(defVal);
                    sb.Append(")", 1);
                }
            break;
        }
    }

    if (constr == NULL)
        return;

    FdoString* propName = prop->GetName();

    if (dt == FdoDataType_DateTime)
    {
        *dateBuff = '\0';
        FdoDateTime dtRet;
        FdoDateTimeValue* dataValue;
        if (constr->GetConstraintType() == FdoPropertyValueConstraintType_Range)
        {
            FdoPropertyValueConstraintRange* rgConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
            FdoPtr<FdoDataValue> dataMin = rgConstr->GetMinValue();
            FdoPtr<FdoDataValue> dataMax = rgConstr->GetMaxValue();
            if((dataMin != NULL && dataMin->GetDataType() == FdoDataType_DateTime && !dataMin->IsNull()) ||
                (dataMax != NULL  && dataMax->GetDataType() == FdoDataType_DateTime && !dataMax->IsNull()))
            {
                sb.Append(" CONSTRAINT CHK_", 16);
                sb.Append(GenerateValidConstrName(propName).c_str());
                sb.Append(" CHECK(", 7);
                if ((dataMin != NULL && !dataMin->IsNull()) && (dataMax != NULL  && !dataMax->IsNull())
                    && rgConstr->GetMinInclusive() && rgConstr->GetMaxInclusive())
                {
                    sb.AppendDQuoted(propName);
                    sb.Append(" BETWEEN ", 9);
                    
                    dataValue = static_cast<FdoDateTimeValue*>(dataMin.p);
                    dtRet = dataValue->GetDateTime();
                    DateToString(&dtRet, dateBuff, 31);
                    sb.AppendSQuoted(dateBuff);
                    sb.Append(" AND ", 5);
                    dataValue = static_cast<FdoDateTimeValue*>(dataMax.p);
                    dtRet = dataValue->GetDateTime();
                    DateToString(&dtRet, dateBuff, 31);
                    sb.AppendSQuoted(dateBuff);
                }
                else
                {
                    if (dataMin != NULL && !dataMin->IsNull())
                    {
                        sb.AppendDQuoted(propName);
                        if(rgConstr->GetMinInclusive())
                            sb.Append(">=", 2);
                        else
                            sb.Append(">", 1);
                        dataValue = static_cast<FdoDateTimeValue*>(dataMin.p);
                        dtRet = dataValue->GetDateTime();
                        DateToString(&dtRet, dateBuff, 31);
                        sb.AppendSQuoted(dateBuff);
                    }
                    if (dataMax != NULL && !dataMax->IsNull())
                    {
                        if(dataMin != NULL && !dataMin->IsNull())
                            sb.Append(" AND ", 5);
                        
                        sb.AppendDQuoted(propName);
                        if(rgConstr->GetMinInclusive())
                            sb.Append("<=", 2);
                        else
                            sb.Append("<", 1);
                        dataValue = static_cast<FdoDateTimeValue*>(dataMax.p);
                        dtRet = dataValue->GetDateTime();
                        DateToString(&dtRet, dateBuff, 31);
                        sb.AppendSQuoted(dateBuff);
                    }
                }

                sb.Append(")", 1);
            }
        }
        else
        {
            FdoPropertyValueConstraintList* lsConstr = static_cast<FdoPropertyValueConstraintList*>(constr.p);
            FdoPtr<FdoDataValueCollection> dataColl = lsConstr->GetConstraintList();
            int cnt = (dataColl != NULL) ? dataColl->GetCount() : 0;
            if (cnt != 0)
            {
                sb.Append(" CONSTRAINT CHK_", 16);
                sb.Append(GenerateValidConstrName(propName).c_str());
                sb.Append(" CHECK(", 7);
                sb.AppendDQuoted(propName);
                sb.Append(" IN(", 4);
                for(int idx = 0; idx < cnt; idx++)
                {
                    FdoPtr<FdoDataValue> dataItem = dataColl->GetItem(idx);
                    if(dataItem != NULL && dataItem->GetDataType() == FdoDataType_DateTime && !dataItem->IsNull())
                    {
                        dataValue = static_cast<FdoDateTimeValue*>(dataItem.p);
                        dtRet = dataValue->GetDateTime();
                        DateToString(&dtRet, dateBuff, 31);
                        sb.AppendSQuoted(dateBuff);
                    }
                    if (idx != (cnt-1))
                        sb.Append(",", 1);
                }
                sb.Append("))", 2);
            }
        }
    }
    else if (constr->GetConstraintType() == FdoPropertyValueConstraintType_Range)
    {
        FdoPropertyValueConstraintRange* rgConstr = static_cast<FdoPropertyValueConstraintRange*>(constr.p);
        FdoPtr<FdoDataValue> dataMin = rgConstr->GetMinValue();
        FdoPtr<FdoDataValue> dataMax = rgConstr->GetMaxValue();
        if(dataMin != NULL || dataMax != NULL)
        {
            sb.Append(" CONSTRAINT CHK_", 16);
            sb.Append(GenerateValidConstrName(propName).c_str());
            sb.Append(" CHECK(", 7);
            if(dataMin != NULL && dataMax != NULL && rgConstr->GetMinInclusive() && rgConstr->GetMaxInclusive())
            {
                sb.AppendDQuoted(propName);
                sb.Append(" BETWEEN ", 9);
                sb.Append(dataMin->ToString());
                sb.Append(" AND ", 5);
                sb.Append(dataMax->ToString());
            }
            else
            {
                if(dataMin != NULL)
                {
                    sb.AppendDQuoted(propName);
                    if(rgConstr->GetMinInclusive())
                        sb.Append(">=", 2);
                    else
                        sb.Append(">", 1);
                    sb.Append(dataMin->ToString());
                }
                if (dataMax != NULL)
                {
                    if(dataMin != NULL)
                        sb.Append(" AND ", 5);

                    sb.AppendDQuoted(propName);
                    if(rgConstr->GetMaxInclusive())
                        sb.Append("<=", 2);
                    else
                        sb.Append("<", 1);
                    sb.Append(dataMax->ToString());
                }
            }
            sb.Append(")", 1);
        }
    }
    else
    {
        FdoPropertyValueConstraintList* lsConstr = static_cast<FdoPropertyValueConstraintList*>(constr.p);
        FdoPtr<FdoDataValueCollection> dataColl = lsConstr->GetConstraintList();
        bool needsSQ = (prop->GetDataType() == FdoDataType_String);
        int cnt = (dataColl != NULL) ? dataColl->GetCount() : 0;
        if (cnt != 0)
        {
            sb.Append(" CONSTRAINT CHK_", 16);
            sb.Append(GenerateValidConstrName(propName).c_str());
            sb.Append(" CHECK(", 7);
            sb.AppendDQuoted(propName);
            sb.Append(" IN(", 4);
            for(int idx = 0; idx < cnt; idx++)
            {
                FdoPtr<FdoDataValue> dataItem = dataColl->GetItem(idx);
                sb.Append(dataItem->ToString());
                if (idx != (cnt-1))
                    sb.Append(",", 1);
            }
            sb.Append("))", 2);
        }
    }
}

void SltConnection::AddComplexUniqueConstraints(FdoUniqueConstraintCollection* uniqueConstr, FdoClassDefinition* fc, StringBuffer& sb)
{
    int cntConstr = uniqueConstr->GetCount();
    for (int i = 0; i < cntConstr; i++)
    {
        FdoPtr<FdoUniqueConstraint> uqc = uniqueConstr->GetItem(i);

        sb.Append("CONSTRAINT UNQ_", 15);
        sb.Append(GenerateValidConstrName(fc->GetName()).c_str());
        sb.Append(i+1);
        sb.Append(" UNIQUE (", 9);
        FdoPtr<FdoDataPropertyDefinitionCollection> pdc = uqc->GetProperties();
        int cnt = pdc->GetCount();
        for(int idx = 0; idx < cnt; idx++)
        {
            FdoPtr<FdoDataPropertyDefinition> item = pdc->GetItem(idx);
            sb.AppendDQuoted(item->GetName());
            sb.Append(",", 1);
        }
        sb.Data()[sb.Length()-1] = ')';
        // those values will be replaced at the end with ");"
        sb.Append(", ", 2);
    }
}

void SltConnection::AddClassPrimaryKeys(FdoClassDefinition* fc, StringBuffer& sb)
{
    FdoPtr<FdoClassDefinition> fctmp = FDO_SAFE_ADDREF(fc);
    sb.Append("PRIMARY KEY(", 12);
    while (fctmp != NULL)
    {
        FdoPtr<FdoDataPropertyDefinitionCollection> pdic = fctmp->GetIdentityProperties();
        int cnt = pdic->GetCount();
        for(int idx = 0; idx < cnt; idx++)
        {
            FdoPtr<FdoDataPropertyDefinition> item = pdic->GetItem(idx);
            sb.AppendDQuoted(item->GetName());
            sb.Append(",", 1);
        }
        fctmp = fctmp->GetBaseClass();
    }
    sb.Data()[sb.Length()-1] = ')';
    // those values will be replaced at the end with ");"
    sb.Append(", ", 2);
}

void SltConnection::CollectBaseClassProperties(FdoClassCollection* myclasses, FdoClassDefinition* fc, FdoClassDefinition* mainfc, 
                                               StringBuffer& sb, int mode, UniqueConstraints& simpleUniqueConstr)
{
    FdoPtr<FdoClassDefinition> baseFc = fc->GetBaseClass();
    if (NULL != baseFc)
        CollectBaseClassProperties(myclasses, baseFc, mainfc, sb, mode, simpleUniqueConstr);

    FdoPtr<FdoPropertyDefinitionCollection> pdc = fc->GetProperties();
    FdoPtr<FdoDataPropertyDefinitionCollection> idpdc = fc->GetIdentityProperties();
    FdoPtr<FdoGeometricPropertyDefinition> gpd;
    
    if (fc->GetClassType() == FdoClassType_FeatureClass)
        gpd = ((FdoFeatureClass*)fc)->GetGeometryProperty();

    if (mode == 0)
    {
        int cntprops = idpdc->GetCount();
		int autoGenCnt = 0;
        for (int i=0; i<cntprops; i++)
        {
            FdoPtr<FdoDataPropertyDefinition> dpd = idpdc->GetItem(i);
			if (dpd->GetIsAutoGenerated())
				autoGenCnt++;
            sb.AppendDQuoted(dpd->GetName());
            sb.Append(" ", 1);
            sb.Append(g_fdo2sql_map[dpd->GetDataType()].c_str());

            AddPropertyConstraintDefaultValue(dpd, sb);
            if (!simpleUniqueConstr.empty())
                AddPropertyUniqueConstraint(simpleUniqueConstr, dpd, sb);
            sb.Append(", ", 2);

            //Add an entry to the metadata type table, if needed
            AddDataCol(dpd, mainfc->GetName());
        }
		if (autoGenCnt > 1)
			throw FdoException::Create(L"Primary key with multiple auto-generated properties is not supported.", 1);
    }
    else if (mode == 1)
    {
        if (idpdc->GetCount() > 0)
        {
            if (idpdc->GetCount() > 1)
            {
                return;
            }
            else
            {
                FdoPtr<FdoDataPropertyDefinition> idp = idpdc->GetItem(0);
                FdoDataType dt = idp->GetDataType();
                // in case it's a decimal with no digits to the right of the decimal point, take
                // it as int64 since it's faster and it will create a closer copy of original schema
                if (idp->GetIsAutoGenerated() 
                    || dt == FdoDataType_Int32 
                    || dt == FdoDataType_Int64
                    || (dt == FdoDataType_Decimal && idp->GetScale() == 0 && idp->GetPrecision() != 0))
                {
                    //autogenerated ID -- we will have SQLite generate a new one for us
                    sb.AppendDQuoted(idp->GetName());
                    if (idp->GetIsAutoGenerated())
                        sb.Append(" INTEGER PRIMARY KEY", 20);
                    else
                    {
                        // 'NOT NULL' will be added by below call to AddPropertyConstraintDefaultValue()
                        // only in case (!prop->GetNullable() && !prop->GetIsAutoGenerated())
                        if (!idp->GetNullable() && !idp->GetIsAutoGenerated())
                            sb.Append(" INTEGER PRIMARY KEY", 20);
                        else
                            sb.Append(" INTEGER PRIMARY KEY NOT NULL", 29);
                    }
                }
                else
                {
                    sb.AppendDQuoted(idp->GetName());
                    sb.Append(" ", 1);
                    sb.Append(g_fdo2sql_map[idp->GetDataType()].c_str());
                    sb.Append(" PRIMARY KEY", 12);
                }

                AddPropertyConstraintDefaultValue(idp, sb);
                if (!simpleUniqueConstr.empty())
                    AddPropertyUniqueConstraint(simpleUniqueConstr, idp, sb);
                sb.Append(", ", 2);

                //Add to the metadata table, if necessary
                AddDataCol(idp, mainfc->GetName());
            }
        }
    }
    else if (mode == 2)
    {
        //now do the geometry property -- we want that to be near the 
        //beginning of the list for better performance when reading
        if (gpd.p && gpd->GetElementState() != FdoSchemaElementState_Deleted && gpd->GetElementState() != FdoSchemaElementState_Detached )
        {
            sb.AppendDQuoted(gpd->GetName());
            sb.Append(" BLOB, ", 7);

            //also add to the geometry_columns table
            AddGeomCol(gpd.p, mainfc->GetName());
        }
    }
    else
    {
        //add the remaining properties
        for (int i=0; i<pdc->GetCount(); i++)
        {
            FdoPtr<FdoPropertyDefinition> pd = pdc->GetItem(i);
            if( pd->GetElementState() == FdoSchemaElementState_Deleted || pd->GetElementState() == FdoSchemaElementState_Detached )
                continue;

            FdoPropertyType ptype = pd->GetPropertyType();

            if (ptype == FdoPropertyType_DataProperty)
            {
                FdoDataPropertyDefinition* dpd = (FdoDataPropertyDefinition*)pd.p;

                //make sure it is not the id property, we did that already
                if (idpdc->Contains(dpd->GetName()))
                    continue;

                // SQLite don't support non PK auto generated properties.
                if (dpd->GetIsAutoGenerated())
                    throw FdoException::Create(L"Non-primary key auto generated properties are not supported.", 1);

                sb.AppendDQuoted(dpd->GetName());
                sb.Append(" ", 1);
                sb.Append(g_fdo2sql_map[dpd->GetDataType()].c_str());

                AddPropertyConstraintDefaultValue(dpd, sb);
                if (!simpleUniqueConstr.empty())
                    AddPropertyUniqueConstraint(simpleUniqueConstr, dpd, sb);
                sb.Append(", ", 2);

                //Add an entry to the metadata type table, if needed
                AddDataCol(dpd, mainfc->GetName());
            }
            else if (ptype == FdoPropertyType_GeometricProperty)
            {
                //make sure it is not THE geometry property which we already did
                if (gpd != NULL && wcscmp(gpd->GetName(), pd->GetName()) == 0)
                    continue;

                sb.AppendDQuoted(pd->GetName());
                sb.Append(" BLOB, ", 7);

                AddGeomCol((FdoGeometricPropertyDefinition*)pd.p, mainfc->GetName());
            }
        }
    }
}

bool SltConnection::GetExtentAndCountInfo(FdoFeatureClass* fc, FdoFilter* filter, bool isExtentReq, FdoInt64* countReq, DBounds* extReq)
{
    *countReq = 0;

    const wchar_t* wfc = fc->GetName();
    size_t wlen = wcslen(wfc);
    size_t clen = 4 * wlen+1;
    char* mbfc = (char*)alloca(4*wlen+1);
    W2A_FAST(mbfc, clen, wfc, wlen);
    StringBuffer sbfcn;
    char* mbfcname = mbfc;

    DBounds bbox;
    std::vector<__int64>* rowids = NULL;
    StringBuffer strWhere((size_t)0);
    bool canFastStep = true;
    bool mustKeepFilterAlive = false;

    const wchar_t* idClassProp = L"rowid";
    SltMetadata* md = GetMetadata(mbfc);
    if (md == NULL)
    {
        std::wstring errVal(L"Class '");
        errVal.append(wfc);
        errVal.append(L"' is not found");
        throw FdoException::Create(errVal.c_str(), 1);
    }
    if (md->IsView())
    {
        if (md->GetIdName() == NULL)
        {
            CacheViewContent(mbfc);
            sbfcn.Reset();
            sbfcn.Append("$view", 5);
            sbfcn.Append(wfc);
            mbfc = sbfcn.Data();
        }
        else
            idClassProp = md->GetIdName();
    }

    //Translate the filter from FDO to SQLite where clause.
    //Also detect if there is a BBOX query that we can accelerate
    //by using the spatial index.
    if (filter)
    {
        SltQueryTranslator qt(fc);
        filter->Process(&qt);

        const char* txtFilter = qt.GetFilter();
        if (*txtFilter) 
            strWhere.Append(txtFilter);
        qt.GetBBOX(bbox);
        rowids = qt.DetachIDList();
        canFastStep = qt.CanUseFastStepping();
        mustKeepFilterAlive = qt.MustKeepFilterAlive();
    }

    SpatialIterator* siter = NULL;
    RowidIterator* ri = NULL;


    //if we have a query by specific ID, it will take precedence over spatial query
    if (rowids)
    {
        ri = new RowidIterator(-1, rowids);
    }
    else if (!bbox.IsEmpty())
    {
        //if we have a BBOX filter, we need to get the spatial index
        SpatialIndex* si = GetSpatialIndex(mbfcname);

        DBounds total_ext;
        si->GetTotalExtent(total_ext);

        if (bbox.Contains(total_ext))
        {
            //only use spatial iterator if the search bounds does not
            //fully contain the data bounds
        }
        else if (bbox.Intersects(total_ext))
        {
            siter = new SpatialIterator(bbox, si);
        }
        else
        {
            // enforce an empty result since result will be empty
            rowids = new std::vector<__int64>();
            ri = new RowidIterator(-1, rowids);
        }
    }

    FdoPtr<FdoIdentifierCollection> props = FdoIdentifierCollection::Create();
    if (isExtentReq)
    {
        FdoPtr<FdoGeometricPropertyDefinition> geomProp = fc->GetGeometryProperty();
        FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(geomProp->GetName());
        props->Add(idf);
    }
    else
    {
        FdoPtr<FdoIdentifier> idf = FdoIdentifier::Create(idClassProp);
        props->Add(idf);
    }

    SltReader* rdr = new SltReader(this, props, mbfcname, strWhere.Data(), siter, canFastStep, ri, NULL);
    FdoPtr<FdoIDataReader> rdrAutoDel = rdr; // in case of exception this smart ptr will delete the reader
    DBounds ext;
    while(rdr->ReadNext())
    {
        if (isExtentReq)
        {
            int len = 0;
            const FdoByte* geom = rdr->GetGeometry(0, &len);
            if (len)
            {
                //TODO: assumes DBounds is 2D since GetFgfExtents is 2D
                GetFgfExtents((unsigned char*)geom, len, (double*)&ext);
                DBounds::Union(extReq, &ext, extReq);
            }
        }
        *countReq += 1;
    }
    // empty result !?
    return (*countReq == 0);
}

SltReader* SltConnection::CheckForSpatialExtents(FdoIdentifierCollection* props, FdoFeatureClass* fc, FdoFilter* filter)
{
    //Checks for a SpatialExtents or Count request
    std::string fcname = W2A_SLOW(fc->GetName());
    std::string gname;
    std::string extname = ""; //if this remains empty, we know extents is not needed
    std::string countname = "";

    int propsCount = props->GetCount();
    bool error = false;
    SltScCHelperTranslator expTrans(fc);
    for (int i=0; i<propsCount; i++)
    {
        FdoPtr<FdoIdentifier> identifier = props->GetItem(i);
        identifier->Process(&expTrans);
        if (expTrans.IsError()) // delay error to check calls to SpatialExtents()
            error = true;
    }
    FdoString* spContxName = expTrans.GetSContextName();
    FdoString* spCountName = expTrans.GetCountName();

    bool countIsFirst = true;
    if (spContxName != NULL && *spContxName != '\0')
    {
        // we do not support SpatialExtents() mixed with other identifiers
        if (propsCount == 2 && (spCountName == NULL || spCountName == '\0'))
            throw FdoException::Create(L"SpatialExtents() can be mixed only with Count()", 1);

        FdoPtr<FdoIdentifier> identifier = props->GetItem(0);
        countIsFirst = (wcscmp(spContxName, identifier->GetName()) != 0);
    }
    else if (propsCount == 2) // in case we mix Count() with other identifiers do it in a different way.
        return NULL;

    // in case we have error or multiple identifiers in select return NULL
    if (error || (propsCount != 1 && propsCount != 2))
        return NULL;

    extname = W2A_SLOW(spContxName);
    countname = W2A_SLOW(spCountName);

    FdoInt64 countReq = -1;
    DBounds extReq;
    bool emptyResult = false;
    if (filter)
        emptyResult = GetExtentAndCountInfo(fc, filter, !extname.empty(), &countReq, &extReq);

    //ok this is a spatial extents or count computed property.
    //Look up the extents and count and return a reader with them.
    //We will always return count, but not extents, if they are not needed.

    //create a temporary table to hold return result
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);

    sqlite3_stmt* stmt;
    const char* tail = NULL;
    StringBuffer sql;

    //get the count
    //we always use the count, whether we compute extents or not
    FdoInt64 count = (countname.empty() || emptyResult) ? 0 : ((countReq != -1) ? countReq : GetFeatureCount(fcname.c_str()));

    //case where we only need count
    if (extname.empty())
    {
        sql.Append("CREATE TABLE SpatialExtentsResult(", 34);
        sql.AppendDQuoted(countname.c_str());
        sql.Append(" INTEGER);", 10);

        char* err;
        rc = sqlite3_exec(db, sql.Data(), NULL, NULL, &err);

        if (rc)
            sqlite3_free(err);

        //insert into the temporary table
        //TODO: some day we should check the error codes...
        //but really, failure is not an option here.
        rc = sqlite3_prepare_v2(db, "INSERT INTO SpatialExtentsResult VALUES(?);", -1, &stmt, &tail);
        rc = sqlite3_bind_int64(stmt, 1, count);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
    }
    else
    {
        //get the extents and convert to blob
        DBounds ext;
        if (extReq.IsEmpty() && !emptyResult)
        {
            SpatialIndex* si = GetSpatialIndex(fcname.c_str());
            si->GetTotalExtent(ext);
        }
        else
            ext = extReq;

        //convert to FGF byte array
        FgfPolygon poly;
        poly.np = 5;

        poly.p[0] = ext.min[0];   poly.p[1] = ext.min[1];
        poly.p[2] = ext.max[0];   poly.p[3] = ext.min[1];
        poly.p[4] = ext.max[0];   poly.p[5] = ext.max[1];
        poly.p[6] = ext.min[0];   poly.p[7] = ext.max[1];
        poly.p[8] = ext.min[0];   poly.p[9] = ext.min[1];

        sql.Append("CREATE TABLE SpatialExtentsResult (\"", 36);
        sql.Append(extname.c_str(), extname.size());
        if (!countname.empty())
        {
            sql.Append("\" BLOB,\"", 8);
            sql.Append(countname.c_str(), countname.size());
            sql.Append("\" INTEGER);", 11);
        }
        else
            sql.Append("\" BLOB);", 8);
        
        char* err;
        rc = sqlite3_exec(db, sql.Data(), NULL, NULL, &err);

        if (rc)
            sqlite3_free(err);

        //insert into the temporary table
        //TODO: some day we should check the error codes...
        //but really, failure is not an option here.
        if (!countname.empty())
            rc = sqlite3_prepare_v2(db, "INSERT INTO SpatialExtentsResult VALUES(?,?);", -1, &stmt, &tail);
        else
            rc = sqlite3_prepare_v2(db, "INSERT INTO SpatialExtentsResult VALUES(?);", -1, &stmt, &tail);
        if (ext.IsEmpty())
            rc = sqlite3_bind_blob(stmt, 1, NULL, 0, SQLITE_TRANSIENT);
        else
            rc = sqlite3_bind_blob(stmt, 1, &poly, sizeof(FgfPolygon), SQLITE_TRANSIENT);
        if (countname.size() != 0)
            rc = sqlite3_bind_int64(stmt, 2, count);
        rc = sqlite3_step(stmt);
        rc = sqlite3_finalize(stmt);
    }

    stmt = NULL;
    tail = NULL;
    sql.Reset();
    sql.Append("SELECT ", 7);
    if (!countname.empty())
    {
        if (countIsFirst)
        {
            sql.AppendDQuoted(countname.c_str());
            sql.Append(",", 1);
            sql.AppendDQuoted(extname.c_str());
        }
        else
        {
            sql.AppendDQuoted(extname.c_str());
            sql.Append(",", 1);
            sql.AppendDQuoted(countname.c_str());
        }
    }
    else
        sql.AppendDQuoted(extname.c_str());
    sql.Append(" FROM SpatialExtentsResult;", 27);

    rc = sqlite3_prepare_v2(db, sql.Data(), -1, &stmt, &tail);

    if (rc == SQLITE_OK)
    {
        FdoPtr<FdoFeatureClass> cls = FdoFeatureClass::Create(L"GenSpatialExtents", NULL);
        FdoPtr<FdoPropertyDefinitionCollection> props = cls->GetProperties();

        // add count
        if (!countname.empty())
        {
            FdoPtr<FdoDataPropertyDefinition> dpdcnt = FdoDataPropertyDefinition::Create(expTrans.GetCountName(), NULL);
		    dpdcnt->SetDataType(FdoDataType_Int64);
            props->Add(dpdcnt);
        }

        if (!extname.empty())
        {            
            // we need extent
            FdoPtr<FdoGeometricPropertyDefinition> geom = FdoGeometricPropertyDefinition::Create(expTrans.GetSContextName(), NULL);
            props->Add(geom);
            cls->SetGeometryProperty(geom);
        }
        if (!countIsFirst && !countname.empty())
        {
            FdoPtr<FdoPropertyDefinition> cntProp = props->GetItem(0);
            props->RemoveAt(0);
            props->Add(cntProp);
        }
        return new SltReader(this, stmt, ReaderCloseType_CloseDb, cls, NULL);        
    }
    else
    {
        const char* err = sqlite3_errmsg(db);
        if (err != NULL)
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);
        else
            throw FdoException::Create(L"Failed to generate Count() or SpatialExtents() reader.", rc);
    }
}

FdoInt64 SltConnection::GetFeatureCount(const char* table)
{
    //get the feature count -- this is approximate since
    //a real count would take a long time (table scan)

    sqlite3_stmt* stmt;
    const char* tail = NULL;

    StringBuffer sb;
    sb.Append("SELECT MAX(ROWID) FROM ", 23);
    sb.AppendDQuoted(table);
    sb.Append(";", 1);

    if (sqlite3_prepare_v2(m_dbWrite, sb.Data(), -1, &stmt, &tail) == SQLITE_OK)
    {
        int rc = sqlite3_step(stmt);
        int count = sqlite3_column_int(stmt, 0);
        sqlite3_finalize(stmt);
        return count;
    }

    return -1;
}

// Do not use it in other functions than GetCachedParsedStatement
// Statement is not cached -- make one, and also add an entry for it
// into the cache table
#define SQL_PREPARE_CACHEPARSEDSTM() {                                           \
    const char* tail = NULL;                                                       \
    int rc = sqlite3_prepare_v2(m_dbWrite, sql, -1, &ret, &tail);   \
    if (rc != SQLITE_OK || ret == NULL){                                           \
        const char* err = sqlite3_errmsg(m_dbWrite);                \
        if (err != NULL)                                                           \
            throw FdoException::Create(A2W_SLOW(err).c_str(), rc);                 \
        else                                                                       \
            throw FdoException::Create(L"Failed to parse SQL statement", rc);      \
        }                                                                          \
}                                                                                  \


sqlite3_stmt* SltConnection::GetCachedParsedStatement(const char* sql)
{
    //Don't let too many queries get cached.
    //There are legitimate cases where lots of different
    //queries can be issued on the same connection.
    if (m_mCachedQueries.size() > 1000)
    {
        ClearQueryCache();
    }

    sqlite3_stmt* ret = NULL;

    QueryCache::iterator iter = m_mCachedQueries.find((char*)sql);
    
    if (iter != m_mCachedQueries.end())
    {
        //found a cached statement -- take it from the cache
        //and return it

        QueryCacheRecList& lst = iter->second;
        for(size_t i=0; i<lst.size(); i++)
        {
            QueryCacheRec& rec = lst[i];
            if (!rec.inUse)
            {
                rec.inUse = true;
                ret = rec.stmt;
                break;
            }
        }
        if (ret == NULL)
        {
            // to avoid a m_mCachedQueries.find() we will clone this small part
            SQL_PREPARE_CACHEPARSEDSTM();
            lst.push_back(QueryCacheRec(ret));
        }
    }
    else
    {
        // to avoid a m_mCachedQueries.find() we will clone this small part
        SQL_PREPARE_CACHEPARSEDSTM();
        m_mCachedQueries[_strdup(sql)].push_back(QueryCacheRec(ret));
    }
    if (ret == NULL)
        throw FdoException::Create(L"Failed to create SQL statement");

    return ret;
}


void SltConnection::ReleaseParsedStatement(const char* sql, sqlite3_stmt* stmt)
{
    QueryCache::iterator iter = m_mCachedQueries.find((char*)sql);

    if (iter != m_mCachedQueries.end())
    {
        QueryCacheRecList& lst = iter->second;
        for(size_t i=0; i<lst.size(); i++)
        {
            QueryCacheRec& rec = lst[i];

            if (rec.stmt == stmt)
            {
                sqlite3_reset(stmt);
                rec.inUse = false;
                return;
            }
        }
    }
    sqlite3_finalize(stmt);
}

void SltConnection::ClearQueryCache()
{
    //We have to keep all cached statements that are still in use, 
    //we can only free the ones that are not in use
    QueryCache newCache;

    for (QueryCache::iterator iter = m_mCachedQueries.begin(); 
        iter != m_mCachedQueries.end(); iter++)
    {
        QueryCacheRecList& lst = iter->second;

        bool stringInUse = false;

        for (size_t i=0; i<lst.size(); i++)
        {
            QueryCacheRec& rec = lst[i];
            
            if (rec.inUse)
            {
                //If the query is in use, carry it over to the new cache map

                stringInUse = true;
                newCache[iter->first].push_back(rec);
            }
            else
            {
                //otherwise free the query and the query string

                sqlite3_finalize(rec.stmt);
            }
        }

        if (!stringInUse)
            free(iter->first); //it was created via strdup, must use free()
    }

    m_mCachedQueries = newCache;
}

bool SltConnection::GetExtents(const wchar_t* fcname, double ext[4])
{
    std::string table = W2A_SLOW(fcname);

    SpatialIndex* si = GetSpatialIndex(table.c_str());

    DBounds dext;
    if (si)
    {
        si->GetTotalExtent(dext);

        ext[0] = dext.min[0];
        ext[1] = dext.min[1];
        ext[2] = dext.max[0];
        ext[3] = dext.max[1];
    }
    return !dext.IsEmpty();
}

// function added to handle old files already created 
// without support for geometry_dettype
bool SltConnection::SupportsDetailedGeomType()
{
    if (m_cSupportsDetGeomType == -1)
    {
        m_cSupportsDetGeomType = 0;
        Table* table = sqlite3FindTable(m_dbWrite, "geometry_columns", 0);
        if( table )
        {
            for(int idx = 0; idx < table->nCol; idx++)
            {
                if (sqlite3StrICmp((table->aCol+idx)->zName, "geometry_dettype") == 0)
                {
                    m_cSupportsDetGeomType = 1;
                    break;
                }
            }
        }
    }
    return (m_cSupportsDetGeomType != 0);
}

FdoITransaction* SltConnection::BeginTransaction()
{
    if (m_dbWrite == NULL)
        throw FdoException::Create(L"Connection is not opened");
    StartTransaction(true);
    return new SltTransaction(this);
}

//Turn on in order to use SAVEPOINTS rather than BEGIN/COMMIT
//for internal transactions. Internally transactions are used for
//batching bulk updates and bulk inserts.
#define SAVEPOINTS_INTERNAL

int SltConnection::StartTransaction(bool isUserTrans)
{
    if (m_dbWrite == NULL)
        return SQLITE_MISUSE;

    int rc = SQLITE_OK;
    if (!isUserTrans)
    {
        if (m_transactionState == SQLiteActiveTransactionType_None)
        {
#ifdef SAVEPOINTS_INTERNAL
            rc = sqlite3_exec(m_dbWrite, "SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "BEGIN;", NULL, NULL, NULL);
#endif
            if (rc == SQLITE_OK) // do we need it!?
                m_transactionState = SQLiteActiveTransactionType_Internal;
        }
    }
    else
    {
        if (m_transactionState == SQLiteActiveTransactionType_User)
        {
            // transaction already started exception
            throw FdoException::Create(L"Transactions is already started");
        }
        else if (m_transactionState == SQLiteActiveTransactionType_Internal)
        {
            // commit internal transaction and open an user transaction
#ifdef SAVEPOINTS_INTERNAL
            rc = sqlite3_exec(m_dbWrite, "RELEASE SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "COMMIT;", NULL, NULL, NULL);
#endif
            m_transactionState = SQLiteActiveTransactionType_None;
        }

        rc = sqlite3_exec(m_dbWrite, "BEGIN;", NULL, NULL, NULL);

        if (rc == SQLITE_OK)
            m_transactionState = SQLiteActiveTransactionType_User;
        else
            throw FdoException::Create(L"SQLite begin transaction failed!", rc);
    }
    return rc;
}

int SltConnection::CommitTransaction(bool isUserTrans)
{
    if (m_dbWrite == NULL)
        return SQLITE_MISUSE;

    int rc = SQLITE_OK;
    if (!isUserTrans)
    {
        if (m_transactionState == SQLiteActiveTransactionType_Internal)
        {
#ifdef SAVEPOINTS_INTERNAL
            rc = sqlite3_exec(m_dbWrite, "RELEASE SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "COMMIT;", NULL, NULL, NULL);
#endif
            if (rc == SQLITE_OK)
                m_transactionState = SQLiteActiveTransactionType_None;
            // else we need to handle this internally
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::commit_hook(this);
        }
    }
    else
    {
        if (m_transactionState == SQLiteActiveTransactionType_User)
        {
            // commit internal transaction and open an user transaction
            rc = sqlite3_exec(m_dbWrite, "COMMIT;", NULL, NULL, NULL);

            if (rc == SQLITE_OK)
                m_transactionState = SQLiteActiveTransactionType_None;
            else
                throw FdoException::Create(L"SQLite commit transaction failed!", rc);
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::commit_hook(this);
        }
        else
            throw FdoException::Create(L"No active transaction to commit");
    }
    return rc;
}

int SltConnection::RollbackTransaction(bool isUserTrans)
{
    if (m_dbWrite == NULL)
        return SQLITE_MISUSE;

    int rc = SQLITE_OK;
    if (!isUserTrans)
    {
        if (m_transactionState == SQLiteActiveTransactionType_Internal)
        {
#ifdef SAVEPOINTS_INTERNAL
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK TO SAVEPOINT sp;", NULL, NULL, NULL);
#else
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK;", NULL, NULL, NULL);
#endif
            m_transactionState = SQLiteActiveTransactionType_None;
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::rollback_hook(this);
        }
    }
    else
    {
        if (m_transactionState == SQLiteActiveTransactionType_User)
        {
            // commit internal transaction and open an user transaction
            rc = sqlite3_exec(m_dbWrite, "ROLLBACK;", NULL, NULL, NULL);

            m_transactionState = SQLiteActiveTransactionType_None;
            
            if (!m_updateHookEnabled && m_changesAvailable)
                SltConnection::rollback_hook(this);
        }
        else
            throw FdoException::Create(L"No active transaction to rollback");
    }
    return rc;
}

void SltConnection::CacheViewContent(const char* viewName)
{
    StringBuffer sb;
    sb.Append("$view", 5);
    sb.Append(viewName);
    Table* table = sqlite3FindTable(m_dbWrite, sb.Data(), 0);
    if (table == NULL)
    {
        sb.Reset();
        sb.Append("CREATE TEMP TABLE IF NOT EXISTS ", 32);
        sb.Append("\"$view", 6);
        sb.Append(viewName);
        sb.Append("\" AS SELECT * FROM ", 19);
        sb.AppendDQuoted(viewName);
        sb.Append(";", 1);
        sqlite3_exec(m_dbWrite, sb.Data(), NULL, NULL, NULL);
    }
}

// below section is a pain but is no other way to "see" the changes on update & delete
void SltConnection::update_hook(void* caller, int action, char const* database,
                                char const* tablename, sqlite3_int64 id)
{
    switch (action)
    {
    case SQLITE_DELETE:
        {
            SltConnection* conn = static_cast<SltConnection*>(caller);
            SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.find((char*)tablename);
            if (iter != conn->m_mNameToSpatialIndex.end())
            {
                iter->second->AddRowIdToDeleteList(id);
                conn->m_changesAvailable = true;
            }
        }
        break;
    case SQLITE_UPDATE:
        {
            SltConnection* conn = static_cast<SltConnection*>(caller);
            SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.find((char*)tablename);
            if (iter != conn->m_mNameToSpatialIndex.end())
            {
                iter->second->AddRowIdToUpdateList(id);
                conn->m_changesAvailable = true;
            }
        }
        break;
        // this even can be fired ONLY by SQL commands and it will process only id < last id inserted
    case SQLITE_INSERT:
        {
            SltConnection* conn = static_cast<SltConnection*>(caller);
            SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.find((char*)tablename);
            if (iter != conn->m_mNameToSpatialIndex.end() &&
                (id < iter->second->GetSpatialIndex()->GetLastInsertedIdx()))
            {
                iter->second->AddRowIdToInsertList(id);
                conn->m_changesAvailable = true;
            }
        }
    default:
        // in this case we do not do anything since
        // insert is handled in a different way performance reasons
        break;
    }
}

int SltConnection::commit_hook(void* caller)
{
    SltConnection* conn = static_cast<SltConnection*>(caller);
    if (conn->m_changesAvailable)
    {
        for (SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.begin();
             iter != conn->m_mNameToSpatialIndex.end(); iter++)
        {
            iter->second->CommitChanges();
        }
        conn->m_changesAvailable = false;
    }
    return 0;
}

void SltConnection::rollback_hook(void* caller)
{
    // since rollback can be hard to maintain just force a SI 
    // rebuild for those who contain changes
    SltConnection* conn = static_cast<SltConnection*>(caller);
    if (conn->m_changesAvailable)
    {
        for (SpatialIndexCache::iterator iter = conn->m_mNameToSpatialIndex.begin();
             iter != conn->m_mNameToSpatialIndex.end(); iter++)
        {
            if (iter->second->ChangesAvailable())
            {
                delete iter->second;
                free(iter->first); //was allocated using strdup
                conn->m_mNameToSpatialIndex.erase(iter);
                // we will get a crash in case we do not set it back
                iter = conn->m_mNameToSpatialIndex.begin();
            }
        }
        conn->m_changesAvailable = false;
    }
}

void SltConnection::EnableHooks(bool enable, bool enforceRollback)
{
    if (enable)
    {
        if (m_updateHookEnabled)
            return;
        sqlite3_update_hook(m_dbWrite, SltConnection::update_hook, this);
        sqlite3_commit_hook(m_dbWrite, SltConnection::commit_hook, this);
        sqlite3_rollback_hook(m_dbWrite, SltConnection::rollback_hook, this);
        m_updateHookEnabled = true;
    }
    else
    {
        sqlite3_update_hook(m_dbWrite, NULL, NULL);
        sqlite3_commit_hook(m_dbWrite, NULL, NULL);
        sqlite3_rollback_hook(m_dbWrite, NULL, NULL);
        m_updateHookEnabled = false;

         // we need to commit since user wants to "see" his change during a transaction
        if (!enforceRollback)
            SltConnection::commit_hook(this);
        else
            SltConnection::rollback_hook(this);
    }
}

bool SltConnection::IsCoordSysLatLong()
{
    if (m_dbWrite == NULL)
        return false;

    const char* sql = "SELECT srid FROM spatial_ref_sys WHERE srtext LIKE '%GEOGCS%' AND srtext NOT LIKE '%PROJCS%';";
    sqlite3_stmt* pStmt = NULL;
    const char* zTail = NULL;
    bool retVal = false;
    if (sqlite3_prepare_v2(m_dbWrite, sql, -1, &pStmt, &zTail) == SQLITE_OK)
    {
        retVal = (sqlite3_step(pStmt) == SQLITE_ROW);
        sqlite3_finalize(pStmt);
    }
    return retVal;
}

bool SltConnection::IsReadOnlyConnection()
{
    return m_isReadOnlyConnection;
}

ConnInfoDetails::ConnInfoDetails(SltConnection* conn)
{
    m_isInitialized = m_isCoordSysLatLong = false;
    m_conn = conn;
}

bool ConnInfoDetails::IsCoordSysLatLong()
{
    if (!m_isInitialized)
    {
        // cache the result to avoid multiple callsto execute
        m_isInitialized = true;
        m_isCoordSysLatLong = m_conn->IsCoordSysLatLong();
    }
    return m_isCoordSysLatLong;
}
